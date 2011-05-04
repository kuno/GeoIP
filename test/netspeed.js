var assert = require('assert'),
    geoip  = require('../index.js');

var NetSpeed = geoip.NetSpeed,
    // Speed classes
    classes = ['Unknown', 'Dailup', 'Cable', 'Corporate'],
    data, speed;

// Aynchronous 
geoip.filter('/tmp/GeoIPNetSpeed.dat', function(err, type, data) {
    assert.strictEqual('netspeed', type, 'Data type is not netspeed');
    speed = NetSpeed.speed_by_addr(data, '8.8.8.8');
    assert.ok(classes.indexOf(speed), 'Unrecognized speed class');
});

// Synchronous
data = geoip.open('/tmp/GeoIPNetSpeed.dat');  
assert.strictEqual('netspeed', geoip.check(data), 'Data type is not netspeed');
NetSpeed.speed_by_domain(data, 'www.sina.com', function(err, speed) {
    if (err) {throw err;}
    assert.ok(classes.indexOf(speed), 'Unrecognized speed class');
});
