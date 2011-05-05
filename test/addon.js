var assert = require('assert');

var geoip = require('../build/default/city.node');

var c = new geoip.City('/tmp/GeoLiteCity.dat', true);

var record = c.lookupSync('114.82.0.29');

c.lookup('114.82.0.29', function(data) {
    assert.deepEqual(record,data, 'oops, sync and async is not equal!');
});
