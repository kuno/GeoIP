/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "country6.h"
#include "global.h"

using namespace native;

Country6::Country6(): db(NULL) {};

Country6::~Country6() {
  if (db) {
    GeoIP_delete(db);
  }
};

Persistent<FunctionTemplate> Country6::constructor_template;

void Country6::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor_template, tpl);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(NanSymbol("Country6"));

  tpl->PrototypeTemplate()->Set(NanSymbol("lookupSync"),
      NanNew<FunctionTemplate>(lookupSync)->GetFunction());
  exports->Set(NanSymbol("Country6"), tpl->GetFunction());
}

NAN_METHOD(Country6::New) {
  NanScope();

  Country6 *c = new Country6();

  String::Utf8Value file_str(args[0]->ToString());
  const char * file_cstr = ToCString(file_str);
  bool cache_on = args[1]->ToBoolean()->Value();

  c->db = GeoIP_open(file_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

  if (c->db) {
    c->db_edition = GeoIP_database_edition(c->db);
    if (c->db_edition == GEOIP_COUNTRY_EDITION_V6) {
      c->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      GeoIP_delete(c->db);  // free()'s the reference & closes fd
      delete c->db;
      return NanThrowError("Error: Not valid country database");
    }
  } else {
    return NanThrowError("Error: Cannot open database");
  }
}

NAN_METHOD(Country6::lookupSync) {
  NanScope();

  Country6 *c = ObjectWrap::Unwrap<Country6>(args.This());

  // Check if database is country ipv6 edition
  if (c->db_edition != GEOIP_COUNTRY_EDITION_V6) {
    return NanThrowError("Error: Database is not country ipv6 edition");
  }

  Local<Object> data = NanNew<Object>();
  Local<String> host_str = args[0]->ToString();
  size_t size = host_str->Length() + 1;
  char host_cstr[size];
  size_t bc;

  NanCString(args[0], &bc, host_cstr, size);

  geoipv6_t ipnum_v6 = _GeoIP_lookupaddress_v6(host_cstr);

  if (__GEOIP_V6_IS_NULL(ipnum_v6)) {
    NanReturnValue(NanNull());
  } else {
    int country_id = GeoIP_id_by_ipnum_v6(c->db, ipnum_v6);
    if (country_id == 0) {
      NanReturnValue(NanNull());
    } else {
      char *name = _GeoIP_iso_8859_1__utf8(GeoIP_country_name[country_id]);

      data->Set(NanSymbol("country_name"), NanNew<String>(name));
      data->Set(NanSymbol("country_code"), NanNew<String>(GeoIP_country_code[country_id]));
      data->Set(NanSymbol("country_code3"), NanNew<String>(GeoIP_country_code3[country_id]));
      data->Set(NanSymbol("continent_code"), NanNew<String>(GeoIP_country_continent[country_id]));

      free(name);

      NanReturnValue(data);
    }
  }
}
