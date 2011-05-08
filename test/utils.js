var assert = require('assert'),
    check  = require('../index.js').check;

// check data base edition
assert.strictEqual('netspeed', check('/tmp/GeoIPNetSpeed.dat'), 'db is not netspeed edition');
assert.strictEqual('asnum', check('/tmp/GeoIPASNum.dat'), 'db is not asnum edition');
assert.strictEqual('country',check('/tmp/GeoIP.dat'), 'db is not coutnry edition');
assert.strictEqual('region', check('/tmp/GeoIPRegion.dat'), 'db is not region edition');
assert.strictEqual('city', check('/tmp/GeoLiteCity.dat'), 'db is not city edition');
assert.strictEqual('org', check('/tmp/GeoIPOrg.dat'), 'db is not org edition');

assert.strictEqual('country', check('/tmp/GeoIPCountryWhois.csv'), 'not support csv format');
