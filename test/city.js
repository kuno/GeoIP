var assert = require('assert'), 
    geoip  = require('../index.js');

var City = geoip.City;
var city = new City('/tmp/GeoLiteCity.dat', true);

// Test record_by_domain method
var record = city.lookupSync('8.8.8.8');
assert.strictEqual(record.country_id, 'US', 'Google is not in US?');

city.lookup('www.google.com', function(data) {
    assert.strictEqual(record.country_id, data.country_id, 'oops! async not equal to sync!');
    assert.ok(city.close(), 'oops! err when closing city object!');
});
