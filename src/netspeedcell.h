/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#ifndef NODE_GEOIP_NETSPEEDCELL_H
#define NODE_GEOIP_NETSPEEDCELL_H

#include <v8.h>
#include <node.h>
#include "init.h"

using namespace v8;
using namespace node;

namespace native {
  class NetSpeedCell: public Nan::ObjectWrap {
    private:
      explicit NetSpeedCell();

      ~NetSpeedCell();

      GeoIP *db;

      int db_edition;

      static Nan::Persistent<FunctionTemplate> constructor_template;

      static NAN_METHOD(New);

      static NAN_METHOD(lookupSync);

    public:
      static NAN_MODULE_INIT(Init);
  };
}

#endif /* NODE_GEOIP_NETSPEEDCELL_H */
