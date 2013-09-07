/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#ifndef NODE_GEOIP_ORG_H
#define NODE_GEOIP_ORG_H

#include <v8.h>
#include <node.h>
#include "init.h"

using namespace v8;
using namespace node;

namespace geoip {
  class Org: ObjectWrap
  {
    private:
      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static void EIO_Org(uv_work_t *req);

      static void EIO_AfterOrg(uv_work_t *req);

    protected:
      static NAN_METHOD(New);

    public:
      Org();

      ~Org();

      static void Init(Handle<Object> target);

      static NAN_METHOD(lookupSync);

      static NAN_METHOD(lookup);

      static NAN_METHOD(update);

      static NAN_METHOD(close);
  };
}

struct org_baton_t {
  geoip::Org *o;
  int ipnum;  // uint32_t?
  char *org;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_ORG_H */
