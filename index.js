var path  = require('path'),
    read  = require('fs').readFileSync;

var binding = require('bindings')('native.node');

var version  = JSON.parse(read(path.resolve(__dirname, './package.json'))).version;

// Classes
exports.NetSpeed = binding.NetSpeed;
exports.Country6 = binding.Country6;
exports.Country  = binding.Country;
exports.Region   = binding.Region;
exports.City6    = binding.City6;
exports.City     = binding.City;
exports.Org      = binding.Org;

exports.City     = require('./lib/city');

// Utilities
exports.check    = binding.check;

// Versions
exports.version  = 'v' + version;
exports.libgeoip = 'v' + binding.libgeoip; // only works on libgeoip >= 1.4.7
