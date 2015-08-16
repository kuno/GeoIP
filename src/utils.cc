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
            Nan::HandleScope scope;

            //Local<Boolean> result = Nan::New(info[0].As<Object>()->IsString() ? Nan::True() : Nan::False());

            info.GetReturnValue().Set(info[0].As<Object>()->IsString() ? Nan::True() : Nan::False());
        }

        NAN_METHOD(check) {
            Nan::HandleScope scope;

            //Local<Value> edition = Nan::New(Nan::Null());
            String::Utf8Value file_str(info[0]->ToString());
            //size_t size = file_str->Length() + 1;
            //char file_cstr[size];
            //size_t bc;
            //NanCString(info[0], &bc, file_cstr, size);

            const char * file_cstr = ToCString(file_str);
            GeoIP *db = GeoIP_open(file_cstr, GEOIP_STANDARD);

            if (db) {
                int db_edition = GeoIP_database_edition(db);

                switch(db_edition) {
                    case GEOIP_COUNTRY_EDITION:
                        info.GetReturnValue().Set(Nan::New<String>("country").ToLocalChecked());
                        break;

                    case GEOIP_COUNTRY_EDITION_V6:
                        info.GetReturnValue().Set(Nan::New<String>("country_v6").ToLocalChecked());
                        break;

                    case GEOIP_CITY_EDITION_REV0:
                        info.GetReturnValue().Set(Nan::New<String>("city").ToLocalChecked());
                        break;

                    case GEOIP_CITY_EDITION_REV1:
                        info.GetReturnValue().Set(Nan::New<String>("city").ToLocalChecked());
                        break;

                    case GEOIP_CITY_EDITION_REV0_V6:
                        info.GetReturnValue().Set(Nan::New<String>("city_v6").ToLocalChecked());
                        break;

                    case GEOIP_CITY_EDITION_REV1_V6:
                        info.GetReturnValue().Set(Nan::New<String>("city_v6").ToLocalChecked());
                        break;

                    case GEOIP_REGION_EDITION_REV0:
                        info.GetReturnValue().Set(Nan::New<String>("region").ToLocalChecked());
                        break;

                    case GEOIP_REGION_EDITION_REV1:
                        info.GetReturnValue().Set(Nan::New<String>("region").ToLocalChecked());
                        break;

                    case GEOIP_ORG_EDITION:
                        info.GetReturnValue().Set(Nan::New<String>("org").ToLocalChecked());
                        break;

                    case GEOIP_ISP_EDITION:
                        info.GetReturnValue().Set(Nan::New<String>("isp").ToLocalChecked());
                        break;

                    case GEOIP_ASNUM_EDITION:
                        info.GetReturnValue().Set(Nan::New<String>("asnum").ToLocalChecked());
                        break;

                    case GEOIP_PROXY_EDITION:
                        info.GetReturnValue().Set(Nan::New<String>("proxy").ToLocalChecked());
                        break;

                    case GEOIP_NETSPEED_EDITION:
                        info.GetReturnValue().Set(Nan::New<String>("netspeed").ToLocalChecked());
                        break;

                    case GEOIP_NETSPEED_EDITION_REV1:
                        info.GetReturnValue().Set(Nan::New<String>("netspeed cellular").ToLocalChecked());
                        break;

                    case GEOIP_DOMAIN_EDITION:
                        info.GetReturnValue().Set(Nan::New<String>("domain").ToLocalChecked());
                        break;

                    default:
                        info.GetReturnValue().Set(Nan::New<String>("unknown").ToLocalChecked());
                }

                GeoIP_delete(db);
                db = NULL;

            } else {
              return Nan::ThrowError("Error: Cannot open database");
            }
        }
    }
}
