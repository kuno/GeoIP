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
  static NAN_MODULE_INIT(InitAll) {
    Nan::HandleScope scope;

    // Initialize Modules
    NetSpeedCell::Init(target);
    NetSpeed::Init(target);
    Country6::Init(target);
    Country::Init(target);
    Region::Init(target);
    City6::Init(target);
    City::Init(target);
    Org::Init(target);

    // Utility memeber method
    Local<FunctionTemplate> check = Nan::New<FunctionTemplate>(utils::check);
    Local<FunctionTemplate> isString = Nan::New<FunctionTemplate>(utils::isString);
    Nan::Set(target, Nan::New<String>("check").ToLocalChecked(), Nan::GetFunction(check).ToLocalChecked());
    Nan::Set(target, Nan::New<String>("isString").ToLocalChecked(), Nan::GetFunction(isString).ToLocalChecked());

    // Meta infomation
    Nan::Set(target, Nan::New<String>("libgeoip").ToLocalChecked(), Nan::New<String>(GeoIP_lib_version()).ToLocalChecked());
  }

  NODE_MODULE(native, InitAll)
}
