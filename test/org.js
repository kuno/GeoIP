
var assert = require('assert'), 
    geoip = require('../index.js');

var Org = geoip.Org;
var org = new Org('/tmp/GeoIPOrg.dat', true);

var sync_data = org.lookupSync('www.google.com');
assert.ok(sync_data, 'Can not find google in org module');

org.lookup('8.8.8.8', function(data) {
    if (data) {
      assert.deepEqual(sync_data, data, 'Oops! Async and sync org data not equal');
    } else {
      console.log('Data not found');
    }

    assert.ok(org.close(), 'Oops when closing org object');
});
