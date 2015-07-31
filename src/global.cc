/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */

#include "global.h"

// Extracts a C string from a Nan::Utf8String.
const char * ToCString(const Nan::Utf8String& value) {
  return *value ? *value : "<string conversion failed>";
}
