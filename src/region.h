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

namespace native {
  class Region: public ObjectWrap {
    private:
      explicit Region();

      ~Region();

      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static void EIO_Region(uv_work_t *req);

      static void EIO_AfterRegion(uv_work_t *req);

      static NAN_METHOD(New);

      static NAN_METHOD(lookupSync);

    public:
      static void Init(Handle<Object> exports);
  };

}

#endif /* NODE_GEOIP_REGION_H */
