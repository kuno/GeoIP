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
      static NAN_METHOD(New);

    public:
      City6();

      ~City6();

      static void Init(Handle<Object> target);

      static NAN_METHOD(lookupSync);

      static NAN_METHOD(lookup);

      static NAN_METHOD(update);

      static NAN_METHOD(close);
  };

}

struct city6_baton_t {
  geoip::City6 *c;
  GeoIPRecord *record;
  geoipv6_t ipnum_v6;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_CITY6_H */
