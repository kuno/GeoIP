/* 
 *	geoip.js - node.JS to C++ glue code for the GeoIP C library
 *	Written by Joe Vennix on March 15, 2011
 *	For the GeoIP C library, go here: http://www.maxmind.com/app/c
 *		./configure && make && sudo make install
 */

// Grab the C++ bindings
var geoip = require('../build/default/city.node');

// Make this a class, so that you can say `var geo = require('geoip')`
function GeoIP() { this.isOpen = false; }

// Finds and opens the database file. Example usage:
//		geo.open({cache:true, filename:'./GeoLiteCity.dat'});
GeoIP.prototype.open = function(options) {
	// Set up the default options (cache:true, filename:GeoLiteCity.dat)
	if (options.cache === null) 
		options.cache = true;
	if (options.filename === null) 
		options.filename = 'GeoLiteCity.dat';
	if (this.isOpen)
		this.close();
	// Open up the database.
	if (geoip.open(options.cache, options.filename))
		this.isOpen = true;
};

// Grabs the latitude/longitude and returns it as an array of
//  	form [latitude, longitude]. Returns [0,0] on error (call open()).
GeoIP.prototype.lookup = function(ip) {
	if (this.isOpen) {
		return geoip.lookup(ip);
	}
	return [0, 0];
};

// Closes the database and destroys the cache
GeoIP.prototype.close = function() {
	if (this.isOpen) {
		geoip.close();
		this.isOpen = false;
	}
}

module.exports = new GeoIP();
