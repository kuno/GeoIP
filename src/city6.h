 /*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#ifndef NODE_GEOIP_CITY6_H
#define NODE_GEOIP_CITY6_H

#include <v8.h>
#include <node.h>
#include "init.h"

using namespace v8;
using namespace node;

namespace geoip {
  class City6: ObjectWrap
  {
    private:
      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static void EIO_City(uv_work_t *req);

      static void EIO_AfterCity(uv_work_t *req);

    protected:
      static Handle<Value> New(const Arguments &args);

    public:
      City6();

      ~City6();

      static void Init(Handle<Object> target);

      static Handle<Value> lookupSync(const Arguments &args);

      static Handle<Value> lookup(const Arguments &args);

      static Handle<Value> update(const Arguments &args);

      static void close(const Arguments &args);
  };

}

struct city6_baton_t {
  geoip::City6 *c;
  GeoIPRecord *record;
  geoipv6_t ipnum_v6;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_CITY6_H */
