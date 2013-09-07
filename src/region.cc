
/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "region.h"
#include "global.h"

Persistent<FunctionTemplate> geoip::Region::constructor_template;

void geoip::Region::Init(Handle<Object> target)
{
  NanScope();

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  NanAssignPersistent(FunctionTemplate, constructor_template, t);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(t, "lookup", lookup);
  NODE_SET_PROTOTYPE_METHOD(t, "lookupSync", lookupSync);
  NODE_SET_PROTOTYPE_METHOD(t, "update", update);
  //NODE_SET_PROTOTYPE_METHOD(t, "close", close);
  target->Set(String::NewSymbol("Region"), t->GetFunction());
}

geoip::Region::Region() : db(NULL) {};

geoip::Region::~Region() {
  if (db) {
    GeoIP_delete(db);
  }
};

NAN_METHOD(geoip::Region::New)
{
  NanScope();
  Region *r = new Region();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);
  bool cache_on = args[1]->ToBoolean()->Value();

  r->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (r->db != NULL) {
    // Successfully opened the file, return 1 (true)
    r->db_edition = GeoIP_database_edition(r->db);
    if (r->db_edition == GEOIP_REGION_EDITION_REV0 ||
        r->db_edition == GEOIP_REGION_EDITION_REV1) {
      r->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(r->db);  // free()'s the gi reference & closes its fd
      //printf("edition is %d", r->db_edition);
      return NanThrowError("Error: Not valid region database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(geoip::Region::lookupSync) {
  NanScope();

  Local<Object> data = Object::New();
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);
  Region* r = ObjectWrap::Unwrap<Region>(args.This());

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  if (ipnum <= 0) {
    NanReturnValue(Null());
  }

  GeoIPRegion *region = GeoIP_region_by_ipnum(r->db, ipnum);

  if (region != NULL) {
    data->Set(String::NewSymbol("country_code"), String::New(region->country_code));
    data->Set(String::NewSymbol("region"), String::New(region->region));
    GeoIPRegion_delete(region);
    NanReturnValue(data);
  } else {
    //GeoIPRegion_delete(region);
    return NanThrowError("Error: Can not find match data");
  }
}

NAN_METHOD(geoip::Region::lookup)
{
  NanScope();

  REQ_FUN_ARG(1, cb);

  Region *r = ObjectWrap::Unwrap<Region>(args.This());
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  region_baton_t *baton = new region_baton_t();
  baton->r = r;
  baton->ipnum = _GeoIP_lookupaddress(host_cstr);
  NanAssignPersistent(Function, baton->cb, cb);

  uv_work_t *req = new uv_work_t;
  req->data = baton;

  uv_queue_work(uv_default_loop(), req, EIO_Region, (uv_after_work_cb)EIO_AfterRegion);

  NanReturnUndefined();
}

void geoip::Region::EIO_Region(uv_work_t *req)
{
  region_baton_t *baton = static_cast<region_baton_t *>(req->data);

  if (baton->ipnum <= 0) {
    baton->region = NULL;
  } else {
    baton->region = GeoIP_region_by_ipnum(baton->r->db, baton->ipnum);
  }
}

void geoip::Region::EIO_AfterRegion(uv_work_t *req)
{
  NanScope();

  region_baton_t *baton = static_cast<region_baton_t *>(req->data);

  Handle<Value> argv[2];

  if (baton->region == NULL) {
    argv[0] = Exception::Error(String::New("Data not found"));
    argv[1] = Null();
  } else {
    Local<Object> data = Object::New();
    data->Set(String::NewSymbol("country_code"), String::New(baton->region->country_code));
    data->Set(String::NewSymbol("region"), String::New(baton->region->region));

    argv[0] = Null();
    argv[1] = data;
  }

  TryCatch try_catch;
  NanPersistentToLocal(baton->cb)->Call(Context::GetCurrent()->Global(), 2, argv);

  // Cleanup
  NanDispose(baton->cb);
  delete baton;
  delete req;

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }
}

NAN_METHOD(geoip::Region::update) {
  NanLocker();

  NanScope();

  Region *r = ObjectWrap::Unwrap<Region>(args.This());

  String::Utf8Value file_str(args[0]->ToString());
  const char *file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value();

  r->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (r->db != NULL) {
    r->db_edition = GeoIP_database_edition(r->db);
    if (r->db_edition == GEOIP_REGION_EDITION_REV0 ||
        r->db_edition == GEOIP_REGION_EDITION_REV1) {
      NanReturnValue(True());
    } else {
      GeoIP_delete(r->db);  // free()'s the gi reference & closes its fd
      return NanThrowError("Error: Not valid region database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }

 NanUnlocker();
}

NAN_METHOD(geoip::Region::close) {
  Region * r = ObjectWrap::Unwrap<Region>(args.This());
  GeoIP_delete(r->db);  // free()'s the gi reference & closes its fd
}
