/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */

#include "utils.h"

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

      if (db_edition == GEOIP_COUNTRY_EDITION) {
        edition = String::New("country");
      } else if (db_edition == GEOIP_COUNTRY_EDITION_V6) {
        edition = String::New("country_v6");
      } else if ( db_edition == GEOIP_CITY_EDITION_REV0 || 
          db_edition == GEOIP_CITY_EDITION_REV1) {
        edition = String::New("city");
      } else if (db_edition == GEOIP_REGION_EDITION_REV0 || 
          db_edition == GEOIP_REGION_EDITION_REV1) {
        edition = String::New("region");
      } else if (db_edition == GEOIP_ORG_EDITION) {
        edition = String::New("org");
      } else if (db_edition == GEOIP_ISP_EDITION) {
        edition = String::New("isp");
      } else if (db_edition == GEOIP_ASNUM_EDITION) {
        edition = String::New("asnum");
      } else if (db_edition == GEOIP_PROXY_EDITION) {
        edition = String::New("proxy");
      } else if (db_edition == GEOIP_NETSPEED_EDITION) {
        edition = String::New("netspeed");
      } else if (db_edition == GEOIP_DOMAIN_EDITION) {
        edition = String::New("domain");
      } else {
        edition = String::New("unknown");
      }

      GeoIP_delete(db);
      db = NULL;

      return scope.Close(edition);
    }

    return ThrowException(String::New("Error: Cao not open database"));   
  }
}
