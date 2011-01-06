var geoip = require('../geoip.js');

var data = geoip.open('/media/d5fc189b-3c6b-4947-bda3-b7d0890fe6ca/Archive/GeoIP/GeoLiteCity.dat');

var City = geoip.City;

// Test record_by_addr method
setTimeout(function() {
  console.log('The result of synchronized method');
  console.log('City.record_by_addr(data, \'8.8.8.8\')');
  console.log('is \'' + JSON.stringify(City.record_by_addr(data, '8.8.8.8')) + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);

// Test record_by_domain method
setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('City.record_by_domain(data, \'www.google.com\', callback())');
  City.record_by_domain(data, 'www.google.com', function(err, record) {
    if (err) {throw err;}
    console.log('is \'' + JSON.stringify(record) + '\'');
    var keys = Object.keys(record);
    keys.forEach(function(k) {
      console.log(k + ':' + record[k]);
    });
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
  });
}, 2000);

