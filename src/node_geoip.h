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

#include <GeoIP.h>
#include <GeoIPCity.h>

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

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// GeoIP Country //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
extern "C" GEOIP_API void GeoIP_setup_custom_directory(char *dir);
extern "C" GEOIP_API GeoIP* GeoIP_open_type (int type, int flags);
extern "C" GEOIP_API GeoIP* GeoIP_new(int flags);
extern "C" GEOIP_API GeoIP* GeoIP_open(const char * filename, int flags);
extern "C" GEOIP_API int GeoIP_db_avail(int type);
extern "C" GEOIP_API void GeoIP_delete(GeoIP* gi);
extern "C" GEOIP_API const char *GeoIP_country_code_by_addr (GeoIP* gi, const char *addr);
extern "C" GEOIP_API const char *GeoIP_country_code_by_name (GeoIP* gi, const char *host);
extern "C" GEOIP_API const char *GeoIP_country_code3_by_addr (GeoIP* gi, const char *addr);
extern "C" GEOIP_API const char *GeoIP_country_code3_by_name (GeoIP* gi, const char *host);
extern "C" GEOIP_API const char *GeoIP_country_name_by_addr (GeoIP* gi, const char *addr);
extern "C" GEOIP_API const char *GeoIP_country_name_by_name (GeoIP* gi, const char *host);
extern "C" GEOIP_API const char *GeoIP_country_name_by_ipnum (GeoIP* gi, unsigned long ipnum);
extern "C" GEOIP_API const char *GeoIP_country_code_by_ipnum (GeoIP* gi, unsigned long ipnum);
extern "C" GEOIP_API const char *GeoIP_country_code3_by_ipnum (GeoIP* gi, unsigned long ipnum);

/* */
extern "C" GEOIP_API const char *GeoIP_country_name_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);
extern "C" GEOIP_API const char *GeoIP_country_code_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);
extern "C" GEOIP_API const char *GeoIP_country_code3_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);

extern "C" GEOIP_API const char *GeoIP_country_code_by_addr_v6 (GeoIP* gi, const char *addr);
extern "C" GEOIP_API const char *GeoIP_country_code_by_name_v6 (GeoIP* gi, const char *host);
extern "C" GEOIP_API const char *GeoIP_country_code3_by_addr_v6 (GeoIP* gi, const char *addr);
extern "C" GEOIP_API const char *GeoIP_country_code3_by_name_v6 (GeoIP* gi, const char *host);
extern "C" GEOIP_API const char *GeoIP_country_name_by_addr_v6 (GeoIP* gi, const char *addr);
extern "C" GEOIP_API const char *GeoIP_country_name_by_name_v6 (GeoIP* gi, const char *host);


extern "C" GEOIP_API char *GeoIP_org_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);
extern "C" GEOIP_API char *GeoIP_org_by_addr_v6 (GeoIP* gi, const char *addr);
extern "C" GEOIP_API char *GeoIP_org_by_name_v6 (GeoIP* gi, const char *name);

/* End deprecated */

extern "C" GEOIP_API int GeoIP_id_by_addr (GeoIP* gi, const char *addr);
extern "C" GEOIP_API int GeoIP_id_by_name (GeoIP* gi, const char *host);
extern "C" GEOIP_API int GeoIP_id_by_ipnum (GeoIP* gi, unsigned long ipnum);

extern "C" GEOIP_API int GeoIP_id_by_addr_v6 (GeoIP* gi, const char *addr);
extern "C" GEOIP_API int GeoIP_id_by_name_v6 (GeoIP* gi, const char *host);
extern "C" GEOIP_API int GeoIP_id_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);

extern "C" GEOIP_API GeoIPRegion * GeoIP_region_by_addr (GeoIP* gi, const char *addr);
extern "C" GEOIP_API GeoIPRegion * GeoIP_region_by_name (GeoIP* gi, const char *host);
extern "C" GEOIP_API GeoIPRegion * GeoIP_region_by_ipnum (GeoIP *gi, unsigned long ipnum);

extern "C" GEOIP_API GeoIPRegion * GeoIP_region_by_addr_v6 (GeoIP* gi, const char *addr);
extern "C" GEOIP_API GeoIPRegion * GeoIP_region_by_name_v6 (GeoIP* gi, const char *host);
extern "C" GEOIP_API GeoIPRegion * GeoIP_region_by_ipnum_v6 (GeoIP *gi, geoipv6_t ipnum);

/* Warning - don't call this after GeoIP_assign_region_by_inetaddr calls */
extern "C" GEOIP_API void GeoIPRegion_delete (GeoIPRegion *gir);

extern "C" GEOIP_API void GeoIP_assign_region_by_inetaddr(GeoIP* gi, unsigned long inetaddr, GeoIPRegion *gir);

extern "C" GEOIP_API void GeoIP_assign_region_by_inetaddr_v6(GeoIP* gi, geoipv6_t inetaddr, GeoIPRegion *gir);

/* Used to query GeoIP Organization, ISP and AS Number databases */
extern "C" GEOIP_API char *GeoIP_name_by_ipnum (GeoIP* gi, unsigned long ipnum);
extern "C" GEOIP_API char *GeoIP_name_by_addr (GeoIP* gi, const char *addr);
extern "C" GEOIP_API char *GeoIP_name_by_name (GeoIP* gi, const char *host);

extern "C" GEOIP_API char *GeoIP_name_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);
extern "C" GEOIP_API char *GeoIP_name_by_addr_v6 (GeoIP* gi, const char *addr);
extern "C" GEOIP_API char *GeoIP_name_by_name_v6 (GeoIP* gi, const char *name);

/** return two letter country code */
extern "C" GEOIP_API const char* GeoIP_code_by_id(int id);

/** return three letter country code */
extern "C" GEOIP_API const char* GeoIP_code3_by_id(int id);

/** return full name of country in utf8 or iso-8859-1 */
extern "C" GEOIP_API const char* GeoIP_country_name_by_id(GeoIP* gi, int id);

/** return full name of country */
extern "C" GEOIP_API const char* GeoIP_name_by_id(int id);

/** return continent of country */
extern "C" GEOIP_API const char* GeoIP_continent_by_id(int id);

/** return id by country code **/
extern "C" GEOIP_API int GeoIP_id_by_code(const char *country);

/** return return number of known countries */
extern "C" GEOIP_API unsigned GeoIP_num_countries(void);

extern "C" GEOIP_API char *GeoIP_database_info (GeoIP* gi);
extern "C" GEOIP_API unsigned char GeoIP_database_edition (GeoIP* gi);

extern "C" GEOIP_API int GeoIP_charset (GeoIP* gi);
extern "C" GEOIP_API int GeoIP_set_charset (GeoIP* gi, int charset);
extern "C" GEOIP_API int GeoIP_enable_teredo (GeoIP* gi, int true_false );
extern "C" GEOIP_API int GeoIP_teredo (GeoIP* gi );

extern "C" GEOIP_API int GeoIP_last_netmask (GeoIP* gi);
extern "C" GEOIP_API char **GeoIP_range_by_ip (GeoIP* gi, const char *addr);
extern "C" GEOIP_API void GeoIP_range_by_ip_delete(char **ptr);

/* Convert region code to region name */
extern "C" GEOIP_API const char * GeoIP_region_name_by_code(const char *country_code, const char *region_code);

/* Get timezone from country and region code */
extern "C" GEOIP_API const char * GeoIP_time_zone_by_country_and_region(const char *country_code, const char *region_code);

/* some v4 helper functions as of 1.4.7 exported to the public API */
extern "C" GEOIP_API unsigned long GeoIP_addr_to_num(const char *addr);
extern "C" GEOIP_API char *        GeoIP_num_to_addr(unsigned long ipnum);

/* Internal function -- convert iso to utf8; return a malloced utf8 string. */
char * _GeoIP_iso_8859_1__utf8(const char * iso);

/* Cleans up memory used to hold file name paths. Returns 1 if successful; otherwise 0. 
 * */
extern "C" GEOIP_API int GeoIP_cleanup(void);


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// GeoIP City ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
extern "C" GeoIPRecord * GeoIP_record_by_addr (GeoIP * gi, const char * addr);
extern "C" GeoIPRecord * GeoIP_record_by_name (GeoIP * gi, const char * host);    

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
