#!/usr/bin/env node

var geoip = require('../index.js');
var util = require('util');
var Country6 = geoip.Country6;
var c6 = new Country6('../database/GeoIPv6.dat', true);

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

var start6 = new Date().getTime();
for (var i = 0; i < 100000; i++) {
  var addr6 = ip128();
  //console.log(addr6);
  var l6 = c6.lookupSync(addr6);
  if (l6) { console.log('addr6 is ' + addr6); console.log(l6);}
}
var end6 = new Date().getTime();

util.puts((end6 - start6) / 1000);

c6.update('/home/kuno/GeoIPv6.dat');

start6 = new Date().getTime();
for (var j = 0; j < 100000; j++) {
  var addr6 = ip128();
  //console.log(addr6);
  var l6 = c6.lookupSync(addr6);
  if (l6) { console.log('addr6 is ' + addr6); console.log(l6);}
}
end6 = new Date().getTime();

util.puts((end6 - start6) / 1000);   
