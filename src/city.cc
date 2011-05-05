/* 
 *	geoip.cc - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */

#include <v8.h>
#include <node.h>

#include <GeoIP.h>
#include <GeoIPCity.h>

#include <unistd.h>

using namespace v8;
using namespace node;

#define REQ_FUN_ARG(I, VAR)                                             \
  if ((args.Length() + 1) <= (I) || !args[I]->IsFunction())                   \
    return ThrowException(Exception::TypeError(                         \
                  String::New("Argument " #I " must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);    

extern "C" GeoIPRecord * GeoIP_record_by_addr (GeoIP* gi, const char *addr);
extern "C" GEOIP_API unsigned char GeoIP_database_edition (GeoIP* gi); 
extern "C" GEOIP_API GeoIP* GeoIP_open(const char * filename, int flags); 
extern "C" GEOIP_API unsigned long _GeoIP_addr_to_num(const char *addr);

class City: ObjectWrap
{
  private:
    GeoIP *db;
    int db_edition;
  public:
    static Persistent<FunctionTemplate> s_ct;
    static void Init(Handle<Object> target)
    {
      HandleScope scope;

      Local<FunctionTemplate> t = FunctionTemplate::New(New);
      s_ct = Persistent<FunctionTemplate>::New(t);
      s_ct->InstanceTemplate()->SetInternalFieldCount(2);
      s_ct->SetClassName(String::NewSymbol("geoip"));

      NODE_SET_PROTOTYPE_METHOD(s_ct, "lookup", lookup);
      NODE_SET_PROTOTYPE_METHOD(s_ct, "lookupSync", lookupSync);
      target->Set(String::NewSymbol("City"), s_ct->GetFunction());
    }

      City() :
        db_edition(0)
      {
      }

      ~City()
        {
        }

      static Handle<Value> New(const Arguments& args)
      {
          HandleScope scope;
          City *c = new City();

          Local<String> path_str = args[0]->ToString();
          char path_cstr[path_str->Length()];
          path_str->WriteAscii(path_cstr);
          bool cache_on = args[1]->ToBoolean()->Value(); 

          c->db = GeoIP_open(path_cstr, cache_on?GEOIP_MEMORY_CACHE:GEOIP_STANDARD);

          if (c->db != NULL) {
            // Successfully opened the file, return 1 (true)
            c->db_edition = GeoIP_database_edition(c->db);
            c->Wrap(args.This());
            return args.This();
          } else {
            return ThrowException(v8::String::New("Error when open the database"));
          }
      }

      static Handle<Value> lookupSync(const Arguments &args) {
        HandleScope scope;

        Local<String> ip_str = args[0]->ToString();
        Local<Object> coords = Object::New();
        char ip_cstr[ip_str->Length()];
        ip_str->WriteAscii(ip_cstr);
        City* c = ObjectWrap::Unwrap<City>(args.This());

        // Here I'm just returning the latitude, longitude
        // If you want to pull out city information, etc,
        //   this is the place to do it.
        GeoIPRecord *gir = GeoIP_record_by_addr(c->db, ip_cstr);

        if (gir != NULL) {
          coords->Set(String::NewSymbol("latitude"), Number::New(gir->latitude));
          coords->Set(String::NewSymbol("longitude"), Number::New(gir->longitude));
          return scope.Close(coords);
        } else {
          return ThrowException(v8::String::New("Can not find out match data."));
        }
      }

      struct city_baton_t {
        City *c;
        char ip_cstr[17];  // standard length of ipv4
        GeoIPRecord *r;
        int increment_by;
        int sleep_for;
        Persistent<Function> cb;
      };

      struct city_baton_t_v6 {
        City *c;
        char ip_cstr[39]; // standard length of ipv6
        GeoIPRecord *r;
        int increment_by;
        int sleep_for;
        Persistent<Function> cb;
      };                        

      static Handle<Value> lookup(const Arguments& args)
      {
        HandleScope scope;

        REQ_FUN_ARG(1, cb);

        City *c = ObjectWrap::Unwrap<City>(args.This());
        Local<String> ip_str = args[0]->ToString();

        city_baton_t *baton = new city_baton_t();

        baton->c = c;
        ip_str->WriteAscii(baton->ip_cstr);
        baton->increment_by = 2;
        baton->sleep_for = 1;
        baton->cb = Persistent<Function>::New(cb);

        c->Ref();

        eio_custom(EIO_City, EIO_PRI_DEFAULT, EIO_AfterCity, baton);
        ev_ref(EV_DEFAULT_UC);

        return Undefined();
      }

      static int EIO_City(eio_req *req)
      {
        city_baton_t *baton = static_cast<city_baton_t *>(req->data);

        sleep(baton->sleep_for);

        baton->r = GeoIP_record_by_addr(baton->c->db, baton->ip_cstr);

        if (baton->r == NULL) {
           return 1;
        } else {
           return 0;
        }
      }

      static int EIO_AfterCity(eio_req *req)
      {
        HandleScope scope;

        city_baton_t *baton = static_cast<city_baton_t *>(req->data);
        ev_unref(EV_DEFAULT_UC);
        baton->c->Unref();

        Local<Value> argv[1];
        Local<Object> coords = Object::New();
        coords->Set(String::NewSymbol("latitude"), Number::New(baton->r->latitude));
        coords->Set(String::NewSymbol("longitude"), Number::New(baton->r->longitude));

        argv[0] = coords;

        TryCatch try_catch;

        baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);

        if (try_catch.HasCaught()) {
          FatalException(try_catch);
        }

        baton->cb.Dispose();

        delete baton;
        return 0;
      }

      /*
      // Destroy the GeoIP* reference we're holding on to
      Handle<Value> GeoIPClose(const Arguments &args) {
      GeoIP_delete(gi);	// free()'s the gi reference & closes its fd
      gi = NULL;
      HandleScope scope;	// Stick this down here since it seems to segfault when on top?
      }
      */
};

Persistent<FunctionTemplate> City::s_ct;

extern "C" {
  static void init(Handle<Object> target)
  {
    City::Init(target);
  }

  NODE_MODULE(city, init);
}
