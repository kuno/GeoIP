#!/usr/bin/env node

var geoip = require('../index.js');
var util = require('util');
var City = geoip.City;
var c = new City('../database/GeoLiteCity.dat', true);

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
for (var j = 0; j < 10000; ++j) {
  var addr = ip32();
  //console.log(addr);
  c.lookupSync(addr); //, function(err, data) {
      //if (err) {throw err;}
      //if (data) {
      //  console.log(data);
      //}
 // });
}

var end = new Date().getTime();
util.puts((end - start) / 1000);
