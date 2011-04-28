/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */

#include <v8.h>
#include <GeoIP.h>
#include <GeoIPCity.h>

using namespace v8;

// Global references used for lookup, open, and close
GeoIP *gi;
int db_edition;

// For some reason this useful method is "private"!
extern "C" GEOIP_API unsigned long _GeoIP_lookupaddress(const char *host);

namespace geoip {
	// Pass this method an IP Address string and it will spit back coordinates
	Handle<Value> GeoIPLookup(const Arguments &args) {
		HandleScope scope;
		
		// Copy the argument strings into local cstrings
		// This is the only way I could figure out how to do this.
		// If anyone knows a better way, let me know.
		Local<String> ip_str = args[0]->ToString();
		char ip_cstr[ip_str->Length()];
		ip_str->WriteAscii(ip_cstr);		
		uint32_t ipnum = _GeoIP_lookupaddress(ip_cstr);
		if (ipnum == 0) {
			// On IP address error, return [0,0]
			Local<Array> coords = Array::New(2);
			coords->Set(Number::New(0), Number::New(0));
			coords->Set(Number::New(1), Number::New(0));
			return coords;
		}
		
		// Here I'm just returning the latitude, longitude
		// If you want to pull out city information, etc,
		//   this is the place to do it.
		GeoIPRecord *gir = GeoIP_record_by_ipnum(gi, ipnum);
		Local<Array> coords = Array::New(2);
		coords->Set(Number::New(0), Number::New(gir->latitude));
		coords->Set(Number::New(1), Number::New(gir->longitude));
		return coords;
	}
	
	// Create a GeoIP* reference with a cache boolean and a filename parameter
	Handle<Value> GeoIPOpen(const Arguments &args) {
		HandleScope scope;
		
		Local<String> path_str = args[1]->ToString();
		char path_cstr[path_str->Length()];
		path_str->WriteAscii(path_cstr);
		
		bool cache_on = args[0]->ToBoolean()->Value();
		
		gi = GeoIP_open(path_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);
		if (gi != NULL) {
			// Successfully opened the file, return 1 (true)
			db_edition = GeoIP_database_edition(gi);
			return Number::New(1);
		} else {
			// Error! return 0 (false)
			return Number::New(0);
		}
	}
	
	// Destroy the GeoIP* reference we're holding on to
	Handle<Value> GeoIPClose(const Arguments &args) {
		GeoIP_delete(gi);	// free()'s the gi reference & closes its fd
		gi = NULL;
		HandleScope scope;	// Stick this down here since it seems to segfault when on top?
	}
}

extern "C" void init(Handle<Object> target)
{
	HandleScope scope;

	// Add lookup, open, and close functions to GeoIP class
	Local<FunctionTemplate> t = FunctionTemplate::New(geoip::GeoIPLookup);
	target->Set(String::NewSymbol("lookup"), t->GetFunction());
	t = FunctionTemplate::New(geoip::GeoIPOpen);
	target->Set(String::NewSymbol("open"), t->GetFunction());
	t = FunctionTemplate::New(geoip::GeoIPClose);
	target->Set(String::NewSymbol("close"), t->GetFunction());
}
