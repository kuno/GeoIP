var read  = require('fs').readFileSync,
    join  = require('path').join;

var binding = require('./build/default/geoip.node');

// Class
exports.NetSpeed = binding.NetSpeed;
exports.Country  = binding.Country;
exports.Region   = binding.Region;
exports.City     = binding.City;
exports.Org      = binding.Org;


// Utility
exports.check    = binding.check;
exports.version  = JSON.parse(read(join(__dirname, 'package.json'))).version;
