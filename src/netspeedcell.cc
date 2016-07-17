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

Nan::Persistent<v8::Function> NetSpeedCell::constructor;

void NetSpeedCell::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("NetSpeedCell").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "lookupSync", lookupSync);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("NetSpeedCell").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(NetSpeedCell::New) {
  Nan::HandleScope scope;

  NetSpeedCell *n = new NetSpeedCell();

  String::Utf8Value file_str(info[0]->ToString());
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
  Nan::HandleScope scope;

  NetSpeedCell *n = ObjectWrap::Unwrap<NetSpeedCell>(info.This());

  //static Nan::Utf8String *host_cstr = new Nan::Utf8String(info[0]);
  Nan::Utf8String host_cstr(info[0]);
  //char *speed = GeoIP_name_by_addr(n->db, **host_cstr);
  char *speed = GeoIP_name_by_addr(n->db, *host_cstr);

  if (!speed) {
    info.GetReturnValue().Set(Nan::New<String>("Unknown").ToLocalChecked());
  } else {
    info.GetReturnValue().Set(Nan::New<String>(speed).ToLocalChecked());
  }
}
