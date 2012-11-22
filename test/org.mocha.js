
var assert = require('assert'), 
    geoip = require('../index.js');

var Org = geoip.Org;
var org = new Org('../database/GeoIPASNum.dat', true);

var sync_data = org.lookupSync('8.8.8.8');
console.log(sync_data);
//assert.ok(sync_data, 'Can not find google in org module');

org.lookup('www.google.com', function(err, data) {
    if (err) {throw err;}
    if (data) {
      console.log(data);
      assert.deepEqual(sync_data, data, 'Oops! Async and sync org data not equal');
    } else {
      console.log('Data not found');
    }
});

assert.ok(org.update('../database/GeoIPASNum.dat'), 'Oops when updating org database');
