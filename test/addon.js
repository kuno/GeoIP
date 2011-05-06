var assert = require('assert');

var geoip = require('../build/default/city.node');

var c = new geoip.City('/tmp/GeoLiteCity.dat');

var record = c.lookupSync('8.8.8.8');

console.log(record);

assert.ok(record, 'record not found.');

c.lookup('www.sina.com.cn', function(data) {
    assert.ok(data, 'record not found in async.');
    console.log(data);
    assert.deepEqual(record,data, 'oops, sync and async is not equal!');

    //assert.ok(c.close(), 'oops!');
});
