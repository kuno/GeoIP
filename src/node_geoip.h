/*
 * =====================================================================================
 *
 *       Filename:  node_geoip.h
 *
 *    Description:  j   
 *
 *        Version:  1.0
 *        Created:  2011年05月05日 18时46分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef NODE_GEOIP_H
#define NODE_GEOIP_H

#ifdef __cplusplus
extern "C" {
#endif

#define REQ_FUN_ARG(I, VAR)                                             \
  if ((args.Length() + 1) <= (I) || !args[I]->IsFunction())             \
    return ThrowException(Exception::TypeError(                         \
                  String::New("Argument " #I " must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);                  

bool isIP (char * string) {
  bool isIP = true;

}
/*
struct city_baton_t {
  City *c;
  char ip_cstr[256];  // standard length of ipv4
  GeoIPRecord *r;
  int increment_by;
  int sleep_for;
  Persistent<Function> cb;
};

struct city_baton_t_v6 {
  City *c;
  char ip_cstr[256]; // standard length of ipv6
  GeoIPRecord *r;
  int increment_by;
  int sleep_for;
  Persistent<Function> cb;
};

*/
#ifdef __cplusplus
}
#endif

#endif /* NODE_GEOIP_H */
