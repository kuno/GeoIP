/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "country.h"
#include "global.h"

Persistent<FunctionTemplate> geoip::Country::constructor_template;

void geoip::Country::Init(Handle<Object> target)
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
  target->Set(String::NewSymbol("Country"), constructor_template->GetFunction());
}

geoip::Country::Country() : db(NULL) {};

geoip::Country::~Country() {
  if (db) {
    GeoIP_delete(db);
  }
};

Handle<Value> geoip::Country::New(const Arguments& args)
{
  HandleScope scope;

  Country *c = new Country();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db != NULL) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION) {
      c->Wrap(args.This());
      return scope.Close(args.This());
    } else {
      GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
      delete c->db;
      return scope.Close(ThrowException(String::New("Error: Not valid country database")));
    }
  } else {
    return scope.Close(ThrowException(String::New("Error: Cannot open database")));
  }
}

Handle<Value> geoip::Country::lookupSync(const Arguments &args) {
  HandleScope scope;

  Country * c = ObjectWrap::Unwrap<Country>(args.This());

  Local<String> host_str = args[0]->ToString();
  Local<Object> data = Object::New();
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  if (ipnum <= 0) {
    return scope.Close(Null());
  } else {
    int country_id = GeoIP_id_by_ipnum(c->db, ipnum);
    if (country_id == 0) {
      return scope.Close(Null());
    } else {
      char * name = _GeoIP_iso_8859_1__utf8(GeoIP_country_name[country_id]);

      data->Set(String::NewSymbol("country_name"), String::New(name));
      data->Set(String::NewSymbol("country_code"), String::New(GeoIP_country_code[country_id]));
      data->Set(String::NewSymbol("country_code3"), String::New(GeoIP_country_code3[country_id]));
      data->Set(String::NewSymbol("continent_code"), String::New(GeoIP_country_continent[country_id]));

      free(name);

      return scope.Close(data);
    }
  }
}

Handle<Value> geoip::Country::lookup(const Arguments& args)
{
  HandleScope scope;

  REQ_FUN_ARG(1, cb);

  Country * c = ObjectWrap::Unwrap<Country>(args.This());
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);

  country_baton_t *baton = new country_baton_t();

  baton->c = c;
  baton->ipnum = _GeoIP_lookupaddress(host_cstr);
  baton->cb = Persistent<Function>::New(cb);

  uv_work_t *req = new uv_work_t;
  req->data = baton;

  uv_queue_work(uv_default_loop(), req, EIO_Country, EIO_AfterCountry);

  return scope.Close(Undefined());
}

void geoip::Country::EIO_Country(uv_work_t *req)
{
  country_baton_t *baton = static_cast<country_baton_t *>(req->data);

  //uint32_t ipnum = _GeoIP_lookupaddress(baton->host_cstr);

  if (baton->ipnum <= 0) {
    baton->country_id = 0;
  } else {
    baton->country_id = GeoIP_id_by_ipnum(baton->c->db, baton->ipnum);
  }
}

void geoip::Country::EIO_AfterCountry(uv_work_t *req)
{
  HandleScope scope;

  country_baton_t *baton = static_cast<country_baton_t *>(req->data);

  Handle<Value> argv[2];

  if (baton->country_id <= 0) {
    argv[0] = Exception::Error(String::New("Data not found"));
    argv[1] = Null();
  } else {
    Local<Object> data = Object::New();

    char * name = _GeoIP_iso_8859_1__utf8(GeoIP_country_name[baton->country_id]);

    data->Set(String::NewSymbol("country_name"), String::New(name));
    data->Set(String::NewSymbol("country_code"), String::New(GeoIP_country_code[baton->country_id]));
    data->Set(String::NewSymbol("country_code3"), String::New(GeoIP_country_code3[baton->country_id]));
    data->Set(String::NewSymbol("continent_code"), String::New(GeoIP_country_continent[baton->country_id]));

    argv[0] = Null();
    argv[1] = data;

    free(name);
  }

  TryCatch try_catch;
  baton->cb->Call(Context::GetCurrent()->Global(), 2, argv);

  // Cleanup
  baton->cb.Dispose();
  delete baton;
  delete req;

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }
}

Handle<Value> geoip::Country::update(const Arguments &args) {
  Locker locker;

  HandleScope scope;

  Country* c = ObjectWrap::Unwrap<Country>(args.This());

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db != NULL) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION) {
      return scope.Close(True());
    } else {
      GeoIP_delete(c->db);  // free()'s the gi reference & closes its fd
      delete c->db;
      return scope.Close(ThrowException(String::New("Error: Not valid country database")));
    }
  } else {
    return scope.Close(ThrowException(String::New("Error: Cannot open database")));
  }

  Unlocker unlocker;
}

void geoip::Country::close(const Arguments &args) {
  Country* c = ObjectWrap::Unwrap<Country>(args.This());
  GeoIP_delete(c->db);  // free()'s the gi reference & closes its fd
  delete c->db;
  HandleScope scope;  // Stick this down here since it seems to segfault when on top?
}
