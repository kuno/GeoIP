#!/usr/bin/env node

var geoip = require('../geoip.js');


var data = geoip.open('../data/GeoIP.dat');

console.log(data.buffer);



