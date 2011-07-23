/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                          

#include "netspeed.h"
#include "global.h"

Persistent<FunctionTemplate> geoip::NetSpeed::constructor_template;

void geoip::NetSpeed::Init(Handle<Object> target)
{
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("geoip"));

  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookup", lookup);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "lookupSync", lookupSync);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "update", update);
  //NODE_SET_PROTOTYPE_METHOD(constructor_template, "close", close);
  target->Set(String::NewSymbol("NetSpeed"), constructor_template->GetFunction());
}

/*
   geoip::NetSpeed::NetSpeed()
   {
   }

   geoip::NetSpeed::~NetSpeed()
   {
   }
   */

Handle<Value> geoip::NetSpeed::New(const Arguments& args)
{
  HandleScope scope;
  NetSpeed *n = new NetSpeed();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);      
  //Local<String> file_str = args[0]->ToString();
  //char file_cstr[file_str->Length()];
  //file_str->WriteAscii(file_cstr);
  bool cache_on = args[1]->ToBoolean()->Value(); 

  n->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (n->db != NULL) {
    n->db_edition = GeoIP_database_edition(n->db);
    if (n->db_edition == GEOIP_NETSPEED_EDITION) {
      n->Wrap(args.This());
      return scope.Close(args.This());
    } else {
      GeoIP_delete(n->db);	// free()'s the gi reference & closes its fd
      n->db = NULL;                                                       
      return scope.Close(ThrowException(String::New("Error: Not valid netspeed database")));
    }
  } else {
    return scope.Close(ThrowException(String::New("Error: Cao not open database")));
  }
}

Handle<Value> geoip::NetSpeed::lookupSync(const Arguments &args) {
  HandleScope scope;

  NetSpeed * n = ObjectWrap::Unwrap<NetSpeed>(args.This());

  Local<String> host_str = args[0]->ToString();
  Local<String> data;
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);

  uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

  if (ipnum <= 0) {
    return scope.Close(Null());
  }

  int netspeed = GeoIP_id_by_ipnum(n->db, ipnum);
  if (netspeed < 0) {
    return scope.Close(Null());
  } else if (netspeed == GEOIP_UNKNOWN_SPEED) {
    data = String::New("Uknown");
  } else if (netspeed == GEOIP_DIALUP_SPEED) {
    data = String::New("Dailup");
  } else if (netspeed == GEOIP_CABLEDSL_SPEED) {
    data = String::New("CableDSL");
  } else if (netspeed == GEOIP_CORPORATE_SPEED) {
    data = String::New("Corporate");
  }
  return scope.Close(data);
}

Handle<Value> geoip::NetSpeed::lookup(const Arguments& args)
{
  HandleScope scope;

  REQ_FUN_ARG(1, cb);

  NetSpeed * n = ObjectWrap::Unwrap<NetSpeed>(args.This());
  Local<String> host_str = args[0]->ToString();
  char host_cstr[host_str->Length()];
  host_str->WriteAscii(host_cstr);

  netspeed_baton_t *baton = new netspeed_baton_t();
  baton->n = n;
  baton->ipnum = _GeoIP_lookupaddress(host_cstr);
  baton->cb = Persistent<Function>::New(cb);

  n->Ref();

  eio_custom(EIO_NetSpeed, EIO_PRI_DEFAULT, EIO_AfterNetSpeed, baton);
  ev_ref(EV_DEFAULT_UC);

  return scope.Close(Undefined());
}

int geoip::NetSpeed::EIO_NetSpeed(eio_req *req)
{
  netspeed_baton_t *baton = static_cast<netspeed_baton_t *>(req->data);

  if (baton->ipnum < 0) {
    baton->netspeed = -1;
  } else {
    baton->netspeed = GeoIP_id_by_ipnum(baton->n->db, baton->ipnum);
  }

  return 0;
}

int geoip::NetSpeed::EIO_AfterNetSpeed(eio_req *req)
{
  HandleScope scope;

  netspeed_baton_t *baton = static_cast<netspeed_baton_t *>(req->data);
  ev_unref(EV_DEFAULT_UC);
  baton->n->Unref();

  Handle<Value> argv[2];
  if (baton->netspeed < 0) {
    argv[0] = Exception::Error(String::New("Data not found"));
    argv[1] = Null();
  } else {
    Local<String> data;
    if (baton->netspeed == GEOIP_UNKNOWN_SPEED) {
      data = String::New("Uknown");
    } else if (baton->netspeed == GEOIP_DIALUP_SPEED) {
      data = String::New("Dailup");
    } else if (baton->netspeed == GEOIP_CABLEDSL_SPEED) {
      data = String::New("CableDSL");
    } else if (baton->netspeed == GEOIP_CORPORATE_SPEED) {
      data = String::New("Corporate");
    }

    argv[0] =  Null();
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

Handle<Value> geoip::NetSpeed::update(const Arguments &args) {
  Locker locker();

  HandleScope scope;

  NetSpeed* n = ObjectWrap::Unwrap<NetSpeed>(args.This()); 

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);

  bool cache_on = args[1]->ToBoolean()->Value(); 

  n->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (n->db != NULL) {
    n->db_edition = GeoIP_database_edition(n->db);
    if (n->db_edition == GEOIP_NETSPEED_EDITION) {
      return scope.Close(True());
    } else {
      GeoIP_delete(n->db);	// free()'s the gi reference & closes its fd
      n->db = NULL;                                                       
      return scope.Close(ThrowException(String::New("Error: Not valid netspeed database")));
    }
  } else {
    return scope.Close(ThrowException(String::New("Error: Cao not open database")));
  }

  Unlocker unlocker();
}

Handle<Value> geoip::NetSpeed::close(const Arguments &args) {
  NetSpeed* n = ObjectWrap::Unwrap<NetSpeed>(args.This()); 
  GeoIP_delete(n->db);	// free()'s the gi reference & closes its fd
  n->db = NULL;
  HandleScope scope;	// Stick this down here since it seems to segfault when on top?
}
