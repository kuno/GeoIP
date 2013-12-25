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
    NanScope();

    // Initialize Modules
    native::NetSpeed::Init(target);
    native::Country6::Init(target);
    native::Country::Init(target);
    native::Region::Init(target);
    native::City6::Init(target);
    native::City::Init(target);
    native::Org::Init(target);

    // Utility memeber method
    Local<FunctionTemplate> t = NanNewLocal<FunctionTemplate>(FunctionTemplate::New(native::check));
    target->Set(String::NewSymbol("check"), t->GetFunction());
    target->Set(String::NewSymbol("libgeoip"), String::New(GeoIP_lib_version()));
  }

  NODE_MODULE(native, init)
}
