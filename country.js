var dns = require('dns'),
ctiy = require('./city.js'),
ip2long = require('./lib/utils.js').ip2long,
seek_country = require('./lib/utils.js').seek_country,
GEOIP_COUNTRY_BEGIN = require('./lib/constants.js').GEOIP_COUNTRY_BEGIN,
GEOIP_CITY_EDITION_REV1 = require('./lib/constants.js').GEOIP_CITY_EDITION_REV1;

exports.id_by_addr = function(data, addr) {
    var ipnum = ip2long(addr);
    console.log(ipnum);
    var country_id = seek_country(data, ipnum) - GEOIP_COUNTRY_BEGIN;
    console.log(country_id);
    return country_id;
};   

exports.id_by_name = function(data, name) {
    var country_id;
    dns.resolve(name, rrtype='A', function(err, addresses) {
        if (err) {throw err;}
        (function(addrs) {
            addr = addrs[0];
            console.log(addr);
            country_id = exports.id_by_addr(data, addr);
            console.log('result is ' + country_id);
            return country_id;
        })(addresses);
    });

};

exports.code_by_name = function(data, name) {
    var country_id = exports.id_by_name(data, name);
    if (country_id !== false) {
        return data.COUNTRY_CODES[country_id];
    }
    return false;
};

exports.name_by_name = function(data, name) {
    var country_id = exports.id_by_name(data, name);
    if (country_id !== false) {
        return data.COUNTRY_NAMES[country_id];
    }
    return false;
};

exports.code_by_addr = function(data, addr) {
    var country_id, record;
    if (data.databaseType === GEOIP_CITY_EDITION_REV1) {
        record = city.record_by_addr(data, addr);
    } else {
        country_id = exports.id_by_addr(data, addr);
        if (country_id !== false) {
            return data.COUNTRY_CODES[country_id];
        }
    }

};

exports.name_by_addr = function(data, addr) {
    var record, country_id;
    if (data.databaseType === GEOIP_CITY_EDITION_REV1) {
        record = city.record_by_addr(data, addr);
        return record.country_name;
    } else {
        country_id = exports.id_by_addr(data, addr);
        if (country_id !== false) {
            return data.COUNTRY_NAMES[country_id];
        }

    }
    return false;
};
