/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                          

#include "test.h"
#include "global.h"

Persistent<FunctionTemplate> geoip::Test::constructor_template;

void geoip::Test::Init(Handle<Object> target)
{
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookup", lookup);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookupSync", lookupSync);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "close", close);
  target->Set(String::NewSymbol("Test"), constructor_template->GetFunction());
}

geoip::Test::Test()
{
}

geoip::Test::~Test()
{
}

Handle<Value> geoip::Test::New(const Arguments& args)
{
  HandleScope scope;
  Test *c = new Test();

  String::Utf8Value  file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);
  bool cache_on = args[1]->ToBoolean()->Value(); 

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

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
      return ThrowException(String::New("Error: Not valid city database"));
    }
  } else {
    return ThrowException(String::New("Error: Cao not open database"));
  }
}

Handle<Value> geoip::Test::lookupSync(const Arguments &args) {
  HandleScope scope;

  Local<String> host_str = args[0]->ToString();
  Local<Object> data = Object::New();
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);
  Test * c = ObjectWrap::Unwrap<geoip::Test>(args.This());

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);
  if (ipnum <= 0) {
    return Null();
  }

  unsigned int seek_record = _GeoIP_seek_record(c->db, ipnum);
  if (seek_record <= 0 ) {
    return Null();
  }

  GeoIPRecord * record = GeoIP_record_by_ipnum(c->db, ipnum);

  if (record == NULL) {
    return scope.Close(Null());
  }

  if (record->country_code != NULL) {
    data->Set(String::NewSymbol("country_code"), String::New(record->country_code));
  }

  if (record->country_code3 != NULL) {
    data->Set(String::NewSymbol("country_code3"), String::New(record->country_code3));
  }

  if (record->country_name != NULL) {
    data->Set(String::NewSymbol("country_name"), String::New(record->country_name));
  }

  if (record->region != NULL ) {
    data->Set(String::NewSymbol("region"), String::New(record->region));
  }

  if (record->postal_code != NULL) {
    data->Set(String::NewSymbol("postal_code"), String::New(record->postal_code));
  }

  if (record->latitude > 0) {
    data->Set(String::NewSymbol("latitude"), Number::New(record->latitude));
  }

  if (record->longitude > 0) {
    data->Set(String::NewSymbol("longitude"), Number::New(record->longitude));
  }

  if (record->metro_code > 0 ) {
    data->Set(String::NewSymbol("metro_code"), Number::New(record->metro_code));
  }

  if (record->dma_code > 0 ) {
    data->Set(String::NewSymbol("dma_code"), Number::New(record->dma_code));
  }

  if (record->area_code > 0) {
    data->Set(String::NewSymbol("area_code"), Number::New(record->area_code));
  }

  if (record->continent_code > 0) {
    data->Set(String::NewSymbol("continent_code"), String::New(record->continent_code));
  }

  return scope.Close(data);
}

Handle<Value> geoip::Test::lookup(const Arguments& args)
{
  HandleScope scope;

  REQ_FUN_ARG(1, cb);

  Test *c = ObjectWrap::Unwrap<geoip::Test>(args.This());
  Local<String> host_str = args[0]->ToString();

  test_baton_t *baton = new test_baton_t();

  baton->c = c;
  host_str->WriteAscii(baton->host_cstr);
  baton->increment_by = 2;
  baton->sleep_for = 1;
  baton->cb = Persistent<Function>::New(cb);

  c->Ref();

  eio_custom(EIO_Test, EIO_PRI_DEFAULT, EIO_AfterTest, baton);
  ev_ref(EV_DEFAULT_UC);

  return Undefined();
}

int geoip::Test::EIO_Test(eio_req *req)
{
  test_baton_t *baton = static_cast<test_baton_t *>(req->data);

  sleep(baton->sleep_for);

  uint32_t ipnum = _GeoIP_lookupaddress(baton->host_cstr);
  if (ipnum <= 0) {
    baton->record = NULL;
  } else {
    baton->record = GeoIP_record_by_ipnum(baton->c->db, ipnum);
  }

  return 0;
}

int geoip::Test::EIO_AfterTest(eio_req *req)
{
  HandleScope scope;

  test_baton_t * baton = static_cast<test_baton_t *>(req->data);
  ev_unref(EV_DEFAULT_UC);
  baton->c->Unref();

  Handle<Value> argv[2];
  if (baton->record == NULL) {
    argv[0] = Exception::Error(String::New("Error")); //Exception::Error(String::New("Error not found"));
    argv[1] = Null(); //Boolean::New(false);
  } else {

    Local<String> data = String::New("found");
    /*
       Local<Object> data = Object::New();
       if (baton->record->country_code != NULL) {
       data->Set(String::NewSymbol("country_code"), String::New(baton->record->country_code));
       }

       if (baton->record->country_code3 != NULL) {
       data->Set(String::NewSymbol("country_code3"), String::New(baton->record->country_code3));
       }

       if (baton->record->country_name != NULL) {
       data->Set(String::NewSymbol("country_name"), String::New(baton->record->country_name));
       }

       if (baton->record->region != NULL ) {
       data->Set(String::NewSymbol("region"), String::New(baton->record->region));
       }

       if (baton->record->postal_code != NULL) {
       data->Set(String::NewSymbol("postal_code"), String::New(baton->record->postal_code));
       }

       if (baton->record->latitude > 0) {
       data->Set(String::NewSymbol("latitude"), Number::New(baton->record->latitude));
       }

       if (baton->record->longitude > 0) {
       data->Set(String::NewSymbol("longitude"), Number::New(baton->record->longitude));
       }

       if (baton->record->metro_code > 0 ) {
       data->Set(String::NewSymbol("metro_code"), Number::New(baton->record->metro_code));
       }

       if (baton->record->dma_code > 0 ) {
       data->Set(String::NewSymbol("dma_code"), Number::New(baton->record->dma_code));
       }

       if (baton->record->area_code > 0) {
       data->Set(String::NewSymbol("area_code"), Number::New(baton->record->area_code));
       }

       if (baton->record->continent_code > 0) {
       data->Set(String::NewSymbol("continent_code"), String::New(baton->record->continent_code));
       }*/

    argv[1] = data;
    argv[0] = Null();
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

Handle<Value> geoip::Test::close(const Arguments& args) {
  Test * c = ObjectWrap::Unwrap<geoip::Test>(args.This()); 
  GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
  c->db = NULL;
  Local<Object> instance = args.This();
  instance.Clear();
  HandleScope scope;	// Stick this down here since it seems to segfault when on top?
}
