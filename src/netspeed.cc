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

    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    NanAssignPersistent(constructor_template, tpl);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(NanSymbol("NetSpeed"));

    tpl->PrototypeTemplate()->Set(NanSymbol("lookupSync"),
            NanNew<FunctionTemplate>(lookupSync)->GetFunction());
    exports->Set(NanSymbol("NetSpeed"), tpl->GetFunction());
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

    Local<Value> data = NanNew(NanNull());
    Local<String> host_str = args[0]->ToString();
    size_t size = host_str->Length() + 1;
    char host_cstr[size];
    size_t bc;
    NanCString(args[0], &bc, host_cstr, size);

    uint32_t ipnum = _GeoIP_lookupaddress(host_cstr);

    if (ipnum <= 0) {
        NanReturnValue(NanNull());
    }

    int netspeed = GeoIP_id_by_ipnum(n->db, ipnum);

    if (netspeed < 0) {
        NanReturnValue(NanNull());
    } else if (netspeed == GEOIP_UNKNOWN_SPEED) {
        data = NanNew<String>("Unknown");
    } else if (netspeed == GEOIP_DIALUP_SPEED) {
        data = NanNew<String>("Dialup");
    } else if (netspeed == GEOIP_CABLEDSL_SPEED) {
        data = NanNew<String>("CableDSL");
    } else if (netspeed == GEOIP_CORPORATE_SPEED) {
        data = NanNew<String>("Corporate");
    }

    NanReturnValue(data);
}
