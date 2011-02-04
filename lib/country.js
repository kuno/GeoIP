var dns = require('dns'),
city = require('./city.js'),
ip2long = require('./utils.js').ip2long,
seek_country = require('./utils.js').seek_country,
CONST = Object.freeze(require('../include/constants.js'));    

/******************************************************************************
 *  Inter Functions
 *****************************************************************************/
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

/******************************************************************************
 *  Export Functions
 ******************************************************************************/
 // Asynchronous method
exports.code_by_domain = function(data, domain, callback) {
  var code, err;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
  data.db_type !== CONST.CITY_EDITION_REV0 &&
  data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Err: Not Country or City Data');
    callback(err, code);
  } else {
    _id_by_domain(data, domain, function(error, country_id) {
      if (error) {throw error;}
      if (!country_id) {
        //code = 'Err: Country Code Not Found';
        err = new Error('Err: Country Code Not Found');
      } else {
        code = data.COUNTRY_CODES[country_id];
      }
      callback(err, code);
    });
  }
};

// Asynchronous method
exports.name_by_domain = function(data, domain, callback) {
  var name, err;
  if (data.db_type !== CONST.COUNTRY_EDITION && 
  data.db_type !== CONST.CITY_EDITION_REV0 &&
  data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Err: Not Country or City Data');
    callback(err, name);
  } else {  
    _id_by_domain(data, domain, function(error, country_id) {
      if (error) {throw error;}
      if (!country_id) {
        //name = 'Err: Country Name Not Found';
        err = new Error('Err: Country Name Not Found');
      } else {
        name = data.COUNTRY_NAMES[country_id];
      }
        callback(err, name);
      });
  }
};

// Synchronous method
exports.code_by_addr = function(data, addr) {
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
    country_id = _id_by_addr(data, addr);
    if (!country_id) {
      return null;
    } else {
      return data.COUNTRY_CODES[country_id];
    }
  }
};

// Synchronous method
exports.name_by_addr = function(data, addr) {
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
    country_id = _id_by_addr(data, addr);
    if (!country_id) {
      return null;
    } else {
      return data.COUNTRY_NAMES[country_id];
    }
  }
};
