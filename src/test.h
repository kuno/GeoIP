/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                              

#ifndef NODE_GEOIP_TEST_H
#define NODE_GEOIP_TEST_H

#include <v8.h>
#include <node.h>
#include "node_geoip.h"

using namespace v8;
using namespace node;

namespace geoip {
    class Test: ObjectWrap
    {
        private:
            GeoIP *db;

            int db_edition;

        public:
            Test();

            ~Test();

            static Persistent<FunctionTemplate> constructor_template;

            static void Init(Handle<Object> target);

            static Handle<Value> New(const Arguments& args);

            static Handle<Value> lookupSync(const Arguments &args);

            static Handle<Value> lookup(const Arguments& args);

            static int EIO_Test(eio_req *req);

            static int EIO_AfterTest(eio_req *req);

            static Handle<Value> close(const Arguments &args);
    };

}

struct test_baton_t {
  geoip::Test * c;
  char host_cstr[256];
  GeoIPRecord * record;
  int increment_by;
  int sleep_for;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_TEST_H */
