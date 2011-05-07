var dns = require('dns'),
    open = require('./io.js').open,
    ipToLong = require('./utils.js').ipToLong,
    toFixedNumber = require('./utils.js').toFixedNumber,
    seekCountry = require('./utils.js').seekCountry,
    CONST = Object.freeze(require('../include/constants.js')),
    RECORD = require('../include/models.js').RECORD,
    DNSRECORD = require('../include/models.js').DNSRECORD;

/******************************************************************************
* Innter Functions
*****************************************************************************/
function __getRecord__(data, ipnum) {
  var offset, buffer, record, begin, end;

  var country_id = seekCountry(data, ipnum);
  if (country_id <= 1) {
    return null;
  }

  record = Object.create(RECORD);
  buffer = data.buffer;

  if (data.db_type > 1) {
    offset = country_id + (2 * data.record_length) * data.db_segments;
    country_id = buffer[offset];
  }

  record.country_code = data.COUNTRY_CODES[country_id];
  record.country_code3 = data.COUNTRY_CODES3[country_id];
  record.country_name = data.COUNTRY_NAMES[country_id];
  record.continent_code = data.CONTINENT_CODES[country_id];

  if (data.db_type > 1) {
    begin = end = offset + 1;
    while (buffer[end]) {
      end++;
    }
    // region_name?
    record.region = buffer.toString('utf8', begin, end);

    begin = end = end + 1;
    while (buffer[end]) {
      end++;
    }
    // city_name?
    record.city = buffer.toString('utf8', begin, end);

    begin = end = end + 1;
    while (buffer[end]) {
      end++;
    }
    record.postal_code = buffer.toString('utf8', begin, end);

    begin = end + 1;
    n = buffer[begin] + (buffer[begin + 1] << 8) + (buffer[begin + 2] << 16);
    begin += 3;
    record.latitude = toFixedNumber((n / 10000.0) - 180.0);
    n = buffer[begin] + (buffer[begin + 1] << 8) + (buffer[begin + 2] << 16);
    begin += 3;
    record.longitude = toFixedNumber((n / 10000.0) - 180.0);

    if (data.db_type === 2 && country_id === 225) {
      n = buffer[begin] + (buffer[begin + 1] << 8) + (buffer[begin + 2] << 16);
      record.dma_code = record.metro_code = Math.floor(n / 1000);
      record.area_code = n % 1000;
    }
  }
  return record;
}  

/******************************************************************************
*  Export Functions
*****************************************************************************/
/* Need passed test
*
exports.record_by_dns = function(data, str) {
  var record = Object.create(DNSRECORD);
  if (typeof str === 'string') {
    var substr = str.splite(';');
    substr.forEach(function(substar) {
        var subsubstr = substr.split('=');

        subsubstr.forEach(function(keyvalue) {
            var key = keyvalue[0];
            var value = keyvalue[1];
            if (key === 'co') {
              record.country_code = value;
            } else if (key === 'ci') {
              record.city = value;
            } else if (key === 're') {
              record.region = value;
            } else if (key === 'ac') {
              record.areacode = value;
            } else if (key == 'dm' || key === 'me') {
              record.dmacode = value;
              record.metrocode = value;
            } else if (key === 'is') {
              record.isp = value;
            } else if (key === 'or') {
              record.org = value;
            } else if (key === 'zi') {
              record.postal_code = value;
            } else if (key === 'la') {
              record.latitude = value;
            } else if (key === 'lo') {
              record.longitude = value;
            }
        });
    });

  }

  var number = data.COUNTRY_CODE_TO_NUMBER[record.country_code];
  record.country_code3 = data.COUNTRY_CODES3[number];
  record.country_name = data.COUNTRY_NAMES[number];

  if (record.region !== '') {
    if (record.country_name === 'US' || record.country_code === 'CA') {
      record.regionname = GLOBA['ISO'][record.country_code][record.region];
    } else {
      record.regionname = GLOBAL['FIPS'][record.country_code][record.region];
    }
  }

  return record;
};
*/
function City(file) {
  this.data = open(file);
}

// synchronous method
City.prototype.lookupSync = function(host) {
  var err, record, ipnum;
  ipnum = addr_to_ipnum(host);
  record = __getRecord__(this.data, ipnum);
  if (!record) {
    return null;
  } else {
    return record;
  }
};

// Asynchronous method
City.prototype.lookup = function(host, callback) {
  var record, err, ipnum, seek;
  dns.resolve4(host, function(error, addresses) {
      if (error) {throw error;}
      ipnum = ipToLong(addresses[0]);
      seek = __getRecord__(data, ipnum);
      if (!seek) {
        record = null;
      } else {
        record = seek;
      }
      callback(err, record);
  });
};

exports = City;
