#!/usr/bin/env node

var path = require('path');

var geoip = require('../index.js');
var file = path.resolve(__dirname, '../database/GeoLiteCity.dat');
var geoIpCity = new geoip.City(file);
var ipAddress = "199.182.212.174";

console.log(geoIpCity.lookupSync(ipAddress));

geoIpCity.lookup(ipAddress, function(err, location) {
  console.log(err, location);
});
