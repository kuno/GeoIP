var path  = require('path'),
    read  = require('fs').readFileSync;

var binding = require('bindings')('native.node');

var version  = JSON.parse(read(path.resolve(__dirname, './package.json'))).version;

// Native classes
exports.native = binding;

// Libraries
exports.NetSpeedCell = require('./lib/netspeedcell');
exports.NetSpeed     = require('./lib/netspeed');
exports.Country6     = require('./lib/country6');
exports.Country      = require('./lib/country');
exports.Region       = require('./lib/region');
exports.City6        = require('./lib/city6');
exports.City         = require('./lib/city');
exports.Org          = require('./lib/org');

// Utilities
exports.check    = binding.check;

// Versions
exports.version  = 'v' + version;
exports.libgeoip = 'v' + binding.libgeoip;
