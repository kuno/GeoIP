#include "country.h"
#include "city.h"
#include "ipnum.h"
#include "region.h"

extern "C" {
  static void init(Handle<Object> target)
  {
    geoip::Country::Init(target);
    geoip::City::Init(target);
    geoip::Region::Init(target);

    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(geoip::addr_to_ipnum);
    target->Set(String::NewSymbol("addr_to_ipnum"), t->GetFunction());
    t = FunctionTemplate::New(geoip::addr_to_ipnum_v6);
    target->Set(String::NewSymbol("addr_to_ipnum_v6"), t->GetFunction());

  }

  NODE_MODULE(geoip, init);
} 
