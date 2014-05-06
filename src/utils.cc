/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "utils.h"
#include "global.h"

namespace native {
    namespace utils {
        NAN_METHOD(isString) {
            NanScope();

            Local<Boolean> result = NanNew(args[0].As<Object>()->IsString() ? NanTrue() : NanFalse());

            NanReturnValue(result);
        }

        NAN_METHOD(check) {
            NanScope();

            Local<Value> edition = NanNew(NanNull());
            Local<String> file_str = args[0]->ToString();
            size_t size = file_str->Length() + 1;
            char file_cstr[size];
            size_t bc;
            NanCString(args[0], &bc, file_cstr, size);

            GeoIP *db = GeoIP_open(file_cstr, GEOIP_STANDARD);

            if (db) {
                int db_edition = GeoIP_database_edition(db);

                switch(db_edition) {
                    case GEOIP_COUNTRY_EDITION:
                        edition = NanNew<String>("country");
                        break;

                    case GEOIP_COUNTRY_EDITION_V6:
                        edition = NanNew<String>("country_v6");
                        break;

                    case GEOIP_CITY_EDITION_REV0:
                        edition = NanNew<String>("city");
                        break;

                    case GEOIP_CITY_EDITION_REV1:
                        edition = NanNew<String>("city");
                        break;

                    case GEOIP_CITY_EDITION_REV0_V6:
                        edition = NanNew<String>("city_v6");
                        break;

                    case GEOIP_CITY_EDITION_REV1_V6:
                        edition = NanNew<String>("city_v6");
                        break;

                    case GEOIP_REGION_EDITION_REV0:
                        edition = NanNew<String>("region");
                        break;

                    case GEOIP_REGION_EDITION_REV1:
                        edition = NanNew<String>("region");
                        break;

                    case GEOIP_ORG_EDITION:
                        edition = NanNew<String>("org");
                        break;

                    case GEOIP_ISP_EDITION:
                        edition = NanNew<String>("isp");
                        break;

                    case GEOIP_ASNUM_EDITION:
                        edition = NanNew<String>("asnum");
                        break;

                    case GEOIP_PROXY_EDITION:
                        edition = NanNew<String>("proxy");
                        break;

                    case GEOIP_NETSPEED_EDITION:
                        edition = NanNew<String>("netspeed");
                        break;

                    case GEOIP_NETSPEED_EDITION_REV1:
                        edition = NanNew<String>("netspeed cellular");
                        break;

                    case GEOIP_DOMAIN_EDITION:
                        edition = NanNew<String>("domain");
                        break;

                    default:
                        edition = NanNew<String>("unknown");
                }

                GeoIP_delete(db);
                db = NULL;

                NanReturnValue(edition);
            }

            return NanThrowError("Error: Cannot open database");
        }
    }
}
