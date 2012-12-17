var geoip = require('geoip');

// !! Important !!
// Befor you can use this package, you need to download or buy some data fro http://www.maxmind.com/app/ip-location.
// There are some free databases among some commercial versions, the free databases can be found http://geolite.maxmind.com/download/geoip/database/.


////////////////////////////////////////////////////////////////////////////////
// Country
////////////////////////////////////////////////////////////////////////////////
/////////////////////////
// ipv4 address lookup //
/////////////////////////
var Country = geoip.Country;
var country = new Country('/path/to/GeoIP.dat');

// Synchronous method(the recommended way):
var country_obj = country.lookupSync('8.8.8.8');

console.log(country_obj);
/*
{ country_code: 'US',
  country_code3: 'USA',
  country_name: 'United States',
  continent_code: 'NA'
}
*/

// Asynchronous method:
country.lookup('www.google.com', function(err, data) {
    if (err) {throw err;}
    if (data) { // if not found, just return null
      console.log(data);  // same as lookup method
    }
});

/////////////////////////
// ipv6 address lookup //
/////////////////////////
var Coutry6 = geoip.Country6;
var country_v6 = new Country6('/path/to/GeoIPv6.dat');

// Synchronous method
var country_obj_v6 = country_v6.lookupSync('2607:f0d0:1002:0051:0000:0000:0000:0004');

console.log(country_obj_v6);
/*
{ country_code: 'US',
  country_code3: 'USA',
  country_name: 'United States',
  continent_code: 'NA'
}
*/

country_v6.update('/path/to/new/country/ipv6/database');

// Asynchronous method
country_v6.lookup('2400:2352:b0f1:36c5:aa9d:694a:2f98:40bd', function(err, data_v6) {
    if (err) {throw err;}
    if (data_v6) {
      console.log(data_v6);
      /*
      {
        country_code: 'JP',
        country_code3: 'JPN',
        country_name: 'Japan',
        continent_code: 'AS'
      }
      */
    }
});


////////////////////////////////////////////////////////////////////////////////
// City
////////////////////////////////////////////////////////////////////////////////
//////////////////////////
// ipv4 address lookup ///
//////////////////////////
var City = geoip.City;
var city = new City('/path/to/GeoLiteCity.dat');

// Synchronous method
var city_obj = city.lookupSync('8.8.8.8');
console.log(city_obj);
/*
Return an object of city information
{
  "country_code":"US",
  "country_code3":"USA",
  "country_name":"United States",
  "continet_code":"NA",
  "region":"CA",
  "city":"Mountain View",
  "postal_code":"94043",
  "latitude":37.41919999999999,
  "longitude":-122.0574,
  "dma_code":807,
  "metro_code":807,
  "area_code":650
}
*/

city.update('/path/to/new/GeoLiteCity.dat');

// Asynchronous method
city.lookup('www.google.com', function(err, data) {
    if (err) {throw err;}
    if (data) {
      console.log(data);
    }
});

/////////////////////////
// ipv6 address lookup //
/////////////////////////
var City6 = geoip.City6;
var city6 = new City6('/path/to/GeoLiteCityv6.dat');

// Synchronous method
var city6_obj = city6.lookupSync('2002:a46f:f395:14de:f876:542b:41cb:5bbe');
console.log(city6_obj);
/*
Return an object of city information
{ country_code: 'US',
  country_code3: 'USA',
  country_name: 'United States',
  region: 'AL',
  city: 'Birmingham',
  postal_code: '35294',
  latitude: 33.54460144042969,
  longitude: -86.92919921875,
  continent_code: 'NA' }
*/

city6.update('/path/to/new/GeoLiteCityv6.dat');

// Asynchronous method
city6.lookup('2001:b565:487a:c05d:5f07:5662:c3cd:cfcf', function(err, data) {
    if (err) {throw err;}
    if (data) {
      console.log(data);
      /*
      { country_code: 'TW',
        country_code3: 'TWN',
        country_name: 'Taiwan',
        latitude: 23.5,
        longitude: 121,
        continent_code: 'AS'
      }
      */
    }
});


////////////////////////////////////////////////////////////////////////////////
// Organization
////////////////////////////////////////////////////////////////////////////////
var Org = geoip.Org;
var org = new Org('/path/to/database');  // Org module can open three edition database 'org', 'asnum', 'isp'

// Synchronous method
var org_str = org.lookup('8.8.8.8');

console.log(org_str);
/*
The type of returned data is string, for example:

'Genuity'
'AS15169 Google Inc.'

no longer an object
*/

org.update('/path/to/new/database');

// Asynchronous method
org.lookup('www.google.com', function(err, data) {
    if (err) {throw err;}
    if (data) {
      console.log(data);
    }
});


////////////////////////////////////////////////////////////////////////////////
// Region
////////////////////////////////////////////////////////////////////////////////
var Region = geoip.Region;
var region = new Region('/path/to/GeoIPRegion.dat');

// Synchronous method
var region_obj = region.lookupSync('8.8.8.8');

console.log(region_obj);
/*
region object has two properties:
{ country_code: 'US', region: 'CO' }

*/

region.update('/path/to/new/GeoIPRegion.dat');

// Asynchronous method
region.lookup('www.google.com', function(data) {
    if (data) {
      console.log(data);
    }
});


////////////////////////////////////////////////////////////////////////////////
// NetSpeed
////////////////////////////////////////////////////////////////////////////////
var NetSpeed = geoip.NetSpeed;
var netspeed = new NetSpeed('/path/to/GeoIPNetSpeed.dat');

// Synchronous method
var netspeed_str = netspeed.lookupSync('8.8.8.8');

console.log(netspeed_str);
/*
netspeed_str just a simple string, 'Dialup', 'Corprate'... so on
*/

netspeed.update('/path/to/new/netspeed/database');

// Asynchronous method
netspeed.lookup('www.google.com', function(err, data) {
    if (err) {throw err;}
    if (data) {
      console.log(data);  // Maybe return 'unknow' or different from lookup method
    }
});
