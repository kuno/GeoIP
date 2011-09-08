
var assert = require('assert'), 
    geoip = require('../index.js');

var City6 = geoip.City6;
var city6 = new City6('../database/GeoLiteCityv6.dat', true);

var sync_data6 = city6.lookupSync('www.google.com');
assert.ok(sync_data6, 'Can not find google domain in city ipv6 module');

var sync_data6 = city6.lookupSync('2406:a000:f0ff:fffe::122d');
assert.ok(sync_data6, 'Can not find google ip in city ipv6 database');

console.log(sync_data6);
city6.lookup('2607:f0d0:1002:0051:0000:0000:0000:0004', function(err, data6) {
    if (err) {throw err;}
    if (data6) {
      console.log(data6);
    }
});

assert.ok(city6.update('../database/GeoLiteCityv6.dat'), 'Oops when updating city ipv6 database');
