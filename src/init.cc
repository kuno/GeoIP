#include "country.h"
#include "city.h"

extern "C" {
  static void init(Handle<Object> target)
  {
    Country::Init(target);
    City::Init(target);
  }

  NODE_MODULE(geoip, init);
} 
