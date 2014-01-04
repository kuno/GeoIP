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

Persistent<FunctionTemplate> NetSpeedCell::constructor_template;

void NetSpeedCell::Init(Handle<Object> exports) {
 NanScope();

  Local<FunctionTemplate> tpl = NanNewLocal<FunctionTemplate>(FunctionTemplate::New(New));
  NanAssignPersistent(FunctionTemplate, constructor_template, tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(String::NewSymbol("NetSpeedCell"));

  tpl->PrototypeTemplate()->Set(String::NewSymbol("lookupSync"),
      FunctionTemplate::New(lookupSync)->GetFunction());
  exports->Set(String::NewSymbol("NetSpeedCell"), tpl->GetFunction());
}

NAN_METHOD(NetSpeedCell::New) {
  NanScope();

  NetSpeedCell *n = new NetSpeedCell();

  String::Utf8Value file_str(args[0]->ToString());
  const char *file_cstr = ToCString(file_str);
  bool cache_on = args[1]->ToBoolean()->Value();

  n->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (n->db) {
    n->db_edition = GeoIP_database_edition(n->db);
    if (n->db_edition == GEOIP_NETSPEED_EDITION_REV1) {
      n->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(n->db);  // free()'s the reference & closes fd
      return NanThrowError("Error: Not valid netspeed cell database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(NetSpeedCell::lookupSync) {
  NanScope();

  NetSpeedCell *n = ObjectWrap::Unwrap<NetSpeedCell>(args.This());

  Local<Value> data = NanNewLocal<Value>(Null());
  Local<String> host_str = NanNewLocal<String>(args[0]->ToString());
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  char *speed = GeoIP_name_by_addr(n->db, host_cstr);
  if (!speed) {
    data = String::New("Unknown");
  }
  else {
    data = String::New(speed);
  }

  NanReturnValue(data);
}
