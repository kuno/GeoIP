/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "org.h"
#include "global.h"

Persistent<FunctionTemplate> geoip::Org::constructor_template;

void geoip::Org::Init(Handle<Object> target)
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
  target->Set(String::NewSymbol("Org"), t->GetFunction());
}

geoip::Org::Org() {};

geoip::Org::~Org() {};

NAN_METHOD(geoip::Org::New)
{
  NanScope();

  Org *o = new Org();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  //Local<String> path_str = args[0]->ToString();
  //char path_cstr[path_str->Length()];
  //path_str->WriteAscii(path_cstr);
  bool cache_on = args[1]->ToBoolean()->Value();

  o->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (o->db != NULL) {
    // Successfully opened the file, return 1 (true)
    o->db_edition = GeoIP_database_edition(o->db);
    if (o->db_edition == GEOIP_ORG_EDITION ||
        o->db_edition == GEOIP_ASNUM_EDITION ||
        o->db_edition == GEOIP_ISP_EDITION) {
      o->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(o->db);  // free()'s the gi reference & closes its fd
      return NanThrowError("Error: Not valid org database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(geoip::Org::lookupSync) {
  NanScope();

  Local<Value> data;
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);
  Org * o = ObjectWrap::Unwrap<geoip::Org>(args.This());

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  if (ipnum <= 0) {
    NanReturnValue(Null());
  }

  char *org = GeoIP_org_by_ipnum(o->db, ipnum);
  if (org == NULL) {
    NanReturnValue(Null());
  }

  char *name = _GeoIP_iso_8859_1__utf8(org);

  data = String::New(name);

  free(org);
  free(name);

  NanReturnValue(data);
}

NAN_METHOD(geoip::Org::lookup)
{
  NanScope();

  REQ_FUN_ARG(1, cb);

  Org *o = ObjectWrap::Unwrap<geoip::Org>(args.This());
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  org_baton_t *baton = new org_baton_t();
  baton->o = o;
  baton->ipnum = _GeoIP_lookupaddress(host_cstr);
  NanAssignPersistent(Function, baton->cb, cb);

  uv_work_t *req = new uv_work_t;
  req->data = baton;

  uv_queue_work(uv_default_loop(), req, EIO_Org, (uv_after_work_cb)EIO_AfterOrg);

  NanReturnUndefined();
}

void geoip::Org::EIO_Org(uv_work_t *req)
{
  org_baton_t *baton = static_cast<org_baton_t *>(req->data);

  if (baton->ipnum <= 0) {
    baton->org = NULL;
  }

  baton->org = GeoIP_org_by_ipnum(baton->o->db, baton->ipnum);
}

void geoip::Org::EIO_AfterOrg(uv_work_t *req)
{
  NanScope();

  org_baton_t *baton = static_cast<org_baton_t *>(req->data);

  Handle<Value> argv[2];

  if (baton->org == NULL) {
    argv[0] = Exception::Error(String::New("Data not found"));
    argv[1] = Null();
  } else {
    char * name = _GeoIP_iso_8859_1__utf8(baton->org);

    Local<String> data = String::New(name);

    argv[0] = Null();
    argv[1] = data;

    free(name);
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

NAN_METHOD(geoip::Org::update) {
  NanLocker();

  NanScope();

  Org *o = ObjectWrap::Unwrap<Org>(args.This());

  String::Utf8Value file_str(args[0]->ToString());
  const char *file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value();

  o->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (o->db != NULL) {
    o->db_edition = GeoIP_database_edition(o->db);
   if (o->db_edition == GEOIP_ORG_EDITION ||
       o->db_edition == GEOIP_ASNUM_EDITION ||
       o->db_edition == GEOIP_ISP_EDITION) {
      NanReturnValue(True());
    } else {
      GeoIP_delete(o->db);  // free()'s the gi reference & closes its fd
      return NanThrowError("Error: Not valid organization database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }

 NanUnlocker();
}

NAN_METHOD(geoip::Org::close) {
  Org* o = ObjectWrap::Unwrap<geoip::Org>(args.This());
  GeoIP_delete(o->db);  // free()'s the gi reference & closes its fd
}
