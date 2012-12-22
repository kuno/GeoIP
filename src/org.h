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
      static Handle<Value> New(const Arguments &args);

    public:
      Org();

      ~Org();

      static void Init(Handle<Object> target);

      static Handle<Value> lookupSync(const Arguments &args);

      static Handle<Value> lookup(const Arguments &args);

      static Handle<Value> update(const Arguments &args);

      static void close(const Arguments &args);
  };
}

struct org_baton_t {
  geoip::Org *o;
  int ipnum;  // uint32_t?
  char *org;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_ORG_H */
