#!/usr/bin/env node

var geoip = require('../index.js');
var util = require('util');
var NetSpeed = geoip.NetSpeed;
var n = new NetSpeed('../database/GeoIPNetSpeed.dat', true);

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
for (var i = 0; i < 100000; ++i) {
  var addr = ip32();
  //console.log(addr);
  var l = n.lookupSync(addr);
  console.log(l);
}
var end = new Date().getTime();

util.puts((end - start) / 1000);
