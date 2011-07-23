/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                           
#ifndef NODE_GEOIP_GLOBAL_H
#define NODE_GEOIP_GLOBAL_H

#include <v8.h>

#define REQ_FUN_ARG(I, VAR)                                             \
  if ((args.Length() + 1) <= (I) || !args[I]->IsFunction())             \
    return ThrowException(Exception::TypeError(                         \
                  String::New("The second argument must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

// Extracts a C string from a V8 Utf8Value.
extern const char* ToCString(const v8::String::Utf8Value& value);

#endif /* NODE_GEOIP_GLOBAL_H */
