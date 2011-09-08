/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                              

#ifndef NODE_GEOIP_NETSPEED_H
#define NODE_GEOIP_NETSPEED_H

#include <v8.h>
#include <node.h>
#include "node_geoip.h"

using namespace v8;
using namespace node;

namespace geoip {
    class NetSpeed: ObjectWrap
    {
        private:
            GeoIP *db;

            int db_edition;

            static Persistent<FunctionTemplate> constructor_template;

            static int EIO_NetSpeed(eio_req *req);

            static int EIO_AfterNetSpeed(eio_req *req);

        protected:
            static Handle<Value> New(const Arguments& args);

        public:
            static void Init(Handle<Object> target);

            static Handle<Value> lookupSync(const Arguments &args);

            static Handle<Value> lookup(const Arguments& args);

            static Handle<Value> update(const Arguments &args);

            static Handle<Value> close(const Arguments &args);
    };
}

struct netspeed_baton_t {
    geoip::NetSpeed * n;
    uint32_t ipnum;
    int netspeed;
    Persistent<Function> cb;
};

#endif /* NODE_GEOIP_NETSPEED_H */
