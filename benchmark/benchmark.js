#!/usr/bin/env node

var geoip = require('../index.js');
var util = require('util');
var city = geoip.City;
city.open(true, '/tmp/GeoLiteCity.dat');

var ip32 = function() {
  var ip = '';
  for (var i = 0; i < 4; ++i) {
    ip += Math.floor(Math.random() * 256);
    if (i <= 2) {
      ip += '.';
    }
  }
  return ip;
}

var start = new Date().getTime();
for (var i = 0; i < 3; ++i) {
  var ip4 = ip32();
  console.log(city.lookup((''+ip4)));
}
var end = new Date().getTime();

util.puts((end - start) / 1000);
