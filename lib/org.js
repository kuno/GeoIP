var dns = require('dns'),
CONST = Object.freeze(require('../include/constants.js')),
seek_country = require('./utils.js').seek_country,
ip2long = require('./utils.js').ip2long,
str2array = require('./utils.js').str2array,
str2obj = require('./utils.js').str2obj;

/******************************************************************************
 * Inner Functions
 ******************************************************************************/
function _get_org(data, ipnum){
  var org_id, buf, org_buf, str, org, start, end;

  org_id = seek_country(data, ipnum);
  if (org_id == data.db_segments) {
    return null;
  }

  start  = org_id + (2 * data.record_length - 1) * data.db_segments;
  end = start + CONST.MAX_ORG_RECORD_LENGTH;
  buf = data.buffer;
  org_buf = data.buffer.slice(start, end);
  str = org_buf.toString('ascii', 0, org_buf.length);
  org = str.split(0, str.indexOf('\u0000'));

  return org;
}

/******************************************************************************
 * Export Functions
 ******************************************************************************/
 // Synchronous method
exports.org_by_addr = function(data, addr) {
  var ipnum, org, err;
  if (data.db_type !== CONST.ORG_EDITION) {
    err = new Error('Err: Not Organization Data');
    throw err;
  } 
  ipnum = ip2long(addr);
  org =  _get_org(data, ipnum);
  if (!org) {
    return 'Err: Organization Not Found';
  } else {
    return str2array(org, '\u0000');
  }
};

// Asynchronous method
exports.org_by_domain = function(data, domain, callback) {
  var org, err, ipnum, seek;
  if (data.db_type !== CONST.ORG_EDITION) {
    err = new Error('Not Organization Data');
    callback(err, org);
  } else {
    dns.resolve(domain, rrtype='A', function(error, addresses) {
      if (error) {throw error;}
      ipnum = ip2long(addresses[0]);
      seek = _get_org(data, ipnum);
      if (!seek) {
        org = 'Err: Organization Not Found';
      } else {
        org = str2array(seek, '\u0000');
      }
      callback(err, org);
    });
  }
};

// Synchronous method
exports.asn_by_addr = function(data, addr) {
  var ipnum, asn, err;
  if (data.db_type !== CONST.ASNUM_EDITION) {
    err = new Error('Err: Not ASNUM Data');
    throw err;
  }
    ipnum = ip2long(addr);
    asn = _get_org(data, ipnum);
    if (!asn) {
      return 'Err: ASNumber Not Found.';
    } else {
      return str2obj(asn, '\u0000');
    }
};

// Asynchronous method
exports.asn_by_domain = function(data, domain, callback) {
  var asn, err, ipnum, seek;
  if (data.db_type !== CONST.ASNUM_EDITION) {
    err = new Error('Not ASNUM Data');
    callback(err, asn);
  } else {
    dns.resolve(domain, rrtype='A', function(error, addresses) {
      if (error) {throw error;}
      ipnum = ip2long(addresses[0]);
      seek = _get_org(data, ipnum);
      if (!seek) {
        asn = 'Err: ASNumber Not Found';
      } else {
        asn = str2obj(seek, '\u0000');
      }
      callback(err, asn);
    });
  }
};

/* Need passed test in future version
 exports.isp_by_addr = function(data, addr) {
   var ipnum;
   if (data.db_type === CONST.ISP_EDITION) {
     return 'Err: Not ISP Data';
   } else {
     if (addr === null) {
       return 0;
     }
     ipnum = ip2long(addr);
     return _get_org(data, ipnum);
   }
 };
 */
