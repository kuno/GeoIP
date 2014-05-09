var path  = require('path'),
    read  = require('fs').readFileSync;

var version  = JSON.parse(read(path.resolve(__dirname, './package.json'))).version;

var binding = require('./lib/native');

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
exports.check       = binding.check;
exports.isString    = binding.isString;
exports.utils       = {
    check: binding.check,
    isString: binding.isString
}

// Versions
exports.version  = 'v' + version;
exports.libgeoip = 'v' + binding.libgeoip;
exports.versions = {
    'geoip': version,
    'libgeoip': binding.libgeoip
};
