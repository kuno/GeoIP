var dns = require('dns'),
ipToLong = require('./utils.js').ipToLong,
seekCountry = require('./utils.js').seekCountry,
CONST = Object.freeze(require('../include/constants.js'));    

/******************************************************************************
* Inner Functions
*****************************************************************************/
function _id_by_addr(data, addr) {
  if (addr !== false) {
    var ipnum = ipToLong(addr);
    var country_id = seekCountry(data, ipnum);
    return country_id;
  }
  return false;
}

/******************************************************************************
* Export Functions
******************************************************************************/
// Synchronous method
exports.speed_by_addr = function(data, addr) {
  var id, speed = null, err;
  if (data.db_type !== CONST.NETSPEED_EDITION) {
    err = new Error('Err: Not NetSpeed Data');
    throw err;
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

// Asynchronous method
exports.speed_by_domain = function(data, domain, callback) {
  var id, err, speed;
  if (data.db_type !== CONST.NETSPEED_EDITION) {
    err = new Error('Not NetSpeed Data');
    callback(err, speed);
  } else {
    dns.resolve4(domain, function(err, addresses) {
      if (err) {throw err;}
      id = _id_by_addr(data, addresses[0]);

      if (id === CONST.UNKNOWN_SPEED) {
        speed = 'Unknown';
      } else if (id === CONST.DIALUP_SPEED) {
        speed = 'Dailup';
      } else if (id === CONST.CABLEDSL_SPEED) {
        speed = 'Cable';
      } else if (id === CONST.CORPORATE_SPEED) {
        speed = 'Corporate';
      }
      callback(err, speed);
    });
  }
};
