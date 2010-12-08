var dns = require('dns'),
city = require('./city.js'),
ip2long = require('./utils.js').ip2long,
seek_country = require('./utils.js').seek_country,
CONST = require('../include/constants.js');    

function _id_by_addr(data, addr) {
  if (addr !== false) {
    var ipnum = ip2long(addr);
    var country_id = seek_country(data, ipnum) - CONST.COUNTRY_BEGIN;
    return country_id;
  }
  return false;
}   

function _id_by_domain(data, domain, callback) {
  var country_id;
  var ipnum;
  dns.resolve(domain, rrtype='A', function(err, addresses) {
    if (err) {throw err;}
    ipnum = ip2long(addresses[0]);
    country_id = seek_country(data, ipnum) - CONST.COUNTRY_BEGIN;
    if (country_id !== false) {
      callback(null, country_id);
    }
  });
}

exports.code_by_domain = function(data, domain, callback) {
  var code;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
  data.db_type !== CONST.CITY_EDITION_REV0 &&
  data.db_type !== CONST.CITY_EDITION_REV1) {
    return 'Err: Unmatch data type';
  } else {
    _id_by_domain(data, domain, function(err, country_id) {
      if (err) {throw err;}
      if (country_id !== false) {
        code = data.COUNTRY_CODES[country_id];
        callback(null, code);
      }
    });
  }
};

exports.name_by_domain = function(data, domain, callback) {
  var country_name;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
  data.db_type !== CONST.CITY_EDITION_REV0 &&
  data.db_type !== CONST.CITY_EDITION_REV1) {
    return 'Err: Unmatch data type';
  } else {  
    _id_by_domain(data, domain, function(err, country_id) {
      if (err) {throw err;}
      if (country_id !== false) {
        country_name = data.COUNTRY_NAMES[country_id];
        callback(null, country_name);
      }
    });
  }
};

exports.code_by_addr = function(data, addr) {
  var country_id, record;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
  data.db_type !== CONST.CITY_EDITION_REV0 &&
  data.db_type !== CONST.CITY_EDITION_REV1) {
    return 'Err: Unmatch data type';
  } else if (data.db_type === CONST.CITY_EDITION_REV1) {
    record = city.record_by_addr(data, addr);
    return record.country_code;
  } else {
    country_id = _id_by_addr(data, addr);
    if (country_id !== false) {
      return data.COUNTRY_CODES[country_id];
    }
  }

};

exports.name_by_addr = function(data, addr) {
  var record, country_id;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
  data.db_type !== CONST.CITY_EDITION_REV0 &&
  data.db_type !== CONST.CITY_EDITION_REV1) {
    return 'Err: Unmatch data type';
  } else if (data.db_type === CONST.CITY_EDITION_REV1) {
    record = city.record_by_addr(data, addr);
    return record.country_name;
  } else {
    country_id = _id_by_addr(data, addr);
    if (country_id !== false) {
      return data.COUNTRY_NAMES[country_id];
    }
  }
  return false;
};
