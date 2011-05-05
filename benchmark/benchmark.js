#!/usr/bin/env node

var geoip = require('../build/default/city.node');
var util = require('util');
var City = geoip.City;
var c = new City('/tmp/GeoLiteCity.dat', true);

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
for (var i = 0; i < 1; ++i) {
  // var addr = ip32();
  var addr = '255.255.255.255';
  console.log(addr);
  var l = c.lookup(addr);
  console.log(l);
}
var end = new Date().getTime();

util.puts((end - start) / 1000);
