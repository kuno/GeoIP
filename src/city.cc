/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "city.h"
#include "global.h"

using namespace native;

City::City() : db(NULL) {};

City::~City() { if (db) {
  GeoIP_delete(db);
}
};

Persistent<FunctionTemplate> City::constructor_template;

void City::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNewLocal<FunctionTemplate>(FunctionTemplate::New(New));
  NanAssignPersistent(FunctionTemplate, constructor_template, tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(String::NewSymbol("City"));

  tpl->PrototypeTemplate()->Set(String::NewSymbol("lookupSync"),
      FunctionTemplate::New(lookupSync)->GetFunction());
  exports->Set(String::NewSymbol("City"), tpl->GetFunction());
}

NAN_METHOD(City::New) {
  NanScope();

  City *c = new City();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);
  bool cache_on = args[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (c->db) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_CITY_EDITION_REV0 ||
        c->db_edition == GEOIP_CITY_EDITION_REV1) {
      c->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(c->db);  // free()'s the reference & closes its fd
      return NanThrowError("Error: Not valid city database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(City::lookupSync) {
  NanScope();

  City *c = ObjectWrap::Unwrap<City>(args.This());

  Local<Object> data = NanNewLocal<Object>(Object::New());
  Local<String> host_str = NanNewLocal<String>(args[0]->ToString());

  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII,
          NULL, host_cstr, host_str->Length() + 1,
          v8::String::HINT_MANY_WRITES_EXPECTED);

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  //printf("Ip is %s.\n", host_cstr);
  //printf("Ipnum is %d.", ipnum);

  if (ipnum == 0) {
    NanReturnValue(Null());
  }

  GeoIPRecord *record = GeoIP_record_by_ipnum(c->db, ipnum);

  if (!record) {
    NanReturnValue(Null());
  }

  if (record->country_code) {
    data->Set(String::NewSymbol("country_code"), String::New(record->country_code));
  }

  if (record->country_code3) {
    data->Set(String::NewSymbol("country_code3"), String::New(record->country_code3));
  }

  if (record->country_name) {
    data->Set(String::NewSymbol("country_name"), String::New(record->country_name));
  }

  if (record->region) {
    data->Set(String::NewSymbol("region"), String::New(record->region));
  }

  if (record->city) {
    char *name = _GeoIP_iso_8859_1__utf8(record->city);

    if (name) {
      data->Set(String::NewSymbol("city"), String::New(name));
    }

    free(name);
  }

  if (record->postal_code) {
    data->Set(String::NewSymbol("postal_code"), String::New(record->postal_code));
  }

  if (record->latitude >= -90 && record->latitude <= 90) {
    data->Set(String::NewSymbol("latitude"), Number::New(record->latitude));
  }

  if (record->longitude >= -180 && record->longitude <= 180) {
    data->Set(String::NewSymbol("longitude"), Number::New(record->longitude));
  }

  if (record->metro_code) {
    data->Set(String::NewSymbol("metro_code"), Number::New(record->metro_code));
  }

  if (record->dma_code) {
    data->Set(String::NewSymbol("dma_code"), Number::New(record->dma_code));
  }

  if (record->area_code) {
    data->Set(String::NewSymbol("area_code"), Number::New(record->area_code));
  }

  if (record->continent_code) {
    data->Set(String::NewSymbol("continent_code"), String::New(record->continent_code));
  }

  if (record->country_code && record->region) {
    const char *time_zone = GeoIP_time_zone_by_country_and_region(record->country_code, record->region);

    if(time_zone) {
      data->Set(String::NewSymbol("time_zone"), String::New(time_zone));
    }
  }

  GeoIPRecord_delete(record);
  NanReturnValue(data);
}
