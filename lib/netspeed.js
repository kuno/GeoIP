var dns = require('dns'),
ip2long = require('./utils.js').ip2long,
seek_country = require('./utils.js').seek_country,
CONST = require('../include/constants.js');    

function _id_by_addr(data, addr) {
  if (addr !== false) {
    var ipnum = ip2long(addr);
    var country_id = seek_country(data, ipnum) - CONST.COUNTRY_BEGIN;
    return country_id;
  }
  return false;
}

exports.speed_by_addr = function(data, addr) {
  var id, speed;
  if (data.db_type !== CONST.NETSPEED_EDITION) {
    return 'Err: Not NetSpeed Data';
  } else {
    id = _id_by_addr(data, addr);

    if (id === CONST.UNKNOWN_SPEED) {
      speed = 'Unknown';
    } else if (id === CONST.DIALUP_SPEED) {
      speed = 'Dailup';
    } else if (id === CONST.CABLEDSL_SPEED) {
      speed = 'Cable';
    } else if (id === CONST.CORPORATE_SPEED) {
      speed = 'Corporate';
    }

    return speed;
  }
};
