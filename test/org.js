var geoip = require('../geoip.js');

var org_data = geoip.open('/tmp/GeoIP-111_20030603/GeoIPOrg-111.dat');

var Org = geoip.Org;

setTimeout(function() {
  var result = Org.org_by_addr(org_data, '8.8.8.8');
  console.log('The result of synchronous method');
  console.log('Org.org_by_addr(org_data, \'8.8.8.8\')');
  result.forEach(function(str) {
    console.log(str);
  });
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);

setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('Org.org_by_domain(data, \'www.google.com\', callback)');
  Org.org_by_domain(org_data, 'www.google.com', function(err, org) {
    if (err) {throw err;}
    console.log(org);
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
  });
}, 2000);

var asn_data = geoip.open('/tmp/GeoIPASNum.dat');

setTimeout(function() {
  var result = Org.asn_by_addr(asn_data, '8.8.8.8');
  console.log('The result of synchronous method');
  console.log('Org.asn_by_addr(asn_data, \'8.8.8.8\')');
  console.log(result);
  console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 3000);

setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('Org.asn_by_domain(data, \'www.google.com\', callback)');
  Org.asn_by_domain(asn_data, 'www.google.com', function(err, asn) {
    if (err) {throw err;}
    asn.forEach(function(a) {
      var keys = Object.keys(a);
      console.log(a[keys[0]] + ' : ' + a[keys[1]]);
    });
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
  });
}, 4000);     
