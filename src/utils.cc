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
            Local<Boolean> result = Nan::New(info[0].As<Object>()->IsString() ? Nan::True() : Nan::False());

            info.GetReturnValue().Set(result);
        }

        NAN_METHOD(check) {
            Local<Value> edition = Nan::New(Nan::Null());
            Local<String> file_str = info[0]->ToString();
            Nan::Utf8String file_s(file_str);

            GeoIP *db = GeoIP_open(*file_s, GEOIP_STANDARD);

            if (db) {
                int db_edition = GeoIP_database_edition(db);

                switch(db_edition) {
                    case GEOIP_COUNTRY_EDITION:
                        edition = Nan::New<String>("country").ToLocalChecked();
                        break;

                    case GEOIP_COUNTRY_EDITION_V6:
                        edition = Nan::New<String>("country_v6").ToLocalChecked();
                        break;

                    case GEOIP_CITY_EDITION_REV0:
                        edition = Nan::New<String>("city").ToLocalChecked();
                        break;

                    case GEOIP_CITY_EDITION_REV1:
                        edition = Nan::New<String>("city").ToLocalChecked();
                        break;

                    case GEOIP_CITY_EDITION_REV0_V6:
                        edition = Nan::New<String>("city_v6").ToLocalChecked();
                        break;

                    case GEOIP_CITY_EDITION_REV1_V6:
                        edition = Nan::New<String>("city_v6").ToLocalChecked();
                        break;

                    case GEOIP_REGION_EDITION_REV0:
                        edition = Nan::New<String>("region").ToLocalChecked();
                        break;

                    case GEOIP_REGION_EDITION_REV1:
                        edition = Nan::New<String>("region").ToLocalChecked();
                        break;

                    case GEOIP_ORG_EDITION:
                        edition = Nan::New<String>("org").ToLocalChecked();
                        break;

                    case GEOIP_ISP_EDITION:
                        edition = Nan::New<String>("isp").ToLocalChecked();
                        break;

                    case GEOIP_ASNUM_EDITION:
                        edition = Nan::New<String>("asnum").ToLocalChecked();
                        break;

                    case GEOIP_PROXY_EDITION:
                        edition = Nan::New<String>("proxy").ToLocalChecked();
                        break;

                    case GEOIP_NETSPEED_EDITION:
                        edition = Nan::New<String>("netspeed").ToLocalChecked();
                        break;

                    case GEOIP_NETSPEED_EDITION_REV1:
                        edition = Nan::New<String>("netspeed cellular").ToLocalChecked();
                        break;

                    case GEOIP_DOMAIN_EDITION:
                        edition = Nan::New<String>("domain").ToLocalChecked();
                        break;

                    default:
                        edition = Nan::New<String>("unknown").ToLocalChecked();
                }

                GeoIP_delete(db);
                db = NULL;

                info.GetReturnValue().Set(edition);
                return;
            }

            return Nan::ThrowError("Error: Cannot open database");
        }
    }
}
