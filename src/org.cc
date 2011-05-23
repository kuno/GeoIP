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
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookup", lookup);
  //NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookup6", lookup6);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookupSync", lookupSync);
  //NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookupSync6", lookupSync6);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "close", close);
  target->Set(String::NewSymbol("Org"), constructor_template->GetFunction());
}

/*
   geoip::Org::Org()
   {
   }

   geoip::Org::~Org()
   {
   }
   */

Handle<Value> geoip::Org::New(const Arguments& args)
{
  HandleScope scope;
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
      return scope.Close(args.This());
    } else {
      GeoIP_delete(o->db);	// free()'s the gi reference & closes its fd
      o->db = NULL;                                                       
      return scope.Close(ThrowException(String::New("Error: Not valid org database")));
    }
  } else {
    return scope.Close(ThrowException(String::New("Error: Cao not open database")));
  }
}

Handle<Value> geoip::Org::lookupSync(const Arguments &args) {
  HandleScope scope;

  Local<String> host_str = args[0]->ToString();
  Local<Value> data;
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);
  Org* o = ObjectWrap::Unwrap<geoip::Org>(args.This());

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);
  if (ipnum <= 0) {
    return scope.Close(Null());
  }

  char *org = GeoIP_org_by_ipnum(o->db, ipnum);
  if (org == NULL) {
    return scope.Close(Null());
  }

  data = String::New(org);
  return scope.Close(data);
}

Handle<Value> geoip::Org::lookup(const Arguments& args)
{
  HandleScope scope;

  REQ_FUN_ARG(1, cb);

  Org *o = ObjectWrap::Unwrap<geoip::Org>(args.This());
  Local<String> host_str = args[0]->ToString();

  org_baton_t *baton = new org_baton_t();

  baton->o = o;
  host_str->WriteAscii(baton->host_cstr);
  baton->increment_by = 2;
  baton->sleep_for = 1;
  baton->cb = Persistent<Function>::New(cb);

  o->Ref();

  eio_custom(EIO_Org, EIO_PRI_DEFAULT, EIO_AfterOrg, baton);
  ev_ref(EV_DEFAULT_UC);

  return Undefined();
}

int geoip::Org::EIO_Org(eio_req *req)
{
  org_baton_t *baton = static_cast<org_baton_t *>(req->data);

  sleep(baton->sleep_for);

  uint32_t ipnum = _GeoIP_lookupaddress(baton->host_cstr);
  if (ipnum <= 0) {
    baton->org = NULL;
  } 

  baton->org = GeoIP_org_by_ipnum(baton->o->db, ipnum);

  return 0;
}

int geoip::Org::EIO_AfterOrg(eio_req *req)
{
  HandleScope scope;

  org_baton_t *baton = static_cast<org_baton_t *>(req->data);
  ev_unref(EV_DEFAULT_UC);
  baton->o->Unref();

  Handle<Value> argv[2];

  if (baton->org == NULL) {
    argv[0] = Exception::Error(String::New("Data not found"));
    argv[1] = Null();
  } else {
    Local<String> data = String::New(baton->org);

    argv[0] = Null();
    argv[1] = data;
  }

  TryCatch try_catch;

  baton->cb->Call(Context::GetCurrent()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  baton->cb.Dispose();

  delete baton;
  return 0;
}

Handle<Value> geoip::Org::close(const Arguments &args) {
  Org* o = ObjectWrap::Unwrap<geoip::Org>(args.This()); 
  GeoIP_delete(o->db);	// free()'s the gi reference & closes its fd
  o->db = NULL;
  HandleScope scope;	// Stick this down here since it seems to segfault when on top?
}
