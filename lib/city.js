var dns = require('dns'), 
ip2long = require('./utils.js').ip2long,
seek_country = require('./utils.js').seek_country,
CONST = Object.freeze(require('../include/constants.js')),
RECORD = require('../include/models.js').RECORD,
DNSRECORD = require('../include/models.js').DNSRECORD;

/******************************************************************************
 * Innter Functions
 *****************************************************************************/
function _get_record(data, ipnum) {
  var countries = seek_country(data, ipnum);
  if (countries === data.db_segments) {
    return null;
  }

  var record = Object.create(RECORD);
  var buf, buf_pos = 0, str_len = 0;
  var start, end, index, sec, char, j;

  start = countries + (2 * data.record_length - 1) * data.db_segments;
  end = start + CONST.FULL_RECORD_LENGTH;

  buf = data.buffer.slice(start, end);
  char = parseInt(buf[buf_pos], 10);

  record.country_code = data.COUNTRY_CODES[char];
  record.country_code3 = data.COUNTRY_CODES3[char];
  record.country_name = data.COUNTRY_NAMES[char];
  record.continet_code = data.CONTINENT_CODES[char];

  buf_pos++;

  // Get region
  do {
    index = buf_pos + str_len;
    char = parseInt(buf[index], 10);
    if (char !== 0) {str_len++;}
  } while (char !== 0);
  if (str_len > 0) {
    start = buf_pos;
    end = start + str_len;
    sec = buf.slice(start, end);
    record.region = sec.toString('ascii', 0, sec.length);
  }
  buf_pos += (str_len + 1);
  str_len = 0;

  // Get city
  do {
    index = buf_pos + str_len;
    char = parseInt(buf[index], 10);
    if (char !== 0) {str_len++;}
  } while (char !== 0);
  if (str_len > 0) {
    start = buf_pos;
    end = start + str_len;
    sec = buf.slice(start, end);
    record.city = sec.toString('ascii', 0, sec.length);
  }
  buf_pos += (str_len + 1);
  str_len = 0;

  // Get postal code
  do {
    index = buf_pos + str_len;
    char = parseInt(buf[index], 10);
    if (char !== 0) {str_len++;}
  } while (char !== 0); 
  if (str_len > 0) {
    start = buf_pos;
    end = start + str_len;
    sec = buf.slice(start, end);
    record.postal_code = sec.toString('ascii', 0, sec.length);
  }
  buf_pos += (str_len + 1);
  str_len = 0;

  // GEt latitude and longitude
  var latitude = 0;
  var longitude = 0;
  for (j = 0; j < 3; ++j) {
    index = buf_pos++;
    char = parseInt(buf[index], 10);
    latitude += (char << (j * 8));
  }
  latitude = (latitude / 10000) - 180;
  record.latitude = latitude.toFixed(CONST.FIXED_DIGIT);

  for (j = 0; j < 3; ++j) {
    index = buf_pos++;
    char = parseInt(buf[index], 10);
    longitude += (char << (j * 8));
  }
  longitude = (longitude / 10000) - 180;
  record.longitude = longitude.toFixed(CONST.FIXED_DIGIT);

  if (CONST.CITY_EDITION_REV1 === data.db_type) {
    var metroarea_combo = 0;
    if (record.country_code === 'US') {
      for (j = 0; j < 3; ++j) {
        index = buf_pos++;
        char = parseInt(buf[index], 10);
        metroarea_combo += (char << (j * 8));
      }
      record.metro_code = record.dma_code = Math.floor(metroarea_combo / 1000);
      record.area_code = metroarea_combo % 1000;
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

// synchronous method
exports.record_by_addr = function(data, addr) {
  var err, record, ipnum;
  if (data.db_type !== CONST.CITY_EDITION_REV0 && 
  data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Err: Not City Data');
    throw err;
  }
  ipnum = ip2long(addr);
  record = _get_record(data, ipnum);
  if (!record) {
    return 'Err: City Record Not Found';
  } else {
    return record;
  }
};

// Asynchronous method
exports.record_by_domain = function(data, domain, callback) {
  var record, err, ipnum, seek;
  if (data.db_type !== CONST.CITY_EDITION_REV0 &&
  data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Not City Data');
    callback(err, record);
  } else {
    dns.resolve(domain, rrtype='A', function(error, addresses) {
      if (error) {throw error;}
      ipnum = ip2long(addresses[0]);
      seek = _get_record(data, ipnum);
      if (!seek) {
        record = 'Err: City Record Not Found';
      } else {
        record = seek;
      }
      callback(err, record);
    });
  }
};
