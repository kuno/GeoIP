var read  = require('fs').readFileSync,
    join  = require('path').join;

var binding = require('./geoip.node'),
    City    = require('./lib/city.js');

exports.Country = binding.Country;
exports.City    = City;

exports.version = JSON.parse(read(join(__dirname, 'package.json'))).version;
