/******************************************************************************
* Varialbes
*****************************************************************************/
var net = require('net'),
CONST = Object.freeze(require('../include/constants.js'));

/*****************************************************************************
* Inter Functions
****************************************************************************/
// Validate IPv4 address
function __isIP4__(addr) {
  var isValid = false, groups, counter = 0;  
  if (typeof addr !== 'string') {
    return false;
  } else {
    groups = addr.split('.', 4);
    groups.forEach(function(numStr) {
      if (parseInt(numStr, 10) <= 255) {
        ++counter;
      }
    });

    if (counter === 4) {
      isValid = true;
    }
  }

  return isValid;
}


/*****************************************************************************
* Exprots Functions
****************************************************************************/
// Convert IP address to an unsigned integer
var ipToLong = function(addr) {
  var counter, isValid, groups, long = 0;
  if (typeof net.isIP4 === 'function') {
    isValid = net.isIP4(addr);
  } else {
    isValid = __isIP4__(addr);
  }

  if (!isValid) {
    return false;
  } else {
    groups = addr.split('.');
    counter = groups.length;
  }

  groups.forEach(function(num) {
    long = long + (parseInt(num, 10) * Math.pow(256, counter - 1));
    --counter;
  });

  if (long < 0) {
    long = Math.pow(2, 32) + long;
  }

  return long;

};

// Convert a buffer to an unsigned integer
var bufToLong = function(buf) {
  var long = 0;
  var len = buf.length;

  for (var i = len - 1; i >= 0; --i) {
    long = long + (parseInt(buf[i], 10) * Math.pow (256, i));
  }

  return long;
};

// Convert string to array
var strToArray = function(string, delimiter) {
  var array = [];
  if (typeof string !== 'string') {
    string = string.toString();
  }
  array = string.trim().split(delimiter);
  array.forEach(function(str) {
    str = str.trim();
  });

  return array;
};

// Convert string to an array of object
var strToObj = function(string, delimiter) {
  var obj = [], arr = [], num, des;
  if (typeof string !== 'string') {
    string = string.toString();
  }

  arr = string.split(delimiter);
  arr.forEach(function(str) {
    if (str.length >= 8) {
    str = str.trim();
    num = str.substr(0, str.indexOf(' ')).replace(/,/g, '');
    des = str.substr(str.indexOf(' ')+1, str.length).replace(/,$/g, '');
    obj.push({"number":num, "description":des});
    }
  });

  return obj;
};

// New seekCountry function, thanks Vladimir Dronnikov 
var seekCountry = function(data, ipnum) {
  var depth, pos, offset = 0, 
  length = data.record_length,
  buffer = data.buffer,
  begin  = data.db_segments;

  if (length === 3) {
    for (depth = 31; depth >= 0; depth--) {
      pos = 6 * offset;
      if (ipnum & (1 << depth)) {
        pos += 3;
      }
      offset = buffer[pos] + (buffer[pos + 1] << 8) + (buffer[pos + 2] << 16);
      if (offset >= begin) {
        return offset - begin;
      }
    }
  } else if (length === 4) {
    for (depth = 31; depth >= 0; depth--) {
      pos = 8 * offset;
      if (ipnum & (1 << depth)) {
        pos += 4;
      }
      offset = buffer[pos] + (buffer[pos + 1] << 8) + (buffer[pos + 2] << 16) + (buffer[pos + 3] << 24);
      if (offset >= begin) {
        return offset - begin;
      }
    }
  } else {
    return 0;
  }
};

var matchFingerPrint = function(data) {
  var match = false, slice, fingerPrint;
  slice = data.buffer.slice(0, 8);
  fingerPrint = slice.toString('utf8', 0, slice.length);
  if (CONST.FINGERPRINT === fingerPrint) {
    match = true;
  }

  return match;
};

var toFixedNumber = function(longNumber) {
  return parseFloat(longNumber.toPrecision(CONST.FIXED_DIGIT));
};

exports.ipToLong = ipToLong;
exports.bufToLong = bufToLong;
exports.strToObj = strToObj;
exports.strToArray = strToArray;
exports.seekCountry = seekCountry;
exports.toFixedNumber = toFixedNumber;
exports.matchFingerPrint = matchFingerPrint;
