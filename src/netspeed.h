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

namespace geoip {
  class NetSpeed: ObjectWrap
  {
    private:
      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static void EIO_NetSpeed(uv_work_t *req);

      static void EIO_AfterNetSpeed(uv_work_t *req);

    protected:
      static Handle<Value> New(const Arguments &args);

    public:
      NetSpeed();

      ~NetSpeed();

      static void Init(Handle<Object> target);

      static Handle<Value> lookupSync(const Arguments &args);

      static Handle<Value> lookup(const Arguments &args);

      static Handle<Value> update(const Arguments &args);

      static void close(const Arguments &args);
  };
}

struct netspeed_baton_t {
  geoip::NetSpeed *n;
  int ipnum;  // uint32_t?
  int netspeed;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_NETSPEED_H */
