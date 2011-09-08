/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                              

#include "netspeed.h"
#include "country6.h"
#include "country.h"
#include "region.h"
#include "city6.h"
#include "city.h"
#include "org.h"
#include "utils.h"

extern "C" {
  static void init(Handle<Object> target)
  {
    HandleScope scope;
    
    // Initialize Modules
    geoip::NetSpeed::Init(target);
    geoip::Country6::Init(target);
    geoip::Country::Init(target);
    geoip::Region::Init(target);
    geoip::City6::Init(target);
    geoip::City::Init(target);
    geoip::Org::Init(target);

    // Utility memeber method
    Local<FunctionTemplate> t = FunctionTemplate::New(geoip::check);
    target->Set(String::NewSymbol("check"), t->GetFunction());
    //only works with libgeoip >= 1.4.7
    target->Set(String::NewSymbol("libgeoip"), String::New(GeoIP_lib_version()));
  }

  NODE_MODULE(geoip, init);
} 
