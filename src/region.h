/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                              

#ifndef NODE_GEOIP_REGION_H
#define NODE_GEOIP_REGION_H

#include <v8.h>
#include <node.h>
#include "node_geoip.h"

using namespace v8;
using namespace node;

namespace geoip {
    class Region: ObjectWrap
    {
        private:
            GeoIP *db;
            int db_edition;

            static Persistent<FunctionTemplate> constructor_template;

            static int EIO_Region(eio_req *req);

            static int EIO_AfterRegion(eio_req *req);

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
struct region_baton_t {
    geoip::Region *r;
    uint32_t ipnum;
    GeoIPRegion *region;
    Persistent<Function> cb;
};

#endif /* NODE_GEOIP_REGION_H */
