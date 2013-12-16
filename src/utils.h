/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#ifndef GEOIP_UTILS_H
#define GEOIP_UTILS_H

#include <v8.h>
#include <node.h>

#include "init.h"

using namespace v8;
using namespace node;

namespace native {

  NAN_METHOD(check);

}

#endif /* GEOIP_UTILS_H */

