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

Nan::Persistent<v8::Function> City6::constructor;

void City6::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("City6").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "lookupSync", lookupSync);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("City6").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(City6::New) {
  Nan::HandleScope scope;

  City6 *c = new City6();

  String::Utf8Value file_str(info[0]->ToString());
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
  Nan::HandleScope scope;

  Local<Object> data = Nan::New<Object>();
  City6 *c = ObjectWrap::Unwrap<City6>(info.This());

  //static Nan::Utf8String *host_cstr = new Nan::Utf8String(info[0]);
  Nan::Utf8String host_cstr(info[0]);
  //Nan::Utf8String host_cstr(info[0]);
  //geoipv6_t ipnum_v6 = _GeoIP_lookupaddress_v6(**host_cstr);
  geoipv6_t ipnum_v6 = _GeoIP_lookupaddress_v6(*host_cstr);

  if (__GEOIP_V6_IS_NULL(ipnum_v6)) {
    info.GetReturnValue().SetNull();
  }

  GeoIPRecord *record = GeoIP_record_by_ipnum_v6(c->db, ipnum_v6);

  if (!record) {
    info.GetReturnValue().SetNull();
    return;
  }

  if (record->country_code) {
    data->Set(Nan::New<String>("country_code").ToLocalChecked(),
      Nan::New<String>(record->country_code).ToLocalChecked());
  }

  if (record->country_code3) {
    data->Set(Nan::New<String>("country_code3").ToLocalChecked(),
      Nan::New<String>(record->country_code3).ToLocalChecked());
  }

  if (record->country_name) {
    data->Set(Nan::New<String>("country_name").ToLocalChecked(),
      Nan::New<String>(record->country_name).ToLocalChecked());
  }

  if (record->region) {
    data->Set(Nan::New<String>("region").ToLocalChecked(),
      Nan::New<String>(record->region).ToLocalChecked());
  }

  if (record->city) {
    char *name = _GeoIP_iso_8859_1__utf8(record->city);

    if (name) {
      data->Set(Nan::New<String>("city").ToLocalChecked(),
        Nan::New<String>(name).ToLocalChecked());
    }

    free(name);
  }

  if (record->postal_code) {
    data->Set(Nan::New<String>("postal_code").ToLocalChecked(),
      Nan::New<String>(record->postal_code).ToLocalChecked());
  }

  if (record->latitude >= -90 && record->latitude <= 90) {
    data->Set(Nan::New<String>("latitude").ToLocalChecked(),
      Nan::New<Number>(record->latitude));
  }

  if (record->longitude >= -180 && record->longitude <= 180) {
    data->Set(Nan::New<String>("longitude").ToLocalChecked(),
      Nan::New<Number>(record->longitude));
  }

  if (record->metro_code) {
    data->Set(Nan::New<String>("metro_code").ToLocalChecked(),
      Nan::New<Number>(record->metro_code));
  }

  if (record->dma_code) {
    data->Set(Nan::New<String>("dma_code").ToLocalChecked(),
      Nan::New<Number>(record->dma_code));
  }

  if (record->area_code) {
    data->Set(Nan::New<String>("area_code").ToLocalChecked(),
      Nan::New<Number>(record->area_code));
  }

  if (record->continent_code) {
    data->Set(Nan::New<String>("continent_code").ToLocalChecked(),
      Nan::New<String>(record->continent_code).ToLocalChecked());
  }

  if (record->country_code && record->region) {
    const char *time_zone = GeoIP_time_zone_by_country_and_region(record->country_code, record->region);

    if(time_zone) {
      data->Set(Nan::New<String>("time_zone").ToLocalChecked(),
        Nan::New<String>(time_zone).ToLocalChecked());
    }
  }

  GeoIPRecord_delete(record);
  info.GetReturnValue().Set(data);
}
