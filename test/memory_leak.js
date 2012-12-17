#!/usr/bin/env node

var path = require('path');
var geoip = require(path.resolve(__dirname, '../index'));

var file = path.resolve(__dirname, '../database/GeoLiteCity.dat');

console.log(file);

var city = new geoip.City(file);

console.log(city.lookupSync('8.8.8.8'));