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
      static NAN_METHOD(New);

    public:
      Country();

      ~Country();

      static void Init(Handle<Object> target);

      static NAN_METHOD(lookupSync);

      static NAN_METHOD(lookup);

      static NAN_METHOD(update);

      static NAN_METHOD(close);
  };
}

struct country_baton_t {
  geoip::Country *c;
  //char host_cstr[256];
  int country_id;
  uint32_t ipnum;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_COUNTRY_H */
