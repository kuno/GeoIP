/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                           
#ifndef NODE_GEOIP_GLOBAL_H
#define NODE_GEOIP_GLOBAL_H

#include <v8.h>
#include <iconv.h>

static iconv_t cd;

#ifndef ICONV_SRC_CONST
#define ICONV_SRC_CONST
#endif

#define icv(a,b,blen) do { \
  ICONV_SRC_CONST char *in = a; \
  char *out = b; \
  size_t inlen = strlen(a); \
  size_t outlen = blen; \
  if(iconv(cd, &in, &inlen, &out, &outlen) == -1) b[0] = '\0'; \
  else *out = '\0'; \
} while(0)

#define REQ_FUN_ARG(I, VAR)                                             \
  if ((args.Length() + 1) <= (I) || !args[I]->IsFunction())             \
    return ThrowException(Exception::TypeError(                         \
                  String::New("The second argument must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

// Extracts a C string from a V8 Utf8Value.
extern const char* ToCString(const v8::String::Utf8Value& value);

#endif /* NODE_GEOIP_GLOBAL_H */
