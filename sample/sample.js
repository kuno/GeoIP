var geoip = require('geoip');


// Open the country data file
var Country = geoip.Country;
var country = new Country('/path/to/GeoIP.dat');

var country_obj = country.lookup('8.8.8.8');

console.log(country_obj);
/*
{ country_code: 'US',
  country_code3: 'USA',
  country_name: 'United States',
  continent_code: 'NA' }
*/

country.lookup('www.google.com', function(data) {
    if (data) { // no err, if not found, just return null
      console.log(data);  // same as lookup method
    }
});

//Close the opened file.
country.close();


// City
var City = geoip.City;
var city = new City('/path/to/GeoLiteCity.dat');

// Synchronous method
var city_obj = city.lookupSync('8.8.8.8');
console.log(city_obj);
// Return an object of city information
// {
//  "country_code":"US",
//  "country_code3":"USA",
//  "country_name":"United States",
//  "continet_code":"NA",
//  "region":"CA",
//  "city":"Mountain View",
//  "postal_code":"94043",
//  "latitude":37.41919999999999,
//  "longitude":-122.0574,
//  "dma_code":807,
//  "metro_code":807,
//  "area_code":650
//  }    

// Asynchronous method
city.lookup('www.google.com', function(data) {
    if (data) {
      console.log(data);
    }
});

city.close();


// Organization
var Org = geoip.Org;
var org = new Org('/path/to/file');  // Org module can open three edition database 'org', 'asnum', 'isp'

// Synchronous method
var org_str = org.lookup('8.8.8.8');

console.log(org_str);
/*
The type of returned data is string, for example:

'Genuity'
'AS15169 Google Inc.'

no longer an object
*/

org.close();

// Asynchronous method
org.lookup('www.google.com', function(data) {
    if (data) {
      console.log(data);
    }
});


// Region
var Region = geoip.Region;
var region = new Region('/path/to/GeoIPRegion.dat');

// Synchronous method
var region_obj = region.lookupSync('8.8.8.8'); 

console.log(region_obj);
/*
region object has two properties:
{ country_code: 'US', region: 'CO' }

*/

// Asynchronous method
region.lookup('www.google.com', function(data) {
    if (data) {
      console.log(data);
    }
});

region.close();


// NetSpeed
var NetSpeed = geoip.NetSpeed;
var netspeed = new NetSpeed('/path/to/GeoIPNetSpeed.dat');

// Synchronous method
var netspeed_str = netspeed.lookupSync('8.8.8.8');

console.log(netspeed_str);
/*
netspeed_str just a simple string, 'Dialup', 'Corprate'... so on
*/

// Asynchronous method
netspeed.lookup('www.google.com', function(data) {
    if (data) {
      console.log(data);  // Maybe return 'unknow' or different from lookup method
    }
});

netspeed.close();         
