var dns = require('dns'),
CONST = Object.freeze(require('../include/constants.js')),
seek_country = require('./utils.js').seek_country,
ip2long = require('./utils.js').ip2long;

/******************************************************************************
 *  Inner Functions
 *****************************************************************************/
 function _get_region(data,ipnum){
   var region, region_id, country_code, char0, char1;
   if (data.db_type == CONST.REGION_EDITION_REV0){
     region_id = seek_country(data, ipnum) - CONST.STATE_BEGIN_REV0;
     if (region_id >= 1000){
       country_code = "US";
       char0 = String.fromCharCode((region_id - 1000) / 26 + 65);
       char1 = String.fromCharCode((region_id - 1000) % 26 + 65);
       region = char0.concat(char1);
     } else {
       country_code = data.COUNTRY_CODES[region_id];
       region = "";
     }
     return [country_code, region];
   }  else if (data.db_type == CONST.REGION_EDITION_REV1) {
     region_id = seek_country(data,ipnum) - CONST.STATE_BEGIN_REV1;
     //print $seek_region;
     if (region_id < CONST.US_OFFSET){
       country_code = "";
       region = "";  
     } else if (region_id < CONST.CANADA_OFFSET) {
       country_code = "US";
       char0 = String.fromCharCode((region_id - CONST.US_OFFSET) / 26 + 65);
       char1 = String.fromCharCode((region_id - CONST.US_OFFSET) % 26 + 65);
       region = char0.concat(char1);
     } else if (region_id < CONST.WORLD_OFFSET) {
       country_code = "CA";
       char0 = String.fromCharCode((region_id - CONST.CANADA_OFFSET) / 26 + 65);
       char1 = String.fromCharCode((region_id - CONST.CANADA_OFFSET) % 26 + 65);
       region = char0.concat(char1);
     } else {
       country_code = data.COUNTRY_CODES[(region_id - CONST.WORLD_OFFSET) / CONST.FIPS_RANGE];
       region = "";
     }
     return [country_code, region];
   }
 }

 /******************************************************************************
  * Export Functions
  *****************************************************************************/
  // Synchronous method
  exports.region_by_addr = function(data, addr) {
    var ipnum, err, region;
    if (data.db_type !== CONST.REGION_EDITION_REV0 && 
    data.db_type !== CONST.REGION_EDITION_REV1) {
      err = new Error('Err: Not Region Data');
      throw err;
    }
    ipnum = ip2long(addr);
    region = _get_region(data, ipnum);
    if (!region) {
      return 'Err: Region Not Found';
    } else {
      return region;
    }
  };

  // Asynchronous method
  exports.region_by_domain = function(data, domain, callback) {
    var region, err, ipnum, seek;
    if (data.db_type !== CONST.REGION_EDITION_REV0 &&
    data.db_type !== CONST.REGION_EDITION_REV1) {
      err = new Error('Not Region Data');
      callback(err, region);
    } else {
      dns.resolve(domain, rrtype='A', function(error, addresses) {
        if (error) {throw error;}
        ipnum = ip2long(addresses[0]);
        seek = _get_region(data, ipnum);
        if (!seek) {
          region = 'Err: Region Not Found';
        } else {
          region = seek;
        }
        callback(err, region);
      });
    }
  };
