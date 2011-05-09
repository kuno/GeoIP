#!/usr/bin/env node

var binding = require('../index.js');
var util = require('util');
var Org = binding.Org;
var o = new Org('/tmp/GeoIPOrg.dat', true);

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
for (var o = 0; i < 1; ++i) {
  var addr = ip32();
  //console.log(addr);
  var l = o.lookupSync(addr);
  //console.log(l);
}
var end = new Date().getTime();

util.puts((end - start) / 1000);
