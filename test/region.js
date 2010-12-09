var geoip = require('../geoip.js');

var data = geoip.open('/tmp/GeoIPRegion-515_20050401/GeoIPRegion-515.dat');

var Region = geoip.Region;

setTimeout(function() {
  console.log('The result of ');
  console.log('Region.region_by_addr(data, \'8.8.8.8\')');
  console.log('is \'' + Region.region_by_addr(data, '8.8.8.8') + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);
