/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "netspeedcell.h"
#include "global.h"

using namespace native;

NetSpeedCell::NetSpeedCell() : db(NULL) {};

NetSpeedCell::~NetSpeedCell() {
  if (db) {
    GeoIP_delete(db);
  }
};

Nan::Persistent<FunctionTemplate> NetSpeedCell::constructor_template;

NAN_MODULE_INIT(NetSpeedCell::Init) {
 Nan::HandleScope scope;

  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  constructor_template.Reset(tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(Nan::New<String>("NetSpeedCell").ToLocalChecked());

  Nan::SetPrototypeTemplate(tpl, "lookupSync",
      Nan::GetFunction(Nan::New<FunctionTemplate>(lookupSync)).ToLocalChecked());
  Nan::Set(target, Nan::New<String>("NetSpeedCell").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NetSpeedCell::New) {
  NetSpeedCell *n = new NetSpeedCell();

  Nan::Utf8String file_str(info[0]->ToString());
  const char *file_cstr = ToCString(file_str);
  bool cache_on = info[1]->ToBoolean()->Value();

  n->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (n->db) {
    n->db_edition = GeoIP_database_edition(n->db);
    if (n->db_edition == GEOIP_NETSPEED_EDITION_REV1) {
      n->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      GeoIP_delete(n->db);  // free()'s the reference & closes fd
      return Nan::ThrowError("Error: Not valid netspeed cell database");
    }
  } else {
    return Nan::ThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(NetSpeedCell::lookupSync) {
  NetSpeedCell *n = Nan::ObjectWrap::Unwrap<NetSpeedCell>(info.This());

  Local<Value> data = Nan::New(Nan::Null());

  static Nan::Utf8String *host_cstr = new Nan::Utf8String(info[0]);

  char *speed = GeoIP_name_by_addr(n->db, **host_cstr);

  if (!speed) {
    data = Nan::New<String>("Unknown").ToLocalChecked();
  } else {
    data = Nan::New<String>(speed).ToLocalChecked();
  }

  info.GetReturnValue().Set(data);
}
