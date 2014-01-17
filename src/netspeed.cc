/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "netspeed.h"
#include "global.h"

using namespace native;

NetSpeed::NetSpeed() : db(NULL) {};

NetSpeed::~NetSpeed() {
    if (db) {
        GeoIP_delete(db);
    }
};

Persistent<FunctionTemplate> NetSpeed::constructor_template;

void NetSpeed::Init(Handle<Object> exports) {
    NanScope();

    Local<FunctionTemplate> tpl = NanNewLocal<FunctionTemplate>(FunctionTemplate::New(New));
    NanAssignPersistent(FunctionTemplate, constructor_template, tpl);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewSymbol("NetSpeed"));

    tpl->PrototypeTemplate()->Set(String::NewSymbol("lookupSync"),
            FunctionTemplate::New(lookupSync)->GetFunction());
    exports->Set(String::NewSymbol("NetSpeed"), tpl->GetFunction());
}

NAN_METHOD(NetSpeed::New) {
    NanScope();

    NetSpeed *n = new NetSpeed();

    String::Utf8Value file_str(args[0]->ToString());
    const char * file_cstr = ToCString(file_str);
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


NAN_METHOD(NetSpeed::lookupSync) {
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
