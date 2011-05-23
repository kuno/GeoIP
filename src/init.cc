/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                              

#include "netspeed.h"
#include "country.h"
#include "region.h"
#include "city.h"
#include "org.h"
#include "utils.h"
#include "test.h"

extern "C" {
  static void init(Handle<Object> target)
  {
    HandleScope scope;
    
    // Modules
    geoip::NetSpeed::Init(target);
    geoip::Country::Init(target);
    geoip::Region::Init(target);
    geoip::City::Init(target);
    geoip::Org::Init(target);

    geoip::Test::Init(target);

    // utility memeber method
    Local<FunctionTemplate> t = FunctionTemplate::New(geoip::check);
    target->Set(String::NewSymbol("check"), t->GetFunction());
  }

  NODE_MODULE(geoip, init);
} 
