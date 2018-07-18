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

Nan::Persistent<v8::Function> Org::constructor;

void Org::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Org").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(Nan::New("lookupSync").ToLocalChecked(),
                                Nan::New<v8::FunctionTemplate>(lookupSync));

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Org").ToLocalChecked(), tpl->GetFunction());;;
}

NAN_METHOD(Org::New) {
  Nan::HandleScope scope;

  Org *o = new Org();

  const char * file_cstr = *Nan::Utf8String(info[0]->ToString());
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
  Nan::HandleScope scope;

  Org *o = ObjectWrap::Unwrap<Org>(info.This());

  Nan::Utf8String host_cstr(info[0]);

  uint32_t ipnum = _GeoIP_lookupaddress(*host_cstr);

  if (ipnum <= 0) {
    info.GetReturnValue().SetNull();
    return;
  }

  char *org = GeoIP_org_by_ipnum(o->db, ipnum);
  if (!org) {
    info.GetReturnValue().SetNull();
    return;
  }

  char *name = _GeoIP_iso_8859_1__utf8(org);

  Local<Value> data = Nan::New<String>(name).ToLocalChecked();

  free(org);
  free(name);

  info.GetReturnValue().Set(data);
}
