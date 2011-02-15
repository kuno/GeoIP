var geoip = require('../index.js');


// Test name_by_addr method
var Country = geoip.Country;

geoip.filter('/tmp/GeoIP.dat', function(err, type, data) {
  if (err) {throw err;}
  if (type === 'country') {
    // Test name_by_addr method
    setTimeout(function() {
      console.log('The result of synchronized method');
      console.log('Country.name_by_addr(data, \'8.8.8.8\')');
      console.log('is \'' + Country.name_by_addr(data, '8.8.8.8') + '\'');
      console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
    }, 1000);

    // Test code_by_addr method
    setTimeout(function() {
      console.log('The result of synchronized method');
      console.log('Country.code_by_addr(data, \'8.8.8.8\')');
      console.log('is \'' + Country.code_by_addr(data, '8.8.8.8') + '\'');
      console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
    }, 2000);
  } else {
    console.log('Not Country data');
  }
});


var data = geoip.open('/tmp/GeoIP.dat');

// Test name_by_domain method
setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('Country.name_by_domain(data, \'www.sina.com\', callback())');
  Country.name_by_domain(data, 'www.sina.com', function(err, name) {
    if (err) {throw err;}
    console.log('is \'' + name + '\'');
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
  });
}, 3000);

// Test code_by_domain method
setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('Country.code_by_domain(data, \'www.sina.com\', callback())');
  Country.code_by_domain(data, 'www.sina.com', function(err, code) {
    if (err) {throw err;}
    console.log('is \'' + code + '\'');
  });

}, 4000);
