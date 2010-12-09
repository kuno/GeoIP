var geoip = require('../geoip.js');

var org_data = geoip.open('/tmp/GeoIP-111_20030603/GeoIPOrg-111.dat');

var Org = geoip.Org;

setTimeout(function() {
  console.log('The result of ');
  console.log('Org.org_by_addr(org_data, \'8.8.8.8\')');
  console.log('is \'' + Org.org_by_addr(org_data, '8.8.8.8.8') + '\'');
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);
