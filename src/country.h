/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#ifndef NODE_GEOIP_COUNTRY_H
#define NODE_GEOIP_COUNTRY_H

#include <v8.h>
#include <node.h>
#include "init.h"

using namespace v8;
using namespace node;

namespace geoip {
  class Country: ObjectWrap
  {
    private:
      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static void EIO_Country(uv_work_t *req);

      static void EIO_AfterCountry(uv_work_t *req);

    protected:
      static Handle<Value> New(const Arguments& args);

    public:
      Country();

      ~Country();

      static void Init(Handle<Object> target);

      static Handle<Value> lookupSync(const Arguments &args);

      static Handle<Value> lookup(const Arguments &args);

      static Handle<Value> update(const Arguments &args);

      static void close(const Arguments &args);
  };
}

struct country_baton_t {
  geoip::Country *c;
  //char host_cstr[256];
  int country_id;
  int ipnum;  // uint32_t?
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_COUNTRY_H */
