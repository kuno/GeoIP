var fs = require('fs'),
dns      = require('dns'),
city         = require('./city.js'),
ipToLong      = require('./utils.js').ipToLong,
seekCountry = require('./utils.js').seekCountry,
CONST        = Object.freeze(require('../include/constants.js'));

/******************************************************************************
*  Inter Functions
*****************************************************************************/
function __idByaddr__(data, addr) {
  if (addr !== false) {
    var ipnum = ipToLong(addr);
    var country_id = seekCountry(data, ipnum) - CONST.COUNTRY_BEGIN;
    return country_id;
  }
  return false;
}   

function __idBydomain__(data, domain, callback) {
  var country_id;
  var ipnum;
  dns.resolve4(domain, function(err, addresses) {
    if (err) {throw err;}
    ipnum = ipToLong(addresses[0]);
    country_id = seekCountry(data, ipnum) - CONST.COUNTRY_BEGIN;
    if (country_id !== false) {
      callback(null, country_id);
    }
  });
}

/******************************************************************************
*  Export Functions
******************************************************************************/
// Asynchronous method
var codeByDomain = function(data, domain, callback) {
  var code, err;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
    data.db_type !== CONST.CITY_EDITION_REV0 &&
    data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Err: Not Country or City Data');
    callback(err, code);
  } else {
    __idBydomain__(data, domain, function(error, country_id) {
      if (error) {throw error;}
      if (!country_id) {
        code = null;
        //err = new Error('Err: Country Code Not Found');
      } else {
        code = data.COUNTRY_CODES[country_id];
      }
      callback(err, code);
    });
  }
};

codeByDomain._help = 'Get country code by domain';

// Asynchronous method
var nameByDomain = function(data, domain, callback) {
  var name, err;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
    data.db_type !== CONST.CITY_EDITION_REV0 &&
    data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Err: Not Country or City Data');
    callback(err, name);
  } else {  
    __idBydomain__(data, domain, function(error, country_id) {
      if (error) {throw error;}
      if (!country_id) {
        name = null;
        //err = new Error('Err: Country Name Not Found');
      } else {
        name = data.COUNTRY_NAMES[country_id];
      }
      callback(err, name);
    });
  }
};

nameByDomain._help = 'Get country name by domain.';

// Synchronous method
var codeByAddr = function(data, addr) {
  var country_id, record, err;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
    data.db_type !== CONST.CITY_EDITION_REV0 &&
    data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Err: Not Country or City Data');
    throw err;
  } else if (data.db_type === CONST.CITY_EDITION_REV1) {
    record = city.record_by_addr(data, addr);
    return record.country_code;
  } else {
    country_id = __idByaddr__(data, addr);
    if (!country_id) {
      return null;
    } else {
      return data.COUNTRY_CODES[country_id];
    }
  }
};

codeByAddr._help = 'Get country by ip address.';

// Synchronous method
var nameByAddr = function(data, addr) {
  var record, country_id, err;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
    data.db_type !== CONST.CITY_EDITION_REV0 &&
    data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Err: Not Country or City Data');
    throw err;
  } else if (data.db_type === CONST.CITY_EDITION_REV1) {
    record = city.record_by_addr(data, addr);
    return record.country_name;
  } else {
    country_id = __idByaddr__(data, addr);
    if (!country_id) {
      return null;
    } else {
      return data.COUNTRY_NAMES[country_id];
    }
  }
};

nameByAddr = 'Get country name by ip address.';

exports.code_by_addr = codeByAddr;
exports.name_by_addr = nameByAddr;
exports.code_by_domain = codeByDomain;
exports.name_by_domain = nameByDomain;
