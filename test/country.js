
var assert = require('assert'), 
    geoip = require('../index.js');

// Test name_by_addr method
var Country = geoip.Country;

var country = new Country('/tmp/GeoIP.dat', true);

var record = country.lookupSync('8.8.8.8');

assert.ok(record, 'can not find google');
