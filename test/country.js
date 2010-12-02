#!/usr/bin/env node

var geoip = require('../geoip.js');


var data = geoip.open('/tmp/GeoIP.dat');

console.log(geoip.Country.name_by_addr(data, '8.8.8.8'));

console.log(geoip.Country.code_by_addr(data, '8.8.8.8'));



