/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "netspeed.h"
#include "global.h"

Persistent<FunctionTemplate> native::NetSpeed::constructor_template;

void native::NetSpeed::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  NanAssignPersistent(FunctionTemplate, constructor_template, t);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(t, "lookup", lookup);
  NODE_SET_PROTOTYPE_METHOD(t, "lookupSync", lookupSync);
  NODE_SET_PROTOTYPE_METHOD(t, "lookupCellularSync", lookupCellularSync);
  NODE_SET_PROTOTYPE_METHOD(t, "update", update);
  //NODE_SET_PROTOTYPE_METHOD(t, "close", close);
  target->Set(String::NewSymbol("NetSpeed"), t->GetFunction());
}

native::NetSpeed::NetSpeed() : db(NULL) {};

native::NetSpeed::~NetSpeed() {
  if (db) {
    GeoIP_delete(db);
  }
};

NAN_METHOD(native::NetSpeed::New) {
  NanScope();
  NetSpeed *n = new NetSpeed();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  //Local<String> file_str = args[0]->ToString();
  //char file_cstr[file_str->Length()];
  //file_str->WriteAscii(file_cstr);
  bool cache_on = args[1]->ToBoolean()->Value();

  n->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (n->db) {
    n->db_edition = GeoIP_database_edition(n->db);
    if (n->db_edition == GEOIP_NETSPEED_EDITION ||
        n->db_edition == GEOIP_NETSPEED_EDITION_REV1) {
      n->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(n->db);  // free()'s the reference & closes fd
      return NanThrowError("Error: Not valid netspeed database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(native::NetSpeed::lookupCellularSync) {
  NanScope();

  NetSpeed *n = ObjectWrap::Unwrap<NetSpeed>(args.This());

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

NAN_METHOD(native::NetSpeed::lookupSync) {
  NanScope();

  NetSpeed *n = ObjectWrap::Unwrap<NetSpeed>(args.This());

  Local<Value> data = NanNewLocal<Value>(Null());
  Local<String> host_str = NanNewLocal<String>(args[0]->ToString());
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  if (ipnum <= 0) {
    NanReturnValue(Null());
  }

  int netspeed = GeoIP_id_by_ipnum(n->db, ipnum);

  if (netspeed < 0) {
    NanReturnValue(Null());
  } else if (netspeed == GEOIP_UNKNOWN_SPEED) {
    data = String::New("Unknown");
  } else if (netspeed == GEOIP_DIALUP_SPEED) {
    data = String::New("Dialup");
  } else if (netspeed == GEOIP_CABLEDSL_SPEED) {
    data = String::New("CableDSL");
  } else if (netspeed == GEOIP_CORPORATE_SPEED) {
    data = String::New("Corporate");
  }

  NanReturnValue(data);
}

NAN_METHOD(native::NetSpeed::lookup) {
  NanScope();

  REQ_FUN_ARG(1, cb);

  NetSpeed *n = ObjectWrap::Unwrap<NetSpeed>(args.This());
  Local<String> host_str = NanNewLocal<String>(args[0]->ToString());
  char host_cstr[host_str->Length() + 1];
  NanFromV8String(args[0].As<Object>(), Nan::ASCII, NULL, host_cstr, host_str->Length() + 1, v8::String::HINT_MANY_WRITES_EXPECTED);

  netspeed_baton_t *baton = new netspeed_baton_t();
  baton->n = n;
  baton->ipnum = _GeoIP_lookupaddress(host_cstr);
  NanAssignPersistent(Function, baton->cb, cb);

  uv_work_t *req = new uv_work_t;
  req->data = baton;

  uv_queue_work(uv_default_loop(), req, EIO_NetSpeed, (uv_after_work_cb)EIO_AfterNetSpeed);

  NanReturnUndefined();
}

void native::NetSpeed::EIO_NetSpeed(uv_work_t *req)
{
  netspeed_baton_t *baton = static_cast<netspeed_baton_t *>(req->data);

  if (baton->ipnum < 0) {
    baton->netspeed = -1;
  } else {
    baton->netspeed = GeoIP_id_by_ipnum(baton->n->db, baton->ipnum);
  }
}

void native::NetSpeed::EIO_AfterNetSpeed(uv_work_t *req) {
  NanScope();

  netspeed_baton_t *baton = static_cast<netspeed_baton_t *>(req->data);

  Local<Value> data = NanNewLocal<Value>(Null());
  Handle<Value> argv[2];

  if (baton->netspeed < 0) {
    argv[0] = Exception::Error(String::New("Data not found"));
    argv[1] = Null();
  } else {
    if (baton->netspeed == GEOIP_UNKNOWN_SPEED) {
      data = String::New("Unknown");
    } else if (baton->netspeed == GEOIP_DIALUP_SPEED) {
      data = String::New("Dialup");
    } else if (baton->netspeed == GEOIP_CABLEDSL_SPEED) {
      data = String::New("CableDSL");
    } else if (baton->netspeed == GEOIP_CORPORATE_SPEED) {
      data = String::New("Corporate");
    }

    argv[0] =  Null();
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

NAN_METHOD(native::NetSpeed::update) {
  NanLocker();

  NanScope();

  NetSpeed *n = ObjectWrap::Unwrap<NetSpeed>(args.This());

  String::Utf8Value file_str(args[0]->ToString());
  const char *file_cstr = ToCString(file_str);
  bool cache_on = args[1]->ToBoolean()->Value();

  n->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

  if (n->db) {
    n->db_edition = GeoIP_database_edition(n->db);
    if (n->db_edition == GEOIP_NETSPEED_EDITION) {
      NanReturnValue(True());
    } else {
      GeoIP_delete(n->db);  // free()'s the reference & closes fd
      return NanThrowError("Error: Not valid netspeed database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }

  NanUnlocker();
}

NAN_METHOD(native::NetSpeed::close) {
  NetSpeed *n = ObjectWrap::Unwrap<NetSpeed>(args.This());
  GeoIP_delete(n->db);  // free()'s the gi reference & closes its fd
}
