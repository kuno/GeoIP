#!/usr/bin/env node

var assert = require('assert');
var binding = require('../build/default/geoip.node');
var util = require('util');
var Test = binding.Test;
var c = new Test('../database/GeoLiteCity.dat', true);

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
for (var i = 0; i < 10000; i++) {
  var addr = ip32();
  //console.log(addr);
  var l = c.lookupSync(addr);
 // console.log(l);
}
var end = new Date().getTime();

util.puts((end - start) / 1000);


/*
var start_asyc = new Date().getTime();
var addr = '0.0.0.0';
c.lookup(addr, function(err, data) {
  if (err) {throw err;}
  if (data) {
    console.log(data);
  }
});
*/
//var end_async = new Date().getTime();

//util.puts((end_async - start_asyc) / 1000);
assert.ok(c.close(), 'Error when closing country object');
assert.ok(c, 'object has been deleted');
console.log(c);
