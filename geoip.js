var fs = require('fs'),
path = require('path'),
buf2long = require('./lib/utils.js').buf2long;
GEOIPDATA = require('./lib/geoipdata.js').GEOIPDATA;

/******************************************************************************
 * Constans
 *****************************************************************************/
 var GEOIP_COUNTRY_BEGIN = 16776960;
 var GEOIP_STATE_BEGIN_REV0 = 16700000;
 var GEOIP_STATE_BEGIN_REV1 = 16000000;
 var GEOIP_STANDARD = 0;
 var GEOIP_MEMORY_CACHE = 1;
 var GEOIP_SHARED_MEMORY = 2;
 var STRUCTURE_INFO_MAX_SIZE = 20;
 var DATABASE_INFO_MAX_SIZE = 100;
 var GEOIP_COUNTRY_EDITION = 106;
 var GEOIP_PROXY_EDITION = 8;
 var GEOIP_ASNUM_EDITION = 9;
 var GEOIP_NETSPEED_EDITION = 10;
 var GEOIP_REGION_EDITION_REV0 = 112;
 var GEOIP_REGION_EDITION_REV1 = 3;
 var GEOIP_CITY_EDITION_REV0 = 111;
 var GEOIP_CITY_EDITION_REV1 = 2;
 var GEOIP_ORG_EDITION = 110;
 var GEOIP_ISP_EDITION = 4;
 var SEGMENT_RECORD_LENGTH = 3;
 var STANDARD_RECORD_LENGTH = 3;
 var ORG_RECORD_LENGTH = 4;
 var MAX_RECORD_LENGTH = 4;
 var MAX_ORG_RECORD_LENGTH = 300;
 var US_OFFSET = 1;
 var CANADA_OFFSET = 677;
 var WORLD_OFFSET = 1353;
 var FIPS_RANGE = 360;
 var GEOIP_UNKNOWN_SPEED = 0;
 var GEOIP_DIALUP_SPEED = 1;
 var GEOIP_CABLEDSL_SPEED = 2;
 var GEOIP_CORPORATE_SPEED = 3;
 var GEOIP_DOMAIN_EDITION = 11;
 var GEOIP_LOCATIONA_EDITION = 13;
 var GEOIP_ACCURACYRADIUS_EDITION = 14;
 var GEOIP_CITYCOMBINED_EDITION = 15;
 var CITYCOMBINED_FIXED_RECORD = 7;
 var Delimter_NUMBER = 16777215;


 var _setup_segments = function(data) {
     var offset;
     data.databaseType = GEOIP_COUNTRY_EDITION;
     data.record_length = STANDARD_RECORD_LENGTH;
     var buf = data.buffer;
     offset = buf.length - 3;
     for (var i = 0; i < STRUCTURE_INFO_MAX_SIZE; i++) {
         var delim = buf2long(buf.slice(offset, offset + 3));
         offset += 3;
         if (delim === Delimter_NUMBER) {
             data.databaseType = parseInt(buf[offset], 10);
             offset++;
             if (data.databaseType === GEOIP_REGION_EDITION_REV0) {
                 data.databaseSegments = GEOIP_STATE_BEGIN_REV0;
             } else if (data.databaseType === GEOIP_REGION_EDITION_REV1) {
                 data.databaseSegments = GEOIP_STATE_BEGIN_REV1;
             } else if (data.databaseType === GEOIP_CITY_EDITION_REV0 ||
             data.databaseType === GEOIP_CITY_EDITION_REV1 ||
             data.databaseType === GEOIP_CITY_ORG_EDITION  ||
             data.databaseType === GEOIP_CITY_DOMAIN_EDITION ||
             data.databaseType === GEOIP_CITY_ISP_EDITION    ||
             data.databaseType === GEOIP_LOCATIONA_EDITION   ||
             data.databaseType === GEOIP_ACCURACYRADIUS_EDITION ||
             data.databaseType === GEIOP_ASNUM_EDITION) {
                 data.databaseSegments = 0;
                 var seg = buf.slice(offset, offset+SEGMENT_RECORD_LENGTH);
                 for (var j = 0; j < SEGMENT_RECORD_LENGTH; j++) {
                     data.databaseSegments += (parseInt(seg[j], 10) << (j * 8));
                 }
                 if (data.databaseType === GEOIP_ORG_EDITION ||
                 data.databaseType === GEOIP_DOMAIN_EDITION ||
                 data.databaseType === GEOIP_ISP_EDITION) {
                     data.record_length = ORG_RECORD_LENGTH;
                 }
             }
             break;
         } else {
             offset -= 4;
         }
     }

     if (data.databaseType === GEOIP_COUNTRY_EDITION ||
     data.databaseType === GEOIP_PROXY_EDITION   ||
     data.databaseType === GEOIP_NETSPEED_EDITION) {
         data.databaseSegments = GEOIP_COUNTRY_BEGIN;
     }

     return data;
 };


 /******************************************************************************
  * Exprots Functions
  *****************************************************************************/
  exports.open = function(file) {
      var f = file || path.join(__dirname, 'data/GeoIP.dat');

      var data = new GEOIPDATA();

      fs.open(f, 'r', function(err, fd) {
          if (err) { throw err;}
          fs.fstat(fd, function(err, stats) {
              if (err) { throw err;}
              data.fileDescriptor = fd;
              var buf = new Buffer(stats.size);
              fs.read(fd, buf, 0, stats.size, 0, function(err, bytesRead) {
                  if (err) { throw err; }
                  data.buffer = buf;
                  data = _setup_segments(data);
              }
          );
      }
  );
   });

   return data;

   };

   exports.close = function(data) {
       return fs.close(data.fileDescriptor);
   };

   exports.country = require('./country.js');
   exports.city = require('./city.js');
