var geoip = require('../index.js');

var NetSpeed = geoip.NetSpeed;

geoip.filter('/tmp/GeoIPNetSpeed.dat', function(err, type, data) {

  if (type === 'netspeed') {
    setTimeout(function() {
      console.log('The result of asynchronous method');
      console.log('NetSpeed.speed_by_addr(data, \'8.8.8.8\')');
      console.log('is \'' + NetSpeed.speed_by_addr(data, '8.8.8.8') + '\'');
      console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
    }, 1000);
  } else {
    console.log('Not netspeed data');
  }
});

var data = geoip.open('/tmp/GeoIPNetSpeed.dat');  

setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('NetSpee.speed_by_domain(data, \'www.sina.com\', callback())');
  NetSpeed.speed_by_domain(data, 'www.sina.com', function(err, speed) {
    if (err) {throw err;}
    console.log(speed);
  });
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 2000);
