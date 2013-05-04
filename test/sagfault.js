#!/usr/bin/env node

var path = require('path');
var file = path.resolve(__dirname, '../database/GeoLiteCity.dat');
var geoip = path.resolve(__dirname, '..');
var city = new (require(geoip).City)(file);

for(var j = 100; --j;){
  var ip = [1,1,1,1].map(function(){return 0|Math.random()*256}).join('.');
  console.log(ip);
  
  console.log(city.lookupSync(ip));
}
