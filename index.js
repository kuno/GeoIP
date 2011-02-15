var fs    = require('fs'),
path      = require('path'),
bufToLong  = require('./lib/utils.js').bufToLong,
matchFingerPrint = require('./lib/utils.js').matchFingerPrint,
DATA      = require('./include/models.js').DATA, 
CONST     = Object.freeze(require('./include/constants.js'));

/******************************************************************************
* Inner Functions
*****************************************************************************/
function __setupSegments__(data) {
  var seg, delim, i, j, 
  buf = data.buffer, offset = buf.length - 3;

  // Set data type and segments
  for (i = 0; i < CONST.STRUCTURE_INFO_MAX_SIZE; i++) {
    delim = bufToLong(buf.slice(offset, offset + 3));
    offset += 3;
    if (delim === CONST.DELIMETER_NUMBER) {
      data.db_type = parseInt(buf[offset], 10);
      offset++;
      if (data.db_type === CONST.REGION_EDITION_REV0) {
        data.db_segments = CONST.STATE_BEGIN_REV0;
      } else if (data.db_type === CONST.REGION_EDITION_REV1) {
        data.db_segments = CONST.STATE_BEGIN_REV1;
      } else if (data.db_type === CONST.CITY_EDITION_REV0 ||
        data.db_type === CONST.CITY_EDITION_REV1 ||
        data.db_type === CONST.ORG_EDITION  ||
        data.db_type === CONST.DOMAIN_EDITION ||
        data.db_type === CONST.ISP_EDITION    ||
        data.db_type === CONST.LOCATIONA_EDITION   ||
        data.db_type === CONST.ACCURACYRADIUS_EDITION ||
        data.db_type === CONST.ASNUM_EDITION) {
        data.db_segments = 0;
        seg = buf.slice(offset, offset + CONST.SEGMENT_RECORD_LENGTH);
        for (j = 0; j < CONST.SEGMENT_RECORD_LENGTH; j++) {
          data.db_segments += (parseInt(seg[j], 10) << (j * 8));
        }
      } else if (data.db_type === CONST.PROXY_EDITION ||
        data.db_type === CONST.NETSPEED_EDITION) {
        data.db_segments = CONST.COUNTRY_BEGIN;
      }   
      break;
    } else {
      offset -= 4;
    }

    // Set default data type to country edition
    if (data.db_type === undefined) {
      if (matchFingerPrint(data)) {
        data.db_type = CONST.COUNTRY_EDITION;
        data.db_segments = CONST.COUNTRY_BEGIN;
      }
    }
  }

  // Set record length
  if (data.db_type === CONST.ORG_EDITION ||
    data.db_type === CONST.DOMAIN_EDITION ||
    data.db_type === CONST.ISP_EDITION) {
    data.record_length = CONST.ORG_RECORD_LENGTH;
  } else {
    data.record_length = CONST.STANDARD_RECORD_LENGTH; 
  }

  return data;
}

function __getType__(code) {
  switch(code) {
  case CONST.COUNTRY_EDITION:
    type = 'country';
    break;

  case CONST.CITY_EDITION_REV0:
    type = 'city';
    break;

  case CONST.CITY_EDITION_REV1:
    type = 'city';
    break;

  case CONST.REGION_EDITION_REV0:
    type = 'region';
    break;

  case CONST.REGION_EDITION_REV1:
    type = 'region';
    break;

  case CONST.ORG_EDITION:
    type = 'org';
    break;

  case CONST.ASNUM_EDITION:
    type = 'asnumber';
    break;

  case CONST.NETSPEED_EDITION:
    type = 'netspeed';
    break;

  default:
    type = null;
    break;
  }  

  return type;
}

/******************************************************************************
* Exprots Functions
*****************************************************************************/
var open = function(file) {
  var stats, bytesRead;
  var data = new DATA();
  data.file_descriptor = fs.openSync(file, 'r');
  stats = fs.fstatSync(data.file_descriptor);
  data.buffer = new Buffer(stats.size);
  bytesRead = fs.readSync(data.file_descriptor, data.buffer, 0, stats.size, 0);

  if (bytesRead >= 0) {
    return __setupSegments__(data);
  } else {
    return false;
  }
};

var check = function(data) {
  var code, type;
  code = data.db_type;


  return __getType__(code);
};

var filter = function(file, callback) {
  var error, code, type, data = new DATA();
  fs.open(file, 'r', mode=0666, function(err, fd) {
    if (err) {throw err;}
    data.file_descriptor = fd;
    fs.fstat(data.file_descriptor, function(err, stats) {
      if (err) {throw err;}
      data.buffer = new Buffer(stats.size);
      fs.read(data.file_descriptor, data.buffer, 0, stats.size, 0, function(err, bytesRead) {
        if (err) {throw err;}
        data = __setupSegments__(data);
        code = data.db_type;
        type = __getType__(code);
        if (!type) {
          error = new Error('Uknow Data Type');
        }
        callback(error, type, data);
      });
    });
  });
};

var close = function(data) {
  var keys;
  fs.closeSync(data.file_descriptor);
  keys = Object.keys(data);
  keys.forEach(function(k) {
    delete data[k];
  });
};    

// GeoIP module method
open._usage = "geoip.open(\'/path/to/file\')";
exports.open = open;

check._usage = 'geoip.check(data)';
exports.check = check;

filter._usage = "geoip.filter(\'/path/to/file\', <CALLBACK>)";
exports.filter = filter;

close._usage = 'geoip.close(data)';
exports.close = close;

// MOdules
exports.NetSpeed = require('./lib/netspeed.js');
exports.Country  = require('./lib/country.js');
exports.Region   = require('./lib/region.js');
exports.City     = require('./lib/city.js');
exports.Org      = require('./lib/org.js');

// Meta data
module.exports.meta   = require('./lib/meta.js');
