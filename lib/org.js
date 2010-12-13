var dns = require('dns'),
CONST = require('../include/constants.js'),
seek_country = require('./utils.js').seek_country,
ip2long = require('./utils.js').ip2long,
str2array = require('./utils.js').str2array,
str2obj = require('./utils.js').str2obj;

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

exports.org_by_addr = function(data, addr) {
  var ipnum, org;
  if (data.db_type !== CONST.ORG_EDITION) {
    return 'Err: Not Organization Data';
  } else { 
    if (addr === null) {
      return 0;
    }
    ipnum = ip2long(addr);
    org =  _get_org(data, ipnum);
    return str2array(org, '\u0000');
  }
};

exports.org_by_domain = function(data, domain, callback) {
  var org, err, ipnum, seek;
  if (data.db_type !== CONST.ORG_EDITION) {
    err = new Error('Not Organization Data');
    callback(err, org);
  } else {
    dns.resolve(domain, rrtype='A', function(err, addresses) {
      if (err) {throw err;}
      ipnum = ip2long(addresses[0]);
      seek = _get_org(data, ipnum);
      if (!seek) {
        seek = new Error('Organization not found');
      } else {
        org = str2array(seek, '\u0000');
      }
      callback(err, seek);
    });
  }
};

exports.asn_by_addr = function(data, addr) {
  var ipnum, asn;
  if (data.db_type !== CONST.ASNUM_EDITION) {
    return 'Err: Not ASNUM Data';
  } else {
    if (addr === null) {
      return 0;
    }
    ipnum = ip2long(addr);
    asn = _get_org(data, ipnum);
    return str2obj(asn, '\u0000');
  }
};

exports.asn_by_domain = function(data, domain, callback) {
  var asn, err, ipnum, seek;
  if (data.db_type !== CONST.ASNUM_EDITION) {
    err = new Error('Not ASNUM Data');
    callback(err, asn);
  } else {
    dns.resolve(domain, rrtype='A', function(err, addresses) {
      if (err) {throw err;}
      ipnum = ip2long(addresses[0]);
      seek = _get_org(data, ipnum);
      if (!seek) {
        err = new Error('ASNUM not found');
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
