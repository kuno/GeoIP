var geoip = require('../geoip.js');

var data = geoip.open('/tmp/GeoIP-111_20030603/GeoIPOrg-111.dat');

var Org = geoip.Org;

setTimeout(function() {
  console.log('The result of ');
  console.log('Org.org_by_addr(data, \'8.8.8.8\')');
  console.log('is \'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);
