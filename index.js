var read  = require('fs').readFileSync,
    join  = require('path').join;

var binding = require('./build/default/geoip.node');

var version  = JSON.parse(read(join(__dirname, 'package.json'))).version;

// Modules
exports.NetSpeed = binding.NetSpeed;
exports.Country6 = binding.Country6;
exports.Country  = binding.Country;
exports.Region   = binding.Region;
exports.City6    = binding.City6;
exports.City     = binding.City;
exports.Org      = binding.Org;


// Utilities
exports.check    = binding.check;

// Versions
exports.version  = 'v' + version; 
exports.libgeoip = 'v' + binding.libgeoip; // only works on libgeoip >= 1.4.7
