var dns = require('dns'),
CONST = Object.freeze(require('../include/constants.js')),
seekCountry = require('./utils.js').seekCountry,
ipToLong = require('./utils.js').ipToLong;

/******************************************************************************
*  Inner Functions
*****************************************************************************/
function __getRegion__(data,ipnum){
  var region, region_id, country_code, char0, char1;
  if (data.db_type == CONST.REGION_EDITION_REV0){
    region_id = seekCountry(data, ipnum);
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
    region_id = seekCountry(data,ipnum);
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
var region_by_addr = function(data, addr) {
  var ipnum, err, region;
  if (data.db_type !== CONST.REGION_EDITION_REV0 && 
    data.db_type !== CONST.REGION_EDITION_REV1) {
    err = new Error('Err: Not Region Data');
    throw err;
  }
  ipnum = ipToLong(addr);
  region = __getRegion__(data, ipnum);
  if (!region) {
    return null;
  } else {
    return region;
  }
};

// Asynchronous method
var region_by_domain = function(data, domain, callback) {
  var region, err, ipnum, seek;
  if (data.db_type !== CONST.REGION_EDITION_REV0 &&
    data.db_type !== CONST.REGION_EDITION_REV1) {
    err = new Error('Not Region Data');
    callback(err, region);
  } else {
    dns.resolve4(domain, function(error, addresses) {
      if (error) {throw error;}
      ipnum = ipToLong(addresses[0]);
      seek = __getRegion__(data, ipnum);
      if (!seek) {
        region = null;
      } else {
        region = seek;
      }
      callback(err, region);
    });
  }
};

region_by_addr._usage = 'Region.region_by_addr(data, <IP>)';
region_by_domain._usage = 'Region.region_by_domain(data, <DOMAIN>, <CALLBACK>)';

exports.region_by_addr = region_by_addr;
exports.region_by_domain = region_by_domain;
