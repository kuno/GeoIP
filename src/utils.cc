/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                          

#include "utils.h"
#include "global.h"

namespace geoip {
  Handle<Value> check(const Arguments &args) {
    HandleScope scope;

    Local<String> edition;
    Local<String> file_str = args[0]->ToString();
    char file_cstr[file_str->Length()];
    file_str->WriteAscii(file_cstr);

    GeoIP * db = GeoIP_open(file_cstr, GEOIP_STANDARD);

    if (db != NULL) {
      int db_edition = GeoIP_database_edition(db);

      switch(db_edition) {
        case GEOIP_COUNTRY_EDITION:
          edition = String::New("country");
          break;

        case GEOIP_COUNTRY_EDITION_V6:
          edition = String::New("country_v6");
          break;

        case GEOIP_CITY_EDITION_REV0:
          edition = String::New("city");
          break;

        case GEOIP_CITY_EDITION_REV1:
          edition = String::New("city");
          break;

       case GEOIP_CITY_EDITION_REV0_V6:
          edition = String::New("city_v6");
          break;

        case GEOIP_CITY_EDITION_REV1_V6:
          edition = String::New("city_v6");
          break;

        case GEOIP_REGION_EDITION_REV0:
          edition = String::New("region");
          break;

        case GEOIP_REGION_EDITION_REV1:
          edition = String::New("region");
          break;

        case GEOIP_ORG_EDITION:
          edition = String::New("org");
          break;

        case GEOIP_ISP_EDITION:
          edition = String::New("isp");
          break;

        case GEOIP_ASNUM_EDITION:
          edition = String::New("asnum");
          break;

        case GEOIP_PROXY_EDITION:
          edition = String::New("proxy");
          break;

        case GEOIP_NETSPEED_EDITION:
          edition = String::New("netspeed");
          break;

        case GEOIP_DOMAIN_EDITION:
          edition = String::New("domain");
          break;

        default:
          edition = String::New("unknown");
      }

      GeoIP_delete(db);
      db = NULL;

      return scope.Close(edition);
    }

    return ThrowException(String::New("Error: Cao not open database"));   
  }

}
