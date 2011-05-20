#!/usr/bin/env node

var geoip = require('../index.js');
var util = require('util');
var Country = geoip.Country;
var c = new Country('/tmp/GeoIP.dat', true);

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

var ip128 = function() {
  var ip = '', hex;
  for (var i = 0; i < 8; i++) {
    hex = (Math.floor(Math.random() * (Math.pow(2, 16)))).toString(16);
    ip += (hex.length = 4) ? hex : (hex + '0');
    if (i <= 6) {
      ip += ':';
    }
  }

  return ip
}

/*
var start = new Date().getTime();
for (var i = 0; i < 100000; ++i) {
  var addr = '109.161.37.182'; //ip32();
 // console.log(addr);
  var l = c.lookupSync(addr);
//  console.log(l);
}
var end = new Date().getTime();

util.puts((end - start) / 1000);
*/

var c6 = new Country('/tmp/GeoIPv6.dat');

var start6 = new Date().getTime();
for (var j = 0; j < 10000; j++) {
  var addr6 = ip128();
  //console.log(addr6);
  var l6 = c6.lookupSync6(addr6);
  if (l6) { console.log('addr6 is ' + addr6); console.log(l6);}
}
var end6 = new Date().getTime();

util.puts((end6 - start6) / 1000);
