var mocha = require('mocha');
var chai = require('chai');
var path = require('path');
var geoip = require('../index.js');

var Country = geoip.Country;
var file = path.resolve('../database/GeoIP.dat');
console.log(file);
var country = new Country(file, true);
// 
// var sync_data = country.lookupSync('8.8.8.8');
// assert.ok(sync_data, 'Can not find google in country module');
// 
// console.log(sync_data);
// 
// country.lookup('www.google.com', function(err, data) {
//   if (err) {throw err;}
//   if (data) {
//     assert.deepEqual(sync_data, data, 'Oops! Async and sync country data not equal');
//   } else {
//     console.log('Data not found');
//   }
// });
// 
// assert.ok(country.update('../database/GeoIP.dat'), 'Oops when updating country database');
