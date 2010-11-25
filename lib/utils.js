/******************************************************************************
 * Varialbes
 *****************************************************************************/
 var BIT = 7;
 var net = require('net');

 /*
  * Inter Functions
  */
 exports.int2octal = function(int) {
     var bits = ['0', '0', '0', '0', '0', '0', '0'];
     var octal = '0';
     for (var i = BIT; i >= 0; --i) {
         if (int > 2 && i <= 255 && Math.pow(2, i) <= int) {
             bits[BIT-1] = '1';
             int -= Math.pow(2, i);
         } else if (int === 2) {
             bits[BIT-2] = '1';
         } else if (int === 1 && i === 0) {
             bits[BIT-1] = '1';
         } else if (int === 0 && i === 0) {
             bits[BIT-1] = '0';
         }
         else {
             // Do nothing
         }
     }

     bits.forEach(function(bit) {
         octal += bit;
     });

     return octal;
 };

 /*
  * Exprots Functions
  */
 exports.ip2long = function(addr) {
     var ipArr, long = 0;
     var ipVer = net.isIP(addr);

     if (!ipVer) {
         return false;
     } else {
         ipArr = addr.split('.', ipVer);
     }

     ipArr.forEach(function(num) {
         long = long + (parseInt(num, 10) * Math.pow(256, ipVer-1));
         --ipVer;
     });

     if (long < 0) {
         long = Math.pow(2, 32) + long;
     }

     return long;

 };

 exports.buf2long = function(buf) {
     var long = 0;
     var len = buf.length;

     for (var i = len - 1; i >= 0; --i) {
         long = long + (parseInt(buf[i], 10) * Math.pow (256, i));
     }

     return long;
 };

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
             if (x[1] >= data.databaseSegments) {
                 return x[1];
             }
             offset = x[1];
         } else {
             if (x[0] >= data.databaseSegments) {
                 return x[0];
             }
             offset = x[0];
         }
     }

     //  return false;
 };   
