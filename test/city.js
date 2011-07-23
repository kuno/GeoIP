var assert = require('assert'), 
    geoip  = require('../index.js');

var City = geoip.City;
var city = new City('/tmp/GeoLiteCity.dat', true);

// Test record_by_domain method
var sync_data = city.lookupSync('www.google.com');
console.log(sync_data);
assert.ok(sync_data, 'Can not find google?');

city.lookup('www.google.org', function(err, data) {
    if (err) {throw err;}
    if (data) {
      console.log(data);
      assert.deepEqual(sync_data, data, 'oops! Async and sync city data not equal');
    } else {
      console.log('Data not found');
    }
});

assert.ok(city.update('/tmp/GeoLiteCity.dat'), 'Oops when updating city database'); 
