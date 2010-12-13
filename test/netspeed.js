var geoip = require('../geoip.js');

var data = geoip.open('/tmp/GeoIP-171_20040418/GeoIP-171_20040418.dat');

var NetSpeed = geoip.NetSpeed;

setTimeout(function() {
  console.log('The result of asynchronized method');
  console.log('NetSpeed.speed_by_addr(data, \'8.8.8.8\')');
  console.log('is \'' + NetSpeed.speed_by_addr(data, '8.8.8.8') + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);

setTimeout(function() {
  console.log('The result of asynchronized method');
  console.log('NetSpee.speed_by_domain(data, \'www.google.com\')');
  NetSpeed.speed_by_domain(data, 'www.google.com', function(err, speed) {
    if (err) {throw err;}
    console.log(speed);
  });
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 2000);     
