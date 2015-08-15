/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "country.h"
#include "global.h"

using namespace native;

Country::Country() : db(NULL) {};

Country::~Country() {
  if (db) {
    GeoIP_delete(db);
  }
};

//Persistent<FunctionTemplate> Country::constructor_template;

Nan::Persistent<v8::Function> Country::constructor;

void Country::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Country").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  //NanAssignPersistent(constructor_template, tpl);
  //tpl->InstanceTemplate()->SetInternalFieldCount(1);
  //tpl->SetClassName(Nan::New<String>("Country"));

  tpl->PrototypeTemplate()->Set(Nan::New("lookupSync").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(lookupSync)->GetFunction());

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Country").ToLocalChecked(), tpl->GetFunction());

  //tpl->PrototypeTemplate()->Set(Nan::New<String>("lookupSync"),
  //    Nan::New<FunctionTemplate>(lookupSync)->GetFunction());
  //exports->Set(Nan::New<String>("Country"), tpl->GetFunction());
}

void Country::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::HandleScope scope;

  Country *c = new Country();

  String::Utf8Value file_str(info[0]->ToString());
  const char * file_cstr = ToCString(file_str);
  bool cache_on = info[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION) {
      c->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      GeoIP_delete(c->db);  // free()'s the reference & closes fd
      delete c->db;
      return Nan::ThrowError("Error: Not valid country database");
    }
  } else {
    return Nan::ThrowError("Error: Cannot open database");
  }
}

void Country::lookupSync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::HandleScope scope;

  Country *c = ObjectWrap::Unwrap<Country>(info.This());

  //Local<Object> data = Nan::New<Object>();
  v8::Local<v8::Object> data = Nan::New<v8::Object>();

  static Nan::Utf8String *host_cstr = new Nan::Utf8String(info[0]);
  uint32_t ipnum = _GeoIP_lookupaddress(**host_cstr);

  if (ipnum <= 0) {
    info.GetReturnValue().Set(Nan::Null());
  } else {
    int country_id = GeoIP_id_by_ipnum(c->db, ipnum);
    if (country_id == 0) {
      info.GetReturnValue().Set(Nan::Null());
    } else {
      char *name = _GeoIP_iso_8859_1__utf8(GeoIP_country_name[country_id]);

      data->Set(Nan::New("country_name").ToLocalChecked(), Nan::New(name).ToLocalChecked());
      data->Set(Nan::New("country_code").ToLocalChecked(), Nan::New(GeoIP_country_code[country_id]).ToLocalChecked());
      data->Set(Nan::New("country_code3").ToLocalChecked(), Nan::New(GeoIP_country_code3[country_id]).ToLocalChecked());
      data->Set(Nan::New("continent_code").ToLocalChecked(), Nan::New(GeoIP_country_continent[country_id]).ToLocalChecked());

      free(name);

      info.GetReturnValue().Set(data);
    }
  }
}
