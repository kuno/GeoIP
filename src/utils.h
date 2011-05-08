/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
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

