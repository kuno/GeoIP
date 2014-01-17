/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#ifndef NODE_GEOIP_NETSPEED_H
#define NODE_GEOIP_NETSPEED_H

#include <v8.h>
#include <node.h>
#include "init.h"

using namespace v8;
using namespace node;

namespace native {
  class NetSpeed: public ObjectWrap {
    private:
        explicit NetSpeed();

        ~NetSpeed();

        GeoIP *db;

        int db_edition;

        static Persistent<FunctionTemplate> constructor_template;

        static NAN_METHOD(New);

        static NAN_METHOD(lookupSync);

    public:
        static void Init(Handle<Object> exports);

  };
}

#endif /* NODE_GEOIP_NETSPEED_H */
