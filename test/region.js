
var assert = require('assert'), 
    geoip = require('../index.js');

var Region = geoip.Region;
var region = new Region('/tmp/GeoIPRegion.dat', true);

var sync_data = region.lookupSync('www.google.com');
assert.ok(sync_data, 'Can not find google in region module');
region.lookup('8.8.8.8', function(data) {
    if (data) {
      //console.log(sync_data, ' vs ' , data);
      assert.strictEqual(sync_data, data, 'Oops! Async and sync region data not equal');
    } else {
      console.log('Data not found');
    }

    assert.ok(region.close(), 'Oops when closing region object');
});
