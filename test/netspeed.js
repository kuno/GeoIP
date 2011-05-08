
var assert = require('assert'), 
    geoip = require('../index.js');

var NetSpeed = geoip.NetSpeed;
var netspeed = new NetSpeed('/tmp/GeoIPNetSpeed.dat', true);

var sync_data = netspeed.lookupSync('www.google.com');
assert.ok(sync_data, 'Can not find google in country module');

netspeed.lookup('8.8.8.8', function(data) {
    if (data) {
      assert.strictEqual(sync_data, data, 'Oops! Async and sync netspeed data not equal.');
    } else {
      console.log('Data not found');
    }
    assert.ok(netspeed.close(), 'Oops when closing netspeed object');
});
