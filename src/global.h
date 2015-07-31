/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */
#ifndef NODE_GEOIP_GLOBAL_H
#define NODE_GEOIP_GLOBAL_H

#include <v8.h>
#include "nan.h"

#define REQ_FUN_ARG(I, VAR)                                                    \
  if ((info.Length() + 1) <= (I) || !info[I]->IsFunction())                    \
    return Nan::ThrowTypeError("The second argument must be a function");      \
  Local<Function> VAR = Local<Function>::Cast(info[I]);

// Extracts a C string from a V8 Utf8Value.
extern const char* ToCString(const Nan::Utf8String& value);

#endif /* NODE_GEOIP_GLOBAL_H */
