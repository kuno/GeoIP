/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                              

#ifndef NODE_GEOIP_COUNTRY_H
#define NODE_GEOIP_COUNTRY_H

#include <v8.h>
#include <node.h>
#include "node_geoip.h"

using namespace v8;
using namespace node;

namespace geoip {
  class Country: ObjectWrap
  {
    private:
      GeoIP *db;

      int db_edition;

      static Persistent<FunctionTemplate> constructor_template;

      static EIO_CUSTOM_TYPE EIO_Country(eio_req *req);

      static int EIO_AfterCountry(eio_req *req);

    protected:
      static Handle<Value> New(const Arguments& args);

    public:
      static void Init(Handle<Object> target);

      static Handle<Value> lookupSync(const Arguments& args);

      static Handle<Value> lookup(const Arguments& args);

      static Handle<Value> update(const Arguments &args);

      static Handle<Value> close(const Arguments &args);
  };
}

struct country_baton_t {
  geoip::Country * c;
  //char host_cstr[256];
  int country_id;
  uint32_t ipnum;
  Persistent<Function> cb;
};

#endif /* NODE_GEOIP_COUNTRY_H */
