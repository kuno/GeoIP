#include "netspeed.h"
#include "country.h"
#include "region.h"
#include "city.h"
#include "org.h"

//#include "test.h"
#include "utils.h"

extern "C" {
  static void init(Handle<Object> target)
  {
    HandleScope scope;

    geoip::NetSpeed::Init(target);
    geoip::Country::Init(target);
    geoip::Region::Init(target);
    geoip::City::Init(target);
    geoip::Org::Init(target);

    //geoip::Test::Init(target);
    // NODE_SET_METHOD(target, "check", geoip::check);

    Local<FunctionTemplate> t = FunctionTemplate::New(geoip::check);
    target->Set(String::NewSymbol("check"), t->GetFunction());
    //t = FunctionTemplate::New(geoip::addr_to_ipnum_v6);
    //target->Set(String::NewSymbol("addr_to_ipnum_v6"), t->GetFunction());
  }

  NODE_MODULE(geoip, init);
} 
