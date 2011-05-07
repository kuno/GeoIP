/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */
#ifndef NODE_GEOIP_UTILS_H
#define NODE_GEOIP_UTILS_H

//#define IPV6_LEN 16

#include <v8.h>
#include <node.h>
//#include "node_geoip.h"

using namespace v8;
using namespace node;

namespace Utils
{
    //static Persistent<FunctionTemplate> construction_template;

    static void Init(Handle<Object> target);

    static Handle<Value> addr_to_ipnum(const Arguments& args);

    //static Handle<Value> addr_to_ipnum_v6(const Arguments &args);

    // double in6_to_num(geoipv6_t addr);
};

#endif /* NODE_GEOIP_UTILS_H */
