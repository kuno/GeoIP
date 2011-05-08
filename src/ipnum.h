/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */
#ifndef NODE_GEOIP_UTILS_H
#define NODE_GEOIP_UTILS_H
#define IPV6_LEN 16

#include <v8.h>
#include <node.h>
#include <netinet/in.h>

#include "node_geoip.h"

using namespace v8;
using namespace node;

namespace geoip {
  Handle<Value> addr_to_ipnum(const Arguments &args);

  Handle<Value> addr_to_ipnum_v6(const Arguments &args);
}

/*
double in6_to_num(in6_addr addr) {
  int i;
  double num_v6 = 0;

  for (i = IPV6_LEN; i > 0; i--) {
    num_v6 += (addr.__in6_u.__u6_addr8[i] << i);
  }

  return num_v6;
} */        

#endif /* NODE_GEOIP_UTILS_H */

