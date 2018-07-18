/*
 * GeoIP C library binding for nodejs
 *
 * Licensed under the GNU LGPL 2.1 license
 */
#ifndef NODE_GEOIP_GLOBAL_H
#define NODE_GEOIP_GLOBAL_H

#include "nan.h"

#define REQ_FUN_ARG(I, VAR)                                             \
  if ((args.Length() + 1) <= (I) || !args[I]->IsFunction())             \
    return NanThrowTypeError("The second argument must be a function"); \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

#endif /* NODE_GEOIP_GLOBAL_H */
