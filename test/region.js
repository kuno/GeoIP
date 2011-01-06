var geoip = require('../geoip.js');

var data = geoip.open('/media/d5fc189b-3c6b-4947-bda3-b7d0890fe6ca/Archive/GeoIP/GeoIPRegion-515_20050401/GeoIPRegion-515.dat');

var Region = geoip.Region;

setTimeout(function() {
  console.log('The result of synchronized method');
  console.log('Region.region_by_addr(data, \'8.8.8.8\')');
  console.log('is \'' + Region.region_by_addr(data, '8.8.8.8') + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);

setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('Region.region_by_domain(data, \'www.google.com\', callback())');
  Region.region_by_domain(data, 'www.google.com', function(err, region) {
      if (err) {throw err;}
    console.log('is ' + region);
  });
}, 2000);
  


