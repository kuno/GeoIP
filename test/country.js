var geoip = require('../geoip.js');

var Country = geoip.Country;

// Test name_by_addr method
setTimeout(function() {
  console.log('The result of synchronized method');
  console.log('geoip.filter(\'/tmp/GeoIP.dat\', callback(err, type, data)');
  console.log('is ');
  geoip.filter('/tmp/GeoIP.dat', function(err, type, data) {
    if (err) {throw err;}
                              
    console.log('The type of data is '+type);
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');

    // Test name_by_domain method
    setTimeout(function() {
      console.log('The result of asynchronous method');
      console.log('Country.name_by_domain(data, \'www.google.com\', callback())');
      Country.name_by_domain(data, 'www.google.com', function(err, name) {
        if (err) {throw err;}
        console.log('is \'' + name + '\'');
        console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
      });
    }, 2000);

    // Test code_by_domain method
    setTimeout(function() {
      console.log('The result of asynchronous method');
      console.log('Country.code_by_domain(data, \'www.google.com\', callback())');
      Country.code_by_domain(data, 'www.google.com', function(err, code) {
        if (err) {throw err;}
        console.log('is \'' + code + '\'');
      });
    }, 3000);
  });
}, 1000);


var data = geoip.open('/tmp/GeoIP.dat');

var type = geoip.check(data);

console.log('The type of data is '+type);

// Test name_by_addr method
setTimeout(function() {
  console.log('The result of synchronized method');
  console.log('Country.name_by_addr(data, \'8.8.8.8\')');
  console.log('is \'' + Country.name_by_addr(data, '8.8.8.8') + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 4000);

// Test code_by_addr method
setTimeout(function() {
  console.log('The result of synchronized method');
  console.log('Country.code_by_addr(data, \'8.8.8.8\')');
  console.log('is \'' + Country.code_by_addr(data, '8.8.8.8') + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 5000);

