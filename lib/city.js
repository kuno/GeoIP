/* 
 *	geoip.js - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */

 var dns = require('dns');

// Grab the C++ bindings
var binding = require('../build/default/city.node');

var City = Object.create(binding.City);

module.exports = City;
