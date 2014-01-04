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

Persistent<FunctionTemplate> Org::constructor_template;

void Org::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNewLocal<FunctionTemplate>(FunctionTemplate::New(New));
  NanAssignPersistent(FunctionTemplate, constructor_template, tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(String::NewSymbol("Org"));

  tpl->PrototypeTemplate()->Set(String::NewSymbol("lookupSync"),
      FunctionTemplate::New(lookupSync)->GetFunction());
  exports->Set(String::NewSymbol("Org"), tpl->GetFunction());
}

NAN_METHOD(Org::New) {
  NanScope();

  Org *o = new Org();

  String::Utf8Value file_str(args[0]->ToString());
  const char *file_cstr = ToCString(file_str);
  bool cache_on = args[1]->ToBoolean()->Value();

  o->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (o->db) {
    // Successfully opened the file, return 1 (true)
    o->db_edition = GeoIP_database_edition(o->db);
    if (o->db_edition == GEOIP_ORG_EDITION ||
        o->db_edition == GEOIP_ASNUM_EDITION ||
        o->db_edition == GEOIP_ISP_EDITION) {
      o->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(o->db);  // free()'s the reference & closes fd
      return NanThrowError("Error: Not valid org database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(Org::lookupSync) {
  NanScope();

  Local<Value> data = NanNewLocal<Value>(Null());
  Local<String> host_str = NanNewLocal<String>(args[0]->ToString());
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);
  Org *o = ObjectWrap::Unwrap<Org>(args.This());

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  if (ipnum <= 0) {
    NanReturnValue(Null());
  }

  char *org = GeoIP_org_by_ipnum(o->db, ipnum);
  if (!org) {
    NanReturnValue(Null());
  }

  char *name = _GeoIP_iso_8859_1__utf8(org);

  data = String::New(name);

  free(org);
  free(name);

  NanReturnValue(data);
}
