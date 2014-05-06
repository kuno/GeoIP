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

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor_template, tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(NanSymbol("City"));

  tpl->PrototypeTemplate()->Set(NanSymbol("lookupSync"),
      NanNew<FunctionTemplate>(lookupSync)->GetFunction());
  exports->Set(NanSymbol("City"), tpl->GetFunction());
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

  Local<Object> data = NanNew<Object>();
  Local<String> host_str = args[0]->ToString();
  size_t size = host_str->Length() + 1;
  char host_cstr[size];
  size_t bc;
  NanCString(args[0], &bc, host_cstr, size);

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  //printf("Ip is %s.\n", host_cstr);
  //printf("Ipnum is %d.", ipnum);

  if (ipnum == 0) {
    NanReturnValue(NanNull());
  }

  GeoIPRecord *record = GeoIP_record_by_ipnum(c->db, ipnum);

  if (!record) {
    NanReturnValue(NanNull());
  }

  if (record->country_code) {
    data->Set(NanSymbol("country_code"), NanNew<String>(record->country_code));
  }

  if (record->country_code3) {
    data->Set(NanSymbol("country_code3"), NanNew<String>(record->country_code3));
  }

  if (record->country_name) {
    data->Set(NanSymbol("country_name"), NanNew<String>(record->country_name));
  }

  if (record->region) {
    data->Set(NanSymbol("region"), NanNew<String>(record->region));
  }

  if (record->city) {
    char *name = _GeoIP_iso_8859_1__utf8(record->city);

    if (name) {
      data->Set(NanSymbol("city"), NanNew<String>(name));
    }

    free(name);
  }

  if (record->postal_code) {
    data->Set(NanSymbol("postal_code"), NanNew<String>(record->postal_code));
  }

  if (record->latitude >= -90 && record->latitude <= 90) {
    data->Set(NanSymbol("latitude"), NanNew<Number>(record->latitude));
  }

  if (record->longitude >= -180 && record->longitude <= 180) {
    data->Set(NanSymbol("longitude"), NanNew<Number>(record->longitude));
  }

  if (record->metro_code) {
    data->Set(NanSymbol("metro_code"), NanNew<Number>(record->metro_code));
  }

  if (record->dma_code) {
    data->Set(NanSymbol("dma_code"), NanNew<Number>(record->dma_code));
  }

  if (record->area_code) {
    data->Set(NanSymbol("area_code"), NanNew<Number>(record->area_code));
  }

  if (record->continent_code) {
    data->Set(NanSymbol("continent_code"), NanNew<String>(record->continent_code));
  }

  if (record->country_code && record->region) {
    const char *time_zone = GeoIP_time_zone_by_country_and_region(record->country_code, record->region);

    if(time_zone) {
      data->Set(NanSymbol("time_zone"), NanNew<String>(time_zone));
    }
  }

  GeoIPRecord_delete(record);
  NanReturnValue(data);
}
