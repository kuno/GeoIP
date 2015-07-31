
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

Nan::Persistent<FunctionTemplate> Region::constructor_template;

NAN_MODULE_INIT(Region::Init) {
  Nan::HandleScope scope;

  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  constructor_template.Reset(tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(Nan::New<String>("Region").ToLocalChecked());

  Nan::SetPrototypeTemplate(tpl, "lookupSync",
      Nan::GetFunction(Nan::New<FunctionTemplate>(lookupSync)).ToLocalChecked());
  Nan::Set(target, Nan::New<String>("Region").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(Region::New) {
  Region *r = new Region();

  Nan::Utf8String file_str(info[0]->ToString());
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

  Local<Object> data = Nan::New<Object>();
  Region *r = Nan::ObjectWrap::Unwrap<Region>(info.This());

  static Nan::Utf8String *host_cstr = new Nan::Utf8String(info[0]);
  uint32_t ipnum = _GeoIP_lookupaddress(**host_cstr);

  if (ipnum <= 0) {
    info.GetReturnValue().Set(Nan::Null());
  }

  GeoIPRegion *region = GeoIP_region_by_ipnum(r->db, ipnum);

  if (region) {
    Nan::Set(data, Nan::New<String>("country_code").ToLocalChecked(), Nan::New<String>(region->country_code).ToLocalChecked());
    Nan::Set(data, Nan::New<String>("region").ToLocalChecked(), Nan::New<String>(region->region).ToLocalChecked());
    GeoIPRegion_delete(region);
    info.GetReturnValue().Set(data);
  } else {
    //GeoIPRegion_delete(region);
    return Nan::ThrowError("Error: Can not find match data");
  }
}
