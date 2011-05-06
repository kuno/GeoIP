var assert = require('assert');

var binding = require('../build/default/geoip.node');

var c = new binding.City('/tmp/GeoLiteCity.dat');

var record = c.lookupSync('8.8.8.8');

console.log(record);

assert.ok(record, 'record not found.');

c.lookup('www.google.com', function(data) {
    assert.ok(data, 'record not found in async.');
    console.log(data);
    assert.deepEqual(record, data, 'oops, sync and async is not equal!');

    assert.ok(c.close(), 'oops!');
});
