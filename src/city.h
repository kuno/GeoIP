 /*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                              

#ifndef NODE_GEOIP_CITY_H
#define NODE_GEOIP_CITY_H

#include <v8.h>
#include <node.h>
#include "node_geoip.h"

using namespace v8;
using namespace node;

namespace geoip {
  class City: ObjectWrap
  {
    private:
      GeoIP *db;

      int db_edition;

    protected:
      static Handle<Value> New(const Arguments& args);

    public:
      static Persistent<FunctionTemplate> constructor_template;

      static void Init(Handle<Object> target);

      static Handle<Value> lookupSync(const Arguments &args);

      static Handle<Value> lookup(const Arguments& args);

      static int EIO_City(eio_req *req);

      static int EIO_AfterCity(eio_req *req);

      static Handle<Value> close(const Arguments &args);
  };

}
struct city_baton_t {
  geoip::City *c;
  char host_cstr[256];  // standard length of ipv4
  GeoIPRecord *record;
  int increment_by;
  int sleep_for;
  Persistent<Function> cb;
};

//// Extracts a C string from a V8 Utf8Value.
//const char * ToCString(const String::AsciiValue& value) {
//  return *value ? *value : "<string conversion failed>";
//}                                                                 

#endif /* NODE_GEOIP_CITY_H */
