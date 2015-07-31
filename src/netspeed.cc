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

Nan::Persistent<FunctionTemplate> NetSpeed::constructor_template;

NAN_MODULE_INIT(NetSpeed::Init) {
    Nan::HandleScope scope;

    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    constructor_template.Reset(tpl);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(Nan::New<String>("NetSpeed").ToLocalChecked());

    Nan::SetPrototypeTemplate(tpl, "lookupSync",
            Nan::GetFunction(Nan::New<FunctionTemplate>(lookupSync)).ToLocalChecked());
    Nan::Set(target, Nan::New<String>("NetSpeed").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NetSpeed::New) {
    NetSpeed *n = new NetSpeed();

    Nan::Utf8String file_str(info[0]->ToString());
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
    NetSpeed *n = Nan::ObjectWrap::Unwrap<NetSpeed>(info.This());

    Local<Value> data = Nan::New(Nan::Null());

    Nan::Utf8String host_cstr(info[0]);
    uint32_t ipnum = _GeoIP_lookupaddress(*host_cstr);

    if (ipnum <= 0) {
        info.GetReturnValue().SetNull();
    }

    int netspeed = GeoIP_id_by_ipnum(n->db, ipnum);

    if (netspeed < 0) {
        info.GetReturnValue().Set(Nan::Null());
    } else if (netspeed == GEOIP_UNKNOWN_SPEED) {
        data = Nan::New<String>("Unknown").ToLocalChecked();
    } else if (netspeed == GEOIP_DIALUP_SPEED) {
        data = Nan::New<String>("Dialup").ToLocalChecked();
    } else if (netspeed == GEOIP_CABLEDSL_SPEED) {
        data = Nan::New<String>("CableDSL").ToLocalChecked();
    } else if (netspeed == GEOIP_CORPORATE_SPEED) {
        data = Nan::New<String>("Corporate").ToLocalChecked();
    }

    info.GetReturnValue().Set(data);
}
