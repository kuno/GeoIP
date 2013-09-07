/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "city6.h"
#include "global.h"

Persistent<FunctionTemplate> geoip::City6::constructor_template;

void geoip::City6::Init(Handle<Object> target)
{
  NanScope();

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  NanAssignPersistent(FunctionTemplate, constructor_template, t);
  t->InstanceTemplate()->SetInternalFieldCount(2);
  t->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(t, "lookup", lookup);
  NODE_SET_PROTOTYPE_METHOD(t, "lookupSync", lookupSync);
  NODE_SET_PROTOTYPE_METHOD(t, "update", update);
  //NODE_SET_PROTOTYPE_METHOD(t, "close", close);
  target->Set(String::NewSymbol("City6"), t->GetFunction());
}

geoip::City6::City6() : db(NULL) {};

geoip::City6::~City6() {
  if (db) {
    GeoIP_delete(db);
  }
};

NAN_METHOD(geoip::City6::New)
{
  NanScope();

  City6 *c = new City6();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  //Local<String> file_str = args[0]->ToString();
  //char file_cstr[file_str->Length()];
  //file_str->WriteAscii(file_cstr);
  bool cache_on = args[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (c->db != NULL) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_CITY_EDITION_REV0_V6 ||
        c->db_edition == GEOIP_CITY_EDITION_REV1_V6) {
      c->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(c->db);  // free()'s the gi reference & closes its fd
      return NanThrowError("Error: Not valid city ipv6 database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(geoip::City6::lookupSync) {
  NanScope();

  Local<Object> data = Object::New();
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);
  City6 * c = ObjectWrap::Unwrap<geoip::City6>(args.This());

  geoipv6_t ipnum_v6 = _GeoIP_lookupaddress_v6(host_cstr);

  if (__GEOIP_V6_IS_NULL(ipnum_v6)) {
    NanReturnValue(Null());
  }

  GeoIPRecord *record = GeoIP_record_by_ipnum_v6(c->db, ipnum_v6);

  if (record == NULL) {
    NanReturnValue(Null()); //return ThrowException(String::New("Error: Can not find match data"));
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

  if (record->city != NULL) {
    char *name = _GeoIP_iso_8859_1__utf8(record->city);

    if (name) {
      data->Set(String::NewSymbol("city"), String::New(name));
    }

    free(name);
  }

  if (record->postal_code != NULL) {
    data->Set(String::NewSymbol("postal_code"), String::New(record->postal_code));
  }

  if (record->latitude >= -90 && record->latitude <= 90) {
    data->Set(String::NewSymbol("latitude"), Number::New(record->latitude));
  }

  if (record->longitude >= -180 && record->longitude <= 180) {
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

  if (record->country_code != NULL && record->region != NULL) {
    const char *time_zone = GeoIP_time_zone_by_country_and_region(record->country_code, record->region);
    if(time_zone != NULL) {
      data->Set(String::NewSymbol("time_zone"), String::New(time_zone));
    }
  }

  GeoIPRecord_delete(record);
  NanReturnValue(data);
}

NAN_METHOD(geoip::City6::lookup)
{
  NanScope();

  REQ_FUN_ARG(1, cb);

  City6 *c = ObjectWrap::Unwrap<geoip::City6>(args.This());
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  city6_baton_t *baton = new city6_baton_t();
  baton->c = c;
  baton->ipnum_v6 = _GeoIP_lookupaddress_v6(host_cstr);
  NanAssignPersistent(Function, baton->cb, cb);

  uv_work_t *req = new uv_work_t;
  req->data = baton;

  uv_queue_work(uv_default_loop(), req, EIO_City, (uv_after_work_cb)EIO_AfterCity);

  NanReturnUndefined();
}

void geoip::City6::EIO_City(uv_work_t *req)
{
  city6_baton_t *baton = static_cast<city6_baton_t *>(req->data);

  if (__GEOIP_V6_IS_NULL(baton->ipnum_v6)) {
    baton->record = NULL;
  } else {
    baton->record = GeoIP_record_by_ipnum_v6(baton->c->db, baton->ipnum_v6);
  }
}

void geoip::City6::EIO_AfterCity(uv_work_t *req)
{
  NanScope();

  city6_baton_t *baton = static_cast<city6_baton_t *>(req->data);

  Handle<Value> argv[2];

  if (baton->record == NULL) {
    argv[0] = Exception::Error(String::New("Data not found"));
    argv[1] = Null();
  } else {
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

    if (baton->record->city != NULL) {
      char *name = _GeoIP_iso_8859_1__utf8(baton->record->city);

      if (name) {
        data->Set(String::NewSymbol("city"), String::New(name));
      }

      free(name);
    }

    if (baton->record->postal_code != NULL) {
      data->Set(String::NewSymbol("postal_code"), String::New(baton->record->postal_code));
    }

    if (baton->record->latitude >= -90 && baton->record->latitude <= 90) {
      data->Set(String::NewSymbol("latitude"), Number::New(baton->record->latitude));
    }

    if (baton->record->longitude >= -180 && baton->record->longitude <= 180) {
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
    }

    if (baton->record->country_code != NULL && baton->record->region != NULL) {
      const char *time_zone = GeoIP_time_zone_by_country_and_region(baton->record->country_code, baton->record->region);
      if(time_zone != NULL) {
        data->Set(String::NewSymbol("time_zone"), String::New(time_zone));
      }
    }

    argv[0] = Null();
    argv[1] = data;
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

NAN_METHOD(geoip::City6::update) {
  NanLocker();

  NanScope();

  City6 *c = ObjectWrap::Unwrap<City6>(args.This());

  String::Utf8Value file_str(args[0]->ToString());
  const char *file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (c->db != NULL) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_CITY_EDITION_REV0_V6 ||
        c->db_edition == GEOIP_CITY_EDITION_REV1_V6) {
      NanReturnValue(True());
    } else {
      GeoIP_delete(c->db);  // free()'s the gi reference & closes its fd
      NanReturnValue(ThrowException(String::New("Error: Not valid city ipv6 database")));
    }
  } else {
    NanReturnValue(ThrowException(String::New("Error: Cannot open database")));
  }

  NanUnlocker();
}

NAN_METHOD(geoip::City6::close) {
  City6 * c = ObjectWrap::Unwrap<geoip::City6>(args.This());
  GeoIP_delete(c->db);  // free()'s the gi reference & closes its fd
}
