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

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor_template, tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(NanSymbol("Org"));

  tpl->PrototypeTemplate()->Set(NanSymbol("lookupSync"),
      NanNew<FunctionTemplate>(lookupSync)->GetFunction());
  exports->Set(NanSymbol("Org"), tpl->GetFunction());
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

  Local<Value> data = NanNew(NanNull());
  Local<String> host_str = args[0]->ToString();
  size_t size = host_str->Length() + 1;
  char host_cstr[size];
  size_t bc;
  NanCString(args[0], &bc, host_cstr, size);
  Org *o = ObjectWrap::Unwrap<Org>(args.This());

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  if (ipnum <= 0) {
    NanReturnValue(NanNull());
  }

  char *org = GeoIP_org_by_ipnum(o->db, ipnum);
  if (!org) {
    NanReturnValue(NanNull());
  }

  char *name = _GeoIP_iso_8859_1__utf8(org);

  data = NanNew<String>(name);

  free(org);
  free(name);

  NanReturnValue(data);
}
