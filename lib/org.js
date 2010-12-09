var CONST = require('../include/constants.js'),
seek_country = require('./utils.js').seek_country,
ip2long = require('./utils.js').ip2long;

function _get_org(data,ipnum){
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
  org = str.split(0, str.indexOf('\0'));

  return org;
}

exports.org_by_addr = function(data, addr) {
  var ipnum;
  if (data.db_type !== CONST.ORG_EDITION) {
    return 'Err: Not Organization Data';
  } else { 
    if (addr === null) {
      return 0;
    }
    ipnum = ip2long(addr);
    return _get_org(data, ipnum);
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
