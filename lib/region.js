var CONST = require('../include/constants.js'),
seek_country = require('./utils.js').seek_country,
ip2long = require('./utils.js').ip2long;

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

exports.region_by_addr = function(data, addr) {
  var ipnum;
  if (data.db_type !== CONST.REGION_EDITION_REV0 && 
  data.db_type !== CONST.REGION_EDITION_REV1) {
    return 'Err: Not Region Data';
  } else {
    if (addr === null) {
      return 0;
    }
    ipnum = ip2long(addr);
    return _get_region(data, ipnum);
  }
};   
