
var assert = require('assert'), 
    geoip = require('../index.js');

var Region = geoip.Region;
var region = new Region('../database/GeoIPRegion-515.dat', true);

var sync_data = region.lookupSync('8.8.8.8');
assert.ok(sync_data, 'Can not find google in region module');

region.lookup('www.google.com', function(err, data) {
    if (err) {throw err;}
    if (data) {
      console.log(sync_data, ' vs ' , data);
      assert.ok(data, 'Oops! Async and sync region data not equal');
    } else {
      console.log('Data not found');
    }
});

assert.ok(region.update('../database/GeoIPRegion-515.dat'), 'Oops when updating region database');
