#!/usr/bin/env node

var path = require('path');
var util = require('util');
var geoip = require(path.resolve(__dirname, '../index.js'));
var City = geoip.City;
var NetSpeedCell = geoip.NetSpeedCell;
var c = new City(path.resolve(__dirname, '../database/GeoLiteCity.dat'), true);
var n = new NetSpeedCell(path.resolve(__dirname, '../database/GeoIPNetSpeedCell.dat'));


var ip32 = function() {
  var ip = '';
  for (var i = 0; i < 4; ++i) {
    ip += Math.floor(Math.random() * 256);
    if (i <= 2) {
      ip += '.';
    }
  }
  return ip;
};

var start = new Date().getTime();
for (var j = 0; j < 10000; ++j) {
  var addr = ip32();
  //console.log(addr);
  var data = c.lookupSync(addr); //, function(err, data) {
      //if (err) {throw err;}
      //if (data) {
        if (data && data.country_name === 'Italy') {
        console.log(addr);
        console.log(n.lookupSync(addr));
        }
      //}
 // });
}

var end = new Date().getTime();
util.puts((end - start) / 1000);
