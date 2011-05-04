var assert = require('assert'), 
    geoip  = require('../index.js');

var City = geoip.City;
var city = new City('/tmp/GeoLiteCity.dat', true);

// Test record_by_domain method
var record = city.lookup('8.8.8.8');
assert.strictEqual(record.country_id, 'US');
