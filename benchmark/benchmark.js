#!/usr/bin/env node

var geoip = require('../index.js');
var util = require('util');
var city = geoip.City;
city.open({cache:true, filename:'/tmp/GeoLiteCity.dat'});

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
for (var i = 0; i < 2; ++i) {
  var addr = ip32();
  console.log(addr);
  var l = city.lookup(addr);
  console.log(l);
}
var end = new Date().getTime();

util.puts((end - start) / 1000);
