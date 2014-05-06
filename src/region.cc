
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

Persistent<FunctionTemplate> Region::constructor_template;

void Region::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor_template, tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(NanSymbol("Region"));

  tpl->PrototypeTemplate()->Set(NanSymbol("lookupSync"),
      NanNew<FunctionTemplate>(lookupSync)->GetFunction());
  exports->Set(NanSymbol("Region"), tpl->GetFunction());
}

NAN_METHOD(Region::New) {
  NanScope();
  Region *r = new Region();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);
  bool cache_on = args[1]->ToBoolean()->Value();

  r->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (r->db) {
    // Successfully opened the file, return 1 (true)
    r->db_edition = GeoIP_database_edition(r->db);
    if (r->db_edition == GEOIP_REGION_EDITION_REV0 ||
        r->db_edition == GEOIP_REGION_EDITION_REV1) {
      r->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(r->db);  // free()'s the reference & closes fd
      //printf("edition is %d", r->db_edition);
      return NanThrowError("Error: Not valid region database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(Region::lookupSync) {
  NanScope();

  Local<Object> data = NanNew<Object>();
  Local<String> host_str = args[0]->ToString();
  size_t size = host_str->Length() + 1;
  char host_cstr[size];
  size_t bc;
  NanCString(args[0], &bc, host_cstr, size);
  Region *r = ObjectWrap::Unwrap<Region>(args.This());

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  if (ipnum <= 0) {
    NanReturnValue(NanNull());
  }

  GeoIPRegion *region = GeoIP_region_by_ipnum(r->db, ipnum);

  if (region) {
    data->Set(NanSymbol("country_code"), NanNew<String>(region->country_code));
    data->Set(NanSymbol("region"), NanNew<String>(region->region));
    GeoIPRegion_delete(region);
    NanReturnValue(data);
  } else {
    //GeoIPRegion_delete(region);
    return NanThrowError("Error: Can not find match data");
  }
}
