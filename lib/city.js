var dns = require('dns'), 
ipToLong = require('./utils.js').ipToLong,
seekCountry = require('./utils.js').seekCountry,
CONST = Object.freeze(require('../include/constants.js')),
RECORD = require('../include/models.js').RECORD,
DNSRECORD = require('../include/models.js').DNSRECORD;

/******************************************************************************
* Innter Functions
*****************************************************************************/
function __getRecord__(data, ipnum) {
  var offset;
  var country_id = seekCountry(data, ipnum);
  if (country_id <= 1) {
    return null;
  }
  var record = Object.create(RECORD);
  var buf, buf_pos = 0, str_len = 0;
  var start, end, index, sec, char, j, b, e, n;
  /*

  start = countries + (2 * data.record_length - 1) * data.db_segments;
  end = start + CONST.FULL_RECORD_LENGTH;

  buf = data.buffer.slice(start, end);
  char = parseInt(buf[buf_pos], 10);
  */

  var buffer = data.buffer;

  if (data.db_type > 1) {
    offset = country_id + (2 * data.record_length) * data.db_segments;
    country_id = buffer[offset];
  }


  record.country_code = data.COUNTRY_CODES[country_id];
  record.country_code3 = data.COUNTRY_CODES3[country_id];
  record.country_name = data.COUNTRY_NAMES[country_id];
  record.continet_code = data.CONTINENT_CODES[country_id];

     if (data.db_type > 1) {
      b = e = offset + 1;
      while (buffer[e]) {
        e++;
      }
      record.region_name = buffer.toString('utf8', b, e);
      b = e = e + 1;
      while (buffer[e]) {
        e++;
      }
      record.city_name = buffer.toString('utf8', b, e);
      b = e = e + 1;
      while (buffer[e]) {
        e++;
      }
      record.postal_code = buffer.toString('utf8', b, e);
      b = e + 1;
      n = buffer[b] + (buffer[b + 1] << 8) + (buffer[b + 2] << 16);
      b += 3;
      record.latitude = (n / 10000.0).toFixed(6) - 180;
      n = buffer[b] + (buffer[b + 1] << 8) + (buffer[b + 2] << 16);
      b += 3;
      record.longitude = (n / 10000.0).toFixed(6) - 180;
      if (data.db_type === 2) {
        if (country_id === 225) {
          n = buffer[b] + (buffer[b + 1] << 8) + (buffer[b + 2] << 16);
          record.dma_code = record.metro_code = Math.floor(n / 1000);
          record.area_code = n % 1000;
        }
      }
    }
    return record;
  }  
  /*
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
  */

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
var recordByaddr = function(data, addr) {
  var err, record, ipnum;
  if (data.db_type !== CONST.CITY_EDITION_REV0 && 
    data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Err: Not City Data');
    throw err;
  }
  ipnum = ipToLong(addr);
  record = __getRecord__(data, ipnum);
  if (!record) {
    return null;
  } else {
    return record;
  }
};

recordByaddr._help = 'Get city record by ip addClass.';

// Asynchronous method
var recordBydomain = function(data, domain, callback) {
  var record, err, ipnum, seek;
  if (data.db_type !== CONST.CITY_EDITION_REV0 &&
    data.db_type !== CONST.CITY_EDITION_REV1) {
    err = new Error('Not City Data');
    callback(err, record);
  } else {
    dns.resolve4(domain, function(error, addresses) {
      if (error) {throw error;}
      ipnum = ipToLong(addresses[0]);
      seek = __getRecord__(data, ipnum);
      if (!seek) {
        record = null;
        // err = new Error('Err: City Record Not Found');
      } else {
        record = seek;
      }
      callback(err, record);
    });
  }
};

recordBydomain._help = 'Get city record by domain.';

exports.record_by_addr = recordByaddr;
exports.record_by_domain = recordBydomain;
