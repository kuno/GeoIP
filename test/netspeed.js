var geoip = require('../geoip.js');

var data = geoip.open('/media/d5fc189b-3c6b-4947-bda3-b7d0890fe6ca/Archive/GeoIP/GeoIP-171_20040418/GeoIP-171_20040418.dat');

var NetSpeed = geoip.NetSpeed;

setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('NetSpeed.speed_by_addr(data, \'8.8.8.8\')');
  console.log('is \'' + NetSpeed.speed_by_addr(data, '8.8.8.8') + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);

setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('NetSpee.speed_by_domain(data, \'www.google.com\', callback())');
    NetSpeed.speed_by_domain(data, 'www.google.com', function(err, speed) {
    if (err) {throw err;}
    console.log(speed);
  });
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 2000);     
