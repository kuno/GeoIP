/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */

#include "city.h"

void geoip::City::Init(Handle<Object> target)
{
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(2);
  constructor_template->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookup", lookup);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookupSync", lookupSync);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "close", close);
  target->Set(String::NewSymbol("City"), constructor_template->GetFunction());
}

/*
City() :
  db_edition(0)
{
}

~City()
{
} */

Handle<Value> geoip::City::New(const Arguments& args)
{
  HandleScope scope;
  City *c = new City();

  Local<String> path_str = args[0]->ToString();
  char path_cstr[path_str->Length()];
  path_str->WriteAscii(path_cstr);
  bool cache_on = args[1]->ToBoolean()->Value(); 

  c->db = GeoIP_open(path_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db != NULL) {
    // Successfully opened the file, return 1 (true)
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_CITY_EDITION_REV0 || 
        c->db_edition == GEOIP_CITY_EDITION_REV1) {
      c->Wrap(args.This());
      return args.This();
    } else {
      GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
      c->db = NULL;                                                       
      return ThrowException(String::New("Error: Not city database"));
    }
  } else {
    return ThrowException(String::New("Error: Cao not open database"));
  }
}

Handle<Value> geoip::City::lookupSync(const Arguments &args) {
  HandleScope scope;

  Local<String> ip_str = args[0]->ToString();
  Local<Object> r = Object::New();
  char ip_cstr[ip_str->Length()];
  ip_str->WriteAscii(ip_cstr);
  City* c = ObjectWrap::Unwrap<City>(args.This());

  // Here I'm just returning the latitude, longitude
  // If you want to pull out city information, etc,
  //   this is the place to do it.
  GeoIPRecord *gir = GeoIP_record_by_addr(c->db, ip_cstr);

  if (gir != NULL) {
    r->Set(String::NewSymbol("country_code"), String::New(gir->country_code));
    r->Set(String::NewSymbol("country_code3"), String::New(gir->country_code3));
    r->Set(String::NewSymbol("country_name"), String::New(gir->country_name));
    r->Set(String::NewSymbol("region"), String::New(gir->region));
    r->Set(String::NewSymbol("postal_code"), String::New(gir->postal_code));
    r->Set(String::NewSymbol("latitude"), Number::New(gir->latitude));
    r->Set(String::NewSymbol("longitude"), Number::New(gir->longitude));
    r->Set(String::NewSymbol("metro_code"), Number::New(gir->metro_code));
    r->Set(String::NewSymbol("dma_code"), Number::New(gir->dma_code));
    r->Set(String::NewSymbol("area_code"), Number::New(gir->area_code));
    r->Set(String::NewSymbol("continent_code"), String::New(gir->continent_code));
    return scope.Close(r);
  } else {
    return ThrowException(String::New("Error: Can not find match data"));
  }
}

Handle<Value> geoip::City::lookup(const Arguments& args)
{
  HandleScope scope;

  REQ_FUN_ARG(1, cb);

  City *c = ObjectWrap::Unwrap<City>(args.This());
  Local<String> ip_str = args[0]->ToString();

  city_baton_t *baton = new city_baton_t();

  baton->c = c;
  ip_str->WriteAscii(baton->ip_cstr);
  baton->increment_by = 2;
  baton->sleep_for = 1;
  baton->cb = Persistent<Function>::New(cb);

  c->Ref();

  eio_custom(EIO_City, EIO_PRI_DEFAULT, EIO_AfterCity, baton);
  ev_ref(EV_DEFAULT_UC);

  return Undefined();
}

int geoip::City::EIO_City(eio_req *req)
{
  city_baton_t *baton = static_cast<city_baton_t *>(req->data);

  sleep(baton->sleep_for);

  baton->record = GeoIP_record_by_name(baton->c->db, baton->ip_cstr);

  if (baton->record == NULL) {
    return 1;
  } else {
    return 0;
  }
}

int geoip::City::EIO_AfterCity(eio_req *req)
{
  HandleScope scope;

  city_baton_t *baton = static_cast<city_baton_t *>(req->data);
  ev_unref(EV_DEFAULT_UC);
  baton->c->Unref();

  Local<Value> argv[1];
  Local<Object> r = Object::New();
  r->Set(String::NewSymbol("country_code"), String::New(baton->record->country_code));
  r->Set(String::NewSymbol("country_code3"), String::New(baton->record->country_code3));
  r->Set(String::NewSymbol("country_name"), String::New(baton->record->country_name));
  r->Set(String::NewSymbol("region"), String::New(baton->record->region));
  r->Set(String::NewSymbol("postal_code"), String::New(baton->record->postal_code));
  r->Set(String::NewSymbol("latitude"), Number::New(baton->record->latitude));
  r->Set(String::NewSymbol("longitude"), Number::New(baton->record->longitude));
  r->Set(String::NewSymbol("metro_code"), Number::New(baton->record->metro_code));
  r->Set(String::NewSymbol("dma_code"), Number::New(baton->record->dma_code));
  r->Set(String::NewSymbol("area_code"), Number::New(baton->record->area_code));
  r->Set(String::NewSymbol("continent_code"), String::New(baton->record->continent_code));     

  argv[0] = r;

  TryCatch try_catch;

  baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  baton->cb.Dispose();

  delete baton;
  return 0;
}

// Destroy the GeoIP* reference we're holding on to
Handle<Value> geoip::City::close(const Arguments &args) {
  City* c = ObjectWrap::Unwrap<City>(args.This()); 
  GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
  c->db = NULL;
  HandleScope scope;	// Stick this down here since it seems to segfault when on top?
}

Persistent<FunctionTemplate> geoip::City::constructor_template;
