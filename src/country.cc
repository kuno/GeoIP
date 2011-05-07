/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */

#include "country.h"

//GeoIP *db;
//int db_edition;
//static Persistent<FunctionTemplate> constructor_template;

void geoip::Country::Init(Handle<Object> target)
{
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(2);
  constructor_template->SetClassName(String::NewSymbol("geoip"));

  //NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookup", lookup);
  //NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookup6", lookup6);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookupSync", lookupSync);
  //NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookupSync6", lookupSync6);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "close", close);
  target->Set(String::NewSymbol("Country"), constructor_template->GetFunction());
}

/*
   Country() :
   db_edition(0)
   {
   }

   ~Country()
   {
   }*/

Handle<Value> geoip::Country::New(const Arguments& args)
{
  HandleScope scope;
  Country *c = new Country();

  Local<String> path_str = args[0]->ToString();
  char path_cstr[path_str->Length()];
  path_str->WriteAscii(path_cstr);
  bool cache_on = args[1]->ToBoolean()->Value(); 

  c->db = GeoIP_open(path_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db != NULL) {
    // Successfully opened the file, return 1 (true)
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION ||
        c->db_edition == GEOIP_COUNTRY_EDITION_V6 ||
        c->db_edition == GEOIP_CITY_EDITION_REV0 || 
        c->db_edition == GEOIP_CITY_EDITION_REV1) {
      c->Wrap(args.This());
      return args.This();
    } else {
      GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
      c->db = NULL;                                                       
      return ThrowException(String::New("Error: Not valid database"));
    }
  } else {
    return ThrowException(String::New("Error: Cao not open database"));
  }
}

Handle<Value> geoip::Country::lookupSync(const Arguments &args) {
  HandleScope scope;

  Local<String> host_str = args[0]->ToString();
  Local<Object> r = Object::New();
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);
  Country* c = ObjectWrap::Unwrap<Country>(args.This());

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);
  if (ipnum <= 0) {
    return Null();
  }

  int country_id = GeoIP_id_by_ipnum(c->db, ipnum);
  if (country_id <= 0) {
    return Null();
  }

  r->Set(String::NewSymbol("country_code"), String::New(GeoIP_country_code[country_id]));
  r->Set(String::NewSymbol("country_code3"), String::New(GeoIP_country_code3[country_id]));
  r->Set(String::NewSymbol("country_name"), String::New(GeoIP_country_name[country_id]));
  //r->Set(String::NewSymbol("continent_code"), String::New(gir->continent_code));
  return scope.Close(r);
}

/*  
   static Handle<Value> Country::lookup(const Arguments& args)
   {
   HandleScope scope;

   REQ_FUN_ARG(1, cb);

   Country *c = ObjectWrap::Unwrap<City>(args.This());
   Local<String> host_str = args[0]->ToString();

   city_baton_t *baton = new city_baton_t();

   baton->c = c;
   host_str->WriteAscii(baton->host_cstr);
   baton->increment_by = 2;
   baton->sleep_for = 1;
   baton->cb = Persistent<Function>::New(cb);

   c->Ref();

   eio_custom(EIO_Country, EIO_PRI_DEFAULT, EIO_AfterCity, baton);
   ev_ref(EV_DEFAULT_UC);

   return Undefined();
   }

   static int Country::EIO_Country(eio_req *req)
   {
   city_baton_t *baton = static_cast<city_baton_t *>(req->data);

   sleep(baton->sleep_for);

   baton->r = GeoIP_record_by_name(baton->c->db, baton->host_cstr);

   if (baton->r == NULL) {
   return 1;
   } else {
   return 0;
   }
   }

   static int Country::EIO_AfterCountry(eio_req *req)
   {
   HandleScope scope;

   city_baton_t *baton = static_cast<city_baton_t *>(req->data);
   ev_unref(EV_DEFAULT_UC);
   baton->c->Unref();

   Local<Value> argv[1];
   Local<Object> r = Object::New();
   r->Set(String::NewSymbol("country_code"), String::New(baton->r->country_code));
   r->Set(String::NewSymbol("country_code3"), String::New(baton->r->country_code3));
   r->Set(String::NewSymbol("country_name"), String::New(baton->r->country_name));
   r->Set(String::NewSymbol("region"), String::New(baton->r->region));
   r->Set(String::NewSymbol("postal_code"), String::New(baton->r->postal_code));
   r->Set(String::NewSymbol("latitude"), Number::New(baton->r->latitude));
   r->Set(String::NewSymbol("longitude"), Number::New(baton->r->longitude));
   r->Set(String::NewSymbol("metro_code"), Number::New(baton->r->metro_code));
   r->Set(String::NewSymbol("dma_code"), Number::New(baton->r->dma_code));
   r->Set(String::NewSymbol("area_code"), Number::New(baton->r->area_code));
   r->Set(String::NewSymbol("continent_code"), String::New(baton->r->continent_code));     

   argv[0] = r;

   TryCatch try_catch;

   baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);

   if (try_catch.HasCaught()) {
   FatalException(try_catch);
}

baton->cb.Dispose();

delete baton;
return 0;
}*/

// Destroy the GeoIP* reference we're holding on to
Handle<Value> geoip::Country::close(const Arguments &args) {
  Country* c = ObjectWrap::Unwrap<geoip::Country>(args.This()); 
  GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
  c->db = NULL;
  HandleScope scope;	// Stick this down here since it seems to segfault when on top?
}

Persistent<FunctionTemplate> geoip::Country::constructor_template;
