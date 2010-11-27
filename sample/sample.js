var geoip = require('geoip');

/*
 * Country geo infomation
 *
 * Befor you can use geoip country information,
 * you need download GeoIP.dat file first. 
 * wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz
 */
var data = geoip.open('/path/to/GeoIP.dat');

// Synchronous methods, network independence.
geoip.Country.code_by_addr(data, '8.8.8.8'); // prints 'US'
geoip.Country.name_by_addr(data, '8.8.8.8'); // prints  'United States'

// Asynchronous methods, depends on node's async-style dns module.
geoip.Country.code_by_domain(data, 'www.google.com', function(err, code) {
    if (err) {throw err;}
    console.log(code);  // prints 'US'
});
geoip.Country.name_by_domain(data, 'www.google.com', function(err, name) {
    if (err) {throw err;}
    console.log(name);  // prints 'United States'
});

// Set the all properties of an existing data object to undefined
geoip.close(data);
/*
 * City geo infomation
 *
 * Befor you can use city geo information,
 * you need download GeoLiteCity.dat file first.
 * wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz
 */

var data = geoip.open('/path/to/GeoLiteCity.dat');

geoip.City.record_by_addr(data, '8.8.8.8');  // output this;
                                                  //{ 
                                                  //country_code: 'US',
                                                  //country_code3: 'USA',
                                                  //country_name: 'United States',
                                                  //continet_code: 'NA',
                                                  //region: 'CA',
                                                  //city: 'Mountain View',
                                                  //postal_code: '94043',
                                                  //latitude: 37.41919999999999,
                                                  //longitude: -122.0574,
                                                  //dma_code: 807,
                                                  //metro_code: 807,
                                                  //area_code: 650 
                                                  //}

