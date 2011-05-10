/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                              

#ifndef NODE_GEOIP_UTILS_H
#define NODE_GEOIP_UTILS_H

#include <v8.h>
#include <node.h>

#include "node_geoip.h"

using namespace v8;
using namespace node;

namespace geoip {

  Handle<Value> check(const Arguments &args);

}

#endif /* NODE_GEOIP_UTILS_H */

