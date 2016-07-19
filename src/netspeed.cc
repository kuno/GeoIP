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

Nan::Persistent<v8::Function> NetSpeed::constructor;

void NetSpeed::Init(v8::Local<v8::Object> exports) {
    Nan::HandleScope scope;

    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("NetSpeed").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "lookupSync", lookupSync);

    constructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New("NetSpeed").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(NetSpeed::New) {
    Nan::HandleScope scope;

    NetSpeed *n = new NetSpeed();

    String::Utf8Value file_str(info[0]->ToString());
    const char * file_cstr = ToCString(file_str);
    bool cache_on = info[1]->ToBoolean()->Value();

    n->db = GeoIP_open(file_cstr, cache_on ? GEOIP_MEMORY_CACHE : GEOIP_STANDARD);

    if (n->db) {
        n->db_edition = GeoIP_database_edition(n->db);
        if (n->db_edition == GEOIP_NETSPEED_EDITION ||
                n->db_edition == GEOIP_NETSPEED_EDITION_REV1) {
            n->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            GeoIP_delete(n->db);  // free()'s the reference & closes fd
            return Nan::ThrowError("Error: Not valid netspeed database");
        }
    } else {
        return Nan::ThrowError("Error: Cannot open database");
    }
}


NAN_METHOD(NetSpeed::lookupSync) {
    Nan::HandleScope scope;

    NetSpeed *n = ObjectWrap::Unwrap<NetSpeed>(info.This());

    //static Nan::Utf8String *host_cstr = new Nan::Utf8String(info[0]);
    Nan::Utf8String host_cstr(info[0]);
    //uint32_t ipnum = _GeoIP_lookupaddress(**host_cstr);
    uint32_t ipnum = _GeoIP_lookupaddress(*host_cstr);

    if (ipnum <= 0) {
        info.GetReturnValue().SetNull();
    }

    int netspeed = GeoIP_id_by_ipnum(n->db, ipnum);

    if (netspeed < 0) {
        info.GetReturnValue().SetNull();
    } else if (netspeed == GEOIP_UNKNOWN_SPEED) {
        info.GetReturnValue().Set(Nan::New<String>("Unknown").ToLocalChecked());
    } else if (netspeed == GEOIP_DIALUP_SPEED) {
        info.GetReturnValue().Set(Nan::New<String>("Dialup").ToLocalChecked());
    } else if (netspeed == GEOIP_CABLEDSL_SPEED) {
        info.GetReturnValue().Set(Nan::New<String>("CableDSL").ToLocalChecked());
    } else if (netspeed == GEOIP_CORPORATE_SPEED) {
        info.GetReturnValue().Set(Nan::New<String>("Corporate").ToLocalChecked());
    }
}
