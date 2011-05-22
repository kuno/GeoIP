
var assert = require('assert'), 
    geoip = require('../index.js');

var Country = geoip.Country;
var country = new Country('/tmp/GeoIP.dat', true);

var sync_data = country.lookupSync('www.google.com');
assert.ok(sync_data, 'Can not find google in country module');

/*
//console.log(sync_data);
country.lookup('8.8.8.8', function(err, data) {
    if (err) {throw err;}
    if (data) {
      assert.deepEqual(sync_data, data, 'Oops! Async and sync country data not equal');
    } else {
      console.log('Data not found');
    }
    assert.ok(country.close(), 'Oops when closing country object');
});*/

var country6 = new Country('/tmp/GeoIPv6.dat');
var sync_data6 = country6.lookupSync6('2406:a000:f0ff:fffe::122d');
assert.ok(sync_data6, 'Can not find google in country ipv6 database');

console.log(sync_data6);
country6.lookup6('2607:f0d0:1002:0051:0000:0000:0000:0004', function(err, data6) {
    //if (err) {throw err;}
    if (data6) {
      //console.log(data6);
      // assert.deepEqual(sync_data6, data6, 'Oops! Async and sync country data ipv6 not equal');
    }

    assert.ok(country6.close(), 'Oops when closing country object');
});
