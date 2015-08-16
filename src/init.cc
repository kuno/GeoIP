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
  static void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
    Nan::HandleScope scope;

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
    v8::Local<FunctionTemplate> check = Nan::New<v8::FunctionTemplate>(utils::check);
    v8::Local<FunctionTemplate> isString = Nan::New<v8::FunctionTemplate>(utils::isString);

    exports->Set(Nan::New("check").ToLocalChecked(),
      check->GetFunction());
    exports->Set(Nan::New("isString").ToLocalChecked(),
      isString->GetFunction());
    exports->Set(Nan::New("libgeoip").ToLocalChecked(),
        Nan::New(GeoIP_lib_version()).ToLocalChecked());

  }

  NODE_MODULE(native, InitAll)
}
