/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "netspeedcell.h"
#include "global.h"

Persistent<FunctionTemplate> native::NetSpeedCell::constructor_template;

void native::NetSpeedCell::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  NanAssignPersistent(FunctionTemplate, constructor_template, t);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(String::NewSymbol("NetSpeedCell"));

  NODE_SET_PROTOTYPE_METHOD(t, "lookupSync", lookupSync);
  target->Set(String::NewSymbol("NetSpeedCell"), t->GetFunction());
}

native::NetSpeedCell::NetSpeedCell() : db(NULL) {};

native::NetSpeedCell::~NetSpeedCell() {
  if (db) {
    GeoIP_delete(db);
  }
};

NAN_METHOD(native::NetSpeedCell::New) {
  NanScope();

  NetSpeedCell *n = new NetSpeedCell();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  //Local<String> file_str = args[0]->ToString();
  //char file_cstr[file_str->Length()];
  //file_str->WriteAscii(file_cstr);
  bool cache_on = args[1]->ToBoolean()->Value();

  n->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (n->db != NULL) {
    n->db_edition = GeoIP_database_edition(n->db);
    if (n->db_edition == GEOIP_NETSPEED_EDITION_REV1) {
      n->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(n->db);  // free()'s the gi reference & closes its fd
      return NanThrowError("Error: Not valid netspeed cell database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(native::NetSpeedCell::lookupSync) {
  NanScope();

  NetSpeedCell *n = ObjectWrap::Unwrap<NetSpeedCell>(args.This());

  Local<String> data;
  Local<String> host_str = NanNewLocal<String>(args[0]->ToString());
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  char* speed = GeoIP_name_by_addr(n->db, host_cstr);
  if (speed == NULL) {
    data = String::New("Unknown");
  }
  else {
    data = String::New(speed);
  }

  NanReturnValue(data);
}
