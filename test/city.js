var assert = require('assert'), 
    geoip  = require('../index.js');

var City = geoip.City;
geoip.filter('/tmp/GeoLiteCity.dat', function(err, type, data) {

  if (type === 'city') {
    // Test record_by_addr method
    setTimeout(function() {
      console.log('The result of synchronized method');
      console.log('City.record_by_addr(data, \'8.8.8.8\')');
      console.log('is \'' + JSON.stringify(City.record_by_addr(data, '8.8.8.8')) + '\'');
      console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
    }, 1000);
  } else {
    console.log('Not city data');
  }
});

var data = geoip.open('/tmp/GeoLiteCity.dat');

// Test record_by_domain method
setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('City.record_by_domain(data, \'www.sina.com\', callback())');
  City.record_by_domain(data, 'www.sina.com', function(err, record) {
    if (err) {throw err;}
    console.log('is \'' + JSON.stringify(record) + '\'');
    var keys = Object.keys(record);
    keys.forEach(function(k) {
      console.log(k + ':' + record[k]);
    });
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
  });
}, 2000);
