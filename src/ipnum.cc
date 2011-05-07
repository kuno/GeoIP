/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */

#include <v8.h>
#include <GeoIP.h>
#include <GeoIPCity.h>
#include "ipnum.h"

using namespace v8;

double in6_to_num(in6_addr addr) {
  int i;
  double num_v6 = 0;

  for (i = IPV6_LEN; i > 0; i--) {
    num_v6 += (addr.__in6_u.__u6_addr8[i] << i);
  }

  return num_v6;

}         


extern "C" GEOIP_API unsigned long _GeoIP_addr_to_num(const char *addr);
// ipv6
extern "C" GEOIP_API geoipv6_t _GeoIP_addr_to_num_v6 (const char *addr);

namespace geoip {
  // Pass this method an IP Address string and it will spit back coordinates
  Handle<Value> addr_to_ipnum(const Arguments &args) {
    HandleScope scope;

    // Copy the argument strings into local cstrings
    // This is the only way I could figure out how to do this.
    // If anyone knows a better way, let me know.

    Local<String> ip_str = args[0]->ToString();
    char ip_cstr[ip_str->Length()];
    ip_str->WriteAscii(ip_cstr);		
    uint32_t num = _GeoIP_addr_to_num(ip_cstr);

    // Here I'm just returning the latitude, longitude
    // If you want to pull out city information, etc,
    //   this is the place to do it.
    Local<Number> ipnum  = Number::New(num);
    return ipnum;
  }

  Handle<Value> addr_to_ipnum_v6(const Arguments &args) {
    HandleScope scope;

    Local<String> ip_str = args[0]->ToString();
    char ip_cstr[ip_str->Length()];
    ip_str->WriteAscii(ip_cstr);		
    geoipv6_t in6 = _GeoIP_addr_to_num_v6(ip_cstr);

    double num = in6_to_num(in6);

    Local<Number> ipnum_v6  = Number::New(num);
    return ipnum_v6;
  }                                                       
}

/*
   extern "C" void init(Handle<Object> target)
   {
   HandleScope scope;

// Add lookup, open, and close functions to GeoIP class
Local<FunctionTemplate> t = FunctionTemplate::New(geoip::addr_to_ipnum);
target->Set(String::NewSymbol("addr_to_ipnum"), t->GetFunction());
t = FunctionTemplate::New(geoip::addr_to_ipnum_v6);
target->Set(String::NewSymbol("addr_to_ipnum_v6"), t->GetFunction());
} */
