/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "country6.h"
#include "global.h"

Persistent<FunctionTemplate> geoip::Country6::constructor_template; 

void geoip::Country6::Init(Handle<Object> target)
{
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookup", lookup);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookupSync", lookupSync);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "update", update); 
  //NODE_SET_PROTOTYPE_METHOD(constructor_template, "close", close);
  target->Set(String::NewSymbol("Country6"), constructor_template->GetFunction());
}

/*
   geoip::Country6::Country6()
   {
   }

   geoip::Country6::~Country6()
   {
   }*/

Handle<Value> geoip::Country6::New(const Arguments& args)
{
  HandleScope scope;

  Country6 *c = new Country6();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value(); 

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db != NULL) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION_V6) {
      c->Wrap(args.This());
      return scope.Close(args.This());
    } else {
      GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
      c->db = NULL;                                                       
      return scope.Close(ThrowException(String::New("Error: Not valid country database")));
    }
  } else {
    return scope.Close(ThrowException(String::New("Error: Cao not open database")));
  }
}

Handle<Value> geoip::Country6::lookupSync(const Arguments &args) {
  HandleScope scope;

  Country6 * c = ObjectWrap::Unwrap<Country6>(args.This());

  // Check if database is country ipv6 edition
  if (c->db_edition != GEOIP_COUNTRY_EDITION_V6) {
    return scope.Close(ThrowException(String::New("Error: Database is not country ipv6 edition")));
  }

  Local<String> host_str = args[0]->ToString();
  Local<Object> data = Object::New();
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);

  geoipv6_t ipnum_v6 = _GeoIP_lookupaddress_v6(host_cstr);

  if (__GEOIP_V6_IS_NULL(ipnum_v6)) {
    return scope.Close(Null());
  } else {
    int country_id = GeoIP_id_by_ipnum_v6(c->db, ipnum_v6);
    if (country_id == 0) {
      return scope.Close(Null());
    } else {
      data->Set(String::NewSymbol("country_name"), String::New(GeoIP_country_name[country_id]));  
      data->Set(String::NewSymbol("country_code"), String::New(GeoIP_country_code[country_id]));
      data->Set(String::NewSymbol("country_code3"), String::New(GeoIP_country_code3[country_id]));
      data->Set(String::NewSymbol("continent_code"), String::New(GeoIP_country_continent[country_id]));
      return scope.Close(data);
    }
  }
}

Handle<Value> geoip::Country6::lookup(const Arguments& args)
{
  HandleScope scope;

  REQ_FUN_ARG(1, cb);

  Country6 * c = ObjectWrap::Unwrap<Country6>(args.This());

  // Check if database is country ipv6 edition
  if (c->db_edition != GEOIP_COUNTRY_EDITION_V6) {
    return scope.Close(ThrowException(String::New("Error: Database is not country ipv6 edition")));
  }                                                                                                   

  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);

  country6_baton_t *baton = new country6_baton_t();
  baton->c = c;
  baton->ipnum_v6 = _GeoIP_lookupaddress_v6(host_cstr);
  baton->cb = Persistent<Function>::New(cb);

  c->Ref();

  eio_custom(EIO_Country, EIO_PRI_DEFAULT, EIO_AfterCountry, baton);
  ev_ref(EV_DEFAULT_UC);

  return Undefined();
}

int geoip::Country6::EIO_Country(eio_req *req)
{
  Locker locker();

  country6_baton_t *baton = static_cast<country6_baton_t *>(req->data);

  if (__GEOIP_V6_IS_NULL(baton->ipnum_v6)) {
    baton->country_id = 0;
  } else {
    baton->country_id = GeoIP_id_by_ipnum_v6(baton->c->db, baton->ipnum_v6);
  }

  return 0;

  Unlocker unlocker(); 
}

int geoip::Country6::EIO_AfterCountry(eio_req *req)
{
  HandleScope scope;

  country6_baton_t *baton = static_cast<country6_baton_t *>(req->data);
  ev_unref(EV_DEFAULT_UC);
  baton->c->Unref();

  Handle<Value> argv[2];
  if (baton->country_id <= 0) {
    argv[0] = Exception::Error(String::New("Data not found"));
    argv[1] = Null();
  } else {
    Local<Object> data = Object::New();
    data->Set(String::NewSymbol("country_name"), String::New(GeoIP_country_name[baton->country_id]));    
    data->Set(String::NewSymbol("country_code"), String::New(GeoIP_country_code[baton->country_id]));
    data->Set(String::NewSymbol("country_code3"), String::New(GeoIP_country_code3[baton->country_id]));
    data->Set(String::NewSymbol("continent_code"), String::New(GeoIP_country_continent[baton->country_id]));

    argv[0] = Null();
    argv[1] = data;
  }

  TryCatch try_catch;

  baton->cb->Call(Context::GetCurrent()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  baton->cb.Dispose();

  delete baton;
  return 0;
}

Handle<Value> geoip::Country6::update(const Arguments &args) {
  Locker locker();

  HandleScope scope;

  Country6* c = ObjectWrap::Unwrap<Country6>(args.This()); 

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value(); 

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db != NULL) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION_V6) {
      return scope.Close(True());
    } else {
      GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
      c->db = NULL;                                                       
      return scope.Close(ThrowException(String::New("Error: Not valid country database")));
    }
  } else {
    return scope.Close(ThrowException(String::New("Error: Cao not open database")));
  }

 Unlocker unlocker();
}         

Handle<Value> geoip::Country6::close(const Arguments &args) {
  Country6* c = ObjectWrap::Unwrap<Country6>(args.This()); 
  GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
  c->db = NULL;
  HandleScope scope;	// Stick this down here since it seems to segfault when on top?
}
