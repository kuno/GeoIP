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
      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static void EIO_NetSpeed(uv_work_t *req);

      static void EIO_AfterNetSpeed(uv_work_t *req);

    protected:
      static NAN_METHOD(New);

    public:
      NetSpeed();

      ~NetSpeed();

      static void Init(Handle<Object> target);

      static NAN_METHOD(lookupSync);

      static NAN_METHOD(lookupCellularSync);

      static NAN_METHOD(lookup);

      static NAN_METHOD(update);

      static NAN_METHOD(close);
  };
}

struct netspeed_baton_t {
  native::NetSpeed *n;
  int ipnum;  // uint32_t?
  int netspeed;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_NETSPEED_H */
