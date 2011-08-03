
var assert = require('assert'), 
    geoip = require('../index.js');

var Country6 = geoip.Country6;
var country6 = new Country6('../database/GeoIPv6.dat', true);

var sync_data6 = country6.lookupSync('www.google.com');
assert.ok(sync_data6, 'Can not find google domain in country ipv6 module');

var sync_data6 = country6.lookupSync('2406:a000:f0ff:fffe::122d');
assert.ok(sync_data6, 'Can not find google ip in country ipv6 database');

console.log(sync_data6);
country6.lookup('2607:f0d0:1002:0051:0000:0000:0000:0004', function(err, data6) {
    if (err) {throw err;}
    if (data6) {
      console.log(data6);
    }
});

assert.ok(country6.update('../database/GeoIPv6.dat'), 'Oops when updating country ipv6 database');
