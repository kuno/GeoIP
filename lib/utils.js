/******************************************************************************
 * Varialbes
 *****************************************************************************/
 var net = require('net');

/*****************************************************************************
 * Inter Functions
 ****************************************************************************/
 // Validate IPv4 address
function _isIP4(addr) {
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
exports.ip2long = function(addr) {
   var counter, isValid, groups, long = 0;
   if (typeof net.isIP4 === 'function') {
     isValid = net.isIP4(addr);
   } else {
     isValid = _isIP4(addr);
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
exports.buf2long = function(buf) {
   var long = 0;
   var len = buf.length;

   for (var i = len - 1; i >= 0; --i) {
     long = long + (parseInt(buf[i], 10) * Math.pow (256, i));
   }

   return long;
 };

// Convert string to array
exports.str2array = function(string, delimiter) {
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
exports.str2obj = function(string, delimiter) {
   var obj = [], arr = [], num, des;
   if (typeof string !== 'string') {
     string = string.toString();
   }

   arr = string.split(delimiter);
   arr.forEach(function(str) {
     str = str.trim();
     num = str.substr(0, 7).replace(/,/g, '');
     des = str.substr(8, str.length).replace(/,$/g, '');
     obj.push({"number":num, "description":des});
   });

   return obj;
 };

// Seek contury id in given data 
exports.seek_country = function(data, ipnum) {
   var offset = 0;
   var start, end;
   var length = data.record_length;
   var buf = data.buffer;
   for (var depth = 31; depth >= 0; --depth) {
     start = 2 * length * offset;
     end = start + 2 * length;
     var sec = buf.slice(start, end);
     var x = [0, 0];
     for (var i = 0; i < 2; ++i) {
       for (var j = 0; j < length; ++j) {
         x[i] += (parseInt(sec[length * i + j], 10) << (j * 8));
       }
     }
     if (ipnum & (1 << depth)) {
       if (x[1] >= data.db_segments) {
         return x[1];
       }
       offset = x[1];
     } else {
       if (x[0] >= data.db_segments) {
         return x[0];
       }
       offset = x[0];
     }
   }
   //  return false;
 };
