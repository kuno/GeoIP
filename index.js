var read  = require('fs').readFileSync,
    join  = require('path').join;

var City = require('./lib/city.js');

exports.City  = City;

exports.version = JSON.parse(read(join(__dirname, 'package.json'))).version;
