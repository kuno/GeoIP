var geoip = requires('geoip');

/*
 * Country geo infomation
 * This package has the GeoIP country data file within it,
 * so you don't need to download.
 * However you can always get the latest versioin by:
 * wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz
 */
var data = geoip.open();

geoip.country.code_by_addr(data, '8.8.8.8'); // prints 'US'
geoip.country.name_by_addr(data, '8.8.8.8'); // prints  'USA'

geoip.country.code_by_domain(data, 'www.google.com', function(err, code) {
    if (err) {throw err;}
    console.log(code);  // prints 'US'
});
geoip.country.name_by_domain(data, 'www.google.com', function(err, name) {
    if (err) {throw err;}
    console.log(name);  // prints 'USA'
});

/*
 * City geo infomation
 *
 * Befor you can use city geo information,
 * you need download GeoLiteCity.dat file first.
 * wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz
 */

var data = geoip.open('/path/to/GeoLiteCity.dat');

geoip.city.record_by_addr(data, '8.8.8.8');
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

