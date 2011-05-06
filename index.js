var read  = require('fs').readFileSync,
    join  = require('path').join;

var binding = require('./build/default/geoip.node');

exports.Country = binding.Country;
exports.City  = binding.City;

exports.version = JSON.parse(read(join(__dirname, 'package.json'))).version;
