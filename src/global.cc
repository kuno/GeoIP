/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */                                           

#include "global.h"

// Extracts a C string from a V8 Utf8Value.
const char * ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}
