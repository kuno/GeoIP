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
        public:
            static Persistent<FunctionTemplate> constructor_template;

            static void Init(Handle<Object> target);

            static Handle<Value> New(const Arguments& args);

            static Handle<Value> lookupSync(const Arguments &args);

            static Handle<Value> lookup(const Arguments& args);

            static int EIO_Region(eio_req *req);

            static int EIO_AfterRegion(eio_req *req);

            // Destroy the GeoIP* reference we're holding on to
            static Handle<Value> close(const Arguments &args);
    };

}
struct region_baton_t {
    geoip::Region *r;
    char host_cstr[256];  // standard length of ipv4
    GeoIPRegion *region;
    int increment_by;
    int sleep_for;
    Persistent<Function> cb;
};

#endif /* NODE_GEOIP_REGION_H */
