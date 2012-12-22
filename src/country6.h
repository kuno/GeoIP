/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#ifndef NODE_GEOIP_COUNTRY6_H
#define NODE_GEOIP_COUNTRY6_H

#include <v8.h>
#include <node.h>
#include "init.h"

using namespace v8;
using namespace node;

namespace geoip {
  class Country6: ObjectWrap
  {
    private:
      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static void EIO_Country(uv_work_t *req);

      static void EIO_AfterCountry(uv_work_t *req);

    protected:
      static Handle<Value> New(const Arguments &args);

    public:
      Country6();

      ~Country6();

      static void Init(Handle<Object> target);

      static Handle<Value> lookupSync(const Arguments &args);

      static Handle<Value> lookup(const Arguments &args);

      static Handle<Value> update(const Arguments &args);

      static void close(const Arguments &args);
  };
}

struct country6_baton_t {
  geoip::Country6 *c;
  geoipv6_t ipnum_v6;
  int country_id;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_COUNTRY6_H */
