
/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "region.h"
#include "global.h"

using namespace native;

Region::Region() : db(NULL) {};

Region::~Region() {
  if (db) {
    GeoIP_delete(db);
  }
};

Nan::Persistent<v8::Function> Region::constructor;

void Region::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Region").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "lookupSync", lookupSync);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Region").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(Region::New) {
  Nan::HandleScope scope;
  Region *r = new Region();

  String::Utf8Value file_str(info[0]->ToString());
  const char * file_cstr = ToCString(file_str);
  bool cache_on = info[1]->ToBoolean()->Value();

  r->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (r->db) {
    // Successfully opened the file, return 1 (true)
    r->db_edition = GeoIP_database_edition(r->db);
    if (r->db_edition == GEOIP_REGION_EDITION_REV0 ||
        r->db_edition == GEOIP_REGION_EDITION_REV1) {
      r->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      GeoIP_delete(r->db);  // free()'s the reference & closes fd
      //printf("edition is %d", r->db_edition);
      return Nan::ThrowError("Error: Not valid region database");
    }
  } else {
    return Nan::ThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(Region::lookupSync) {
  Nan::HandleScope scope;

  Local<Object> data = Nan::New<Object>();
  Region *r = ObjectWrap::Unwrap<Region>(info.This());

  //static Nan::Utf8String *host_cstr = new Nan::Utf8String(info[0]);
  Nan::Utf8String host_cstr(info[0]);
  //uint32_t ipnum = _GeoIP_lookupaddress(**host_cstr);
  uint32_t ipnum = _GeoIP_lookupaddress(*host_cstr);

  if (ipnum <= 0) {
    info.GetReturnValue().SetNull();
  }

  GeoIPRegion *region = GeoIP_region_by_ipnum(r->db, ipnum);

  if (region) {
    data->Set(Nan::New<String>("country_code").ToLocalChecked(),
      Nan::New<String>(region->country_code).ToLocalChecked());
    data->Set(Nan::New<String>("region").ToLocalChecked(),
      Nan::New<String>(region->region).ToLocalChecked());
    GeoIPRegion_delete(region);
    info.GetReturnValue().Set(data);
  } else {
    //GeoIPRegion_delete(region);
    return Nan::ThrowError("Error: Can not find match data");
  }
}
