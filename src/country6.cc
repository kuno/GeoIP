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
  NanScope();

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  NanAssignPersistent(FunctionTemplate, constructor_template, t);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(t, "lookup", lookup);
  NODE_SET_PROTOTYPE_METHOD(t, "lookupSync", lookupSync);
  NODE_SET_PROTOTYPE_METHOD(t, "update", update);
  //NODE_SET_PROTOTYPE_METHOD(t, "close", close);
  target->Set(String::NewSymbol("Country6"), t->GetFunction());
}

geoip::Country6::Country6(): db(NULL) {};

geoip::Country6::~Country6() {
  if (db) {
    GeoIP_delete(db);
  }
};

NAN_METHOD(geoip::Country6::New)
{
  NanScope();

  Country6 *c = new Country6();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db != NULL) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION_V6) {
      c->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(c->db);	// free()'s the gi reference & closes its fd
      delete c->db;
      return NanThrowError("Error: Not valid country database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(geoip::Country6::lookupSync) {
  NanScope();

  Country6 * c = ObjectWrap::Unwrap<Country6>(args.This());

  // Check if database is country ipv6 edition
  if (c->db_edition != GEOIP_COUNTRY_EDITION_V6) {
    return NanThrowError("Error: Database is not country ipv6 edition");
  }

  Local<Object> data = Object::New();
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  geoipv6_t ipnum_v6 = _GeoIP_lookupaddress_v6(host_cstr);

  if (__GEOIP_V6_IS_NULL(ipnum_v6)) {
    NanReturnValue(Null());
  } else {
    int country_id = GeoIP_id_by_ipnum_v6(c->db, ipnum_v6);
    if (country_id == 0) {
      NanReturnValue(Null());
    } else {
      char * name = _GeoIP_iso_8859_1__utf8(GeoIP_country_name[country_id]);

      data->Set(String::NewSymbol("country_name"), String::New(name));
      data->Set(String::NewSymbol("country_code"), String::New(GeoIP_country_code[country_id]));
      data->Set(String::NewSymbol("country_code3"), String::New(GeoIP_country_code3[country_id]));
      data->Set(String::NewSymbol("continent_code"), String::New(GeoIP_country_continent[country_id]));

      free(name);

      NanReturnValue(data);
    }
  }
}

NAN_METHOD(geoip::Country6::lookup)
{
  NanScope();

  REQ_FUN_ARG(1, cb);

  Country6 * c = ObjectWrap::Unwrap<Country6>(args.This());

  // Check if database is country ipv6 edition
  if (c->db_edition != GEOIP_COUNTRY_EDITION_V6) {
    return NanThrowError("Error: Database is not country ipv6 edition");
  }

  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  country6_baton_t *baton = new country6_baton_t();
  baton->c = c;
  baton->ipnum_v6 = _GeoIP_lookupaddress_v6(host_cstr);
  NanAssignPersistent(Function, baton->cb, cb);

  uv_work_t *req = new uv_work_t;
  req->data = baton;

  uv_queue_work(uv_default_loop(), req, EIO_Country, (uv_after_work_cb)EIO_AfterCountry);

  NanReturnUndefined();
}

void geoip::Country6::EIO_Country(uv_work_t *req)
{
  //Locker locker();

  country6_baton_t *baton = static_cast<country6_baton_t *>(req->data);

  if (__GEOIP_V6_IS_NULL(baton->ipnum_v6)) {
    baton->country_id = 0;
  } else {
    baton->country_id = GeoIP_id_by_ipnum_v6(baton->c->db, baton->ipnum_v6);
  }

  //Unlocker unlocker();
}

void geoip::Country6::EIO_AfterCountry(uv_work_t *req)
{
  NanScope();

  country6_baton_t *baton = static_cast<country6_baton_t *>(req->data);

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
  NanPersistentToLocal(baton->cb)->Call(Context::GetCurrent()->Global(), 2, argv);

  // Cleanup
  NanDispose(baton->cb);
  delete baton;
  delete req;

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }
}

NAN_METHOD(geoip::Country6::update) {
  NanLocker();

  NanScope();

  Country6* c = ObjectWrap::Unwrap<Country6>(args.This());

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db != NULL) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION_V6) {
      NanReturnValue(True());
    } else {
      GeoIP_delete(c->db);  // free()'s the gi reference & closes its fd
      delete c->db;
      return NanThrowError("Error: Not valid country database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }

 NanUnlocker();
}

NAN_METHOD(geoip::Country6::close) {
  Country6 * c = ObjectWrap::Unwrap<Country6>(args.This());
  GeoIP_delete(c->db);  // free()'s the gi reference & closes its fd
  delete c->db;
}
