var geoip = require('../geoip.js');

var org_data = geoip.open('/tmp/GeoIP-111_20030603/GeoIPOrg-111.dat');

var Org = geoip.Org;

setTimeout(function() {
  var result = Org.org_by_addr(org_data, '8.8.8.8');
  console.log('The result of ');
  console.log('Org.org_by_addr(org_data, \'8.8.8.8\')');
  result.forEach(function(str) {
    console.log(str);
  });
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);

var asn_data = geoip.open('/tmp/GeoIPASNum.dat');

setTimeout(function() {
  var result = Org.asn_by_addr(asn_data, '8.8.8.8');
  console.log('The result of ');
  console.log('Org.asn_by_addr(asn_data, \'8.8.8.8\')');
  console.log(result);
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 2000);
