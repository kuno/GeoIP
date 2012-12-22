/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#ifndef NODE_GEOIP_REGION_H
#define NODE_GEOIP_REGION_H

#include <v8.h>
#include <node.h>
#include "init.h"

using namespace v8;
using namespace node;

namespace geoip {
  class Region: ObjectWrap
  {
    private:
      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static void EIO_Region(uv_work_t *req);

      static void EIO_AfterRegion(uv_work_t *req);

    protected:
      static Handle<Value> New(const Arguments &args);

    public:
      Region();

      ~Region();

      static void Init(Handle<Object> target);

      static Handle<Value> lookupSync(const Arguments &args);

      static Handle<Value> lookup(const Arguments &args);

      static Handle<Value> update(const Arguments &args);

      static void close(const Arguments &args);
  };

}
struct region_baton_t {
  geoip::Region *r;
  int ipnum;  // uint32_t?
  GeoIPRegion *region;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_REGION_H */
