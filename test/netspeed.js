var geoip = require('../geoip.js');

var data = geoip.open('/tmp/GeoIP-171_20040418/GeoIP-171_20040418.dat');

var NetSpeed = geoip.NetSpeed;

setTimeout(function() {
  console.log('The result of ');
  console.log('NetSpeed.speed_by_addr(data, \'8.8.8.8\')');
  console.log('is \'' + NetSpeed.speed_by_addr(data, '8.8.8.8') + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);
