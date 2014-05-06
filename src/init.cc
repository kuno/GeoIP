/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "netspeedcell.h"
#include "netspeed.h"
#include "country6.h"
#include "country.h"
#include "region.h"
#include "city6.h"
#include "city.h"
#include "org.h"
#include "utils.h"

using namespace native;

extern "C" {
  static void InitAll(Handle<Object> exports) {
    NanScope();

    // Initialize Modules
    NetSpeedCell::Init(exports);
    NetSpeed::Init(exports);
    Country6::Init(exports);
    Country::Init(exports);
    Region::Init(exports);
    City6::Init(exports);
    City::Init(exports);
    Org::Init(exports);

    // Utility memeber method
    Local<FunctionTemplate> check = NanNew<FunctionTemplate>(utils::check);
    Local<FunctionTemplate> isString = NanNew<FunctionTemplate>(utils::isString);
    exports->Set(NanSymbol("check"), check->GetFunction());
    exports->Set(NanSymbol("isString"), isString->GetFunction());

    // Meta infomation
    exports->Set(NanSymbol("libgeoip"), NanNew<String>(GeoIP_lib_version()));
  }

  NODE_MODULE(native, InitAll)
}
