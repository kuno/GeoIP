var fs    = require('fs'),
    path  = require('path');

var City = require('./lib/city.js');

exports.City  = City;

exports.version = JSON.parse(fs.readFileSync(path.join(__dirname, 'package.json'))).version;
