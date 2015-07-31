/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "org.h"
#include "global.h"

using namespace native;

Org::Org() {};

Org::~Org() { if (db) {
  GeoIP_delete(db);
}
};

Nan::Persistent<FunctionTemplate> Org::constructor_template;

NAN_MODULE_INIT(Org::Init) {
  Nan::HandleScope scope;

  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  constructor_template.Reset(tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(Nan::New<String>("Org").ToLocalChecked());

  Nan::SetPrototypeTemplate(tpl, "lookupSync",
      Nan::GetFunction(Nan::New<FunctionTemplate>(lookupSync)).ToLocalChecked());
  Nan::Set(target, Nan::New<String>("Org").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(Org::New) {

  Org *o = new Org();

  Nan::Utf8String file_str(info[0]->ToString());
  const char *file_cstr = ToCString(file_str);
  bool cache_on = info[1]->ToBoolean()->Value();

  o->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (o->db) {
    // Successfully opened the file, return 1 (true)
    o->db_edition = GeoIP_database_edition(o->db);
    if (o->db_edition == GEOIP_ORG_EDITION ||
        o->db_edition == GEOIP_ASNUM_EDITION ||
        o->db_edition == GEOIP_ISP_EDITION) {
      o->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      GeoIP_delete(o->db);  // free()'s the reference & closes fd
      return Nan::ThrowError("Error: Not valid org database");
    }
  } else {
    return Nan::ThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(Org::lookupSync) {

  Local<Value> data = Nan::New(Nan::Null());
  Org *o = Nan::ObjectWrap::Unwrap<Org>(info.This());

  Nan::Utf8String host_cstr(info[0]);
  uint32_t ipnum = _GeoIP_lookupaddress(*host_cstr);

  if (ipnum <= 0) {
    info.GetReturnValue().SetNull();
  }

  char *org = GeoIP_org_by_ipnum(o->db, ipnum);
  if (!org) {
    info.GetReturnValue().Set(Nan::Null());
  }

  char *name = _GeoIP_iso_8859_1__utf8(org);

  data = Nan::New<String>(name).ToLocalChecked();

  free(org);
  free(name);

  info.GetReturnValue().Set(data);
}
