/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "city6.h"
#include "global.h"

using namespace native;


City6::City6() : db(NULL) {};

City6::~City6() {
  if (db) {
    GeoIP_delete(db);
  }
};

Nan::Persistent<FunctionTemplate> City6::constructor_template;

NAN_MODULE_INIT(City6::Init) {
  Nan::HandleScope scope;

  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  constructor_template.Reset(tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(Nan::New<String>("City6").ToLocalChecked());

  Nan::SetPrototypeTemplate(tpl, "lookupSync",
      Nan::GetFunction(Nan::New<FunctionTemplate>(lookupSync)).ToLocalChecked());
  Nan::Set(target, Nan::New<String>("City6").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(City6::New) {

  City6 *c = new City6();

  Nan::Utf8String file_str(info[0]->ToString());
  const char * file_cstr = ToCString(file_str);
  bool cache_on = info[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (c->db) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_CITY_EDITION_REV0_V6 ||
        c->db_edition == GEOIP_CITY_EDITION_REV1_V6) {
      c->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      GeoIP_delete(c->db);  // free()'s the reference & closes fd
      return Nan::ThrowError("Error: Not valid city ipv6 database");
    }
  } else {
    return Nan::ThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(City6::lookupSync) {

  Local<Object> data = Nan::New<Object>();
  City6 *c = Nan::ObjectWrap::Unwrap<City6>(info.This());

  Nan::Utf8String host_cstr(info[0]);
  geoipv6_t ipnum_v6 = _GeoIP_lookupaddress_v6(*host_cstr);

  if (__GEOIP_V6_IS_NULL(ipnum_v6)) {
    info.GetReturnValue().SetNull();
  }

  GeoIPRecord *record = GeoIP_record_by_ipnum_v6(c->db, ipnum_v6);

  if (!record) {
    info.GetReturnValue().Set(Nan::Null());
  }

  if (record->country_code) {
    Nan::Set(data, Nan::New<String>("country_code").ToLocalChecked(), Nan::New<String>(record->country_code).ToLocalChecked());
  }

  if (record->country_code3) {
    Nan::Set(data, Nan::New<String>("country_code3").ToLocalChecked(), Nan::New<String>(record->country_code3).ToLocalChecked());
  }

  if (record->country_name) {
    Nan::Set(data, Nan::New<String>("country_name").ToLocalChecked(), Nan::New<String>(record->country_name).ToLocalChecked());
  }

  if (record->region) {
    Nan::Set(data, Nan::New<String>("region").ToLocalChecked(), Nan::New<String>(record->region).ToLocalChecked());
  }

  if (record->city) {
    char *name = _GeoIP_iso_8859_1__utf8(record->city);

    if (name) {
      Nan::Set(data, Nan::New<String>("city").ToLocalChecked(), Nan::New<String>(name).ToLocalChecked());
    }

    free(name);
  }

  if (record->postal_code) {
    Nan::Set(data, Nan::New<String>("postal_code").ToLocalChecked(), Nan::New<String>(record->postal_code).ToLocalChecked());
  }

  if (record->latitude >= -90 && record->latitude <= 90) {
    Nan::Set(data, Nan::New<String>("latitude").ToLocalChecked(), Nan::New<Number>(record->latitude));
  }

  if (record->longitude >= -180 && record->longitude <= 180) {
    Nan::Set(data, Nan::New<String>("longitude").ToLocalChecked(), Nan::New<Number>(record->longitude));
  }

  if (record->metro_code) {
    Nan::Set(data, Nan::New<String>("metro_code").ToLocalChecked(), Nan::New<Number>(record->metro_code));
  }

  if (record->dma_code) {
    Nan::Set(data, Nan::New<String>("dma_code").ToLocalChecked(), Nan::New<Number>(record->dma_code));
  }

  if (record->area_code) {
    Nan::Set(data, Nan::New<String>("area_code").ToLocalChecked(), Nan::New<Number>(record->area_code));
  }

  if (record->continent_code) {
    Nan::Set(data, Nan::New<String>("continent_code").ToLocalChecked(), Nan::New<String>(record->continent_code).ToLocalChecked());
  }

  if (record->country_code && record->region) {
    const char *time_zone = GeoIP_time_zone_by_country_and_region(record->country_code, record->region);

    if(time_zone) {
      Nan::Set(data, Nan::New<String>("time_zone").ToLocalChecked(), Nan::New<String>(time_zone).ToLocalChecked());
    }
  }

  GeoIPRecord_delete(record);
  info.GetReturnValue().Set(data);
}
