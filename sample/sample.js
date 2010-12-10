var geoip = require('geoip');


/*
 * Country geographic infomation
 *
 * Befor you can use geoip country information,
 * you need download GeoIP.dat file first. 
 * wget http://geolite.maxmind.com/download/geoip/country_database/GeoLiteCountry/GeoIP.dat.gz
 */
var country_data = geoip.open('/path/to/GeoIP.dat');

// Synchronous methods, network independence.
geoip.Country.code_by_addr(country_data, '8.8.8.8'); // prints 'US'
geoip.Country.name_by_addr(country_data, '8.8.8.8'); // prints  'United States'

// Asynchronous methods, depends on node's async-style dns module.
geoip.Country.code_by_domain(country_data, 'www.google.com', function(err, code) {
  if (err) {throw err;}
  console.log(code);  // prints 'US'
});
geoip.Country.name_by_domain(country_data, 'www.google.com', function(err, name) {
  if (err) {throw err;}
  console.log(name);  // prints 'United States'
});

// Close the opened file
geoip.close(country_data);


/*
 * City geographic infomation
 *
 * Befor you can use city geographic information,
 * you need download GeoLiteCity.dat file first.
 * wget http://geolite.maxmind.com/download/geoip/country_database/GeoLiteCity.dat.gz
 */

var city_country_data = geoip.open('/path/to/GeoLiteCity.dat');

geoip.City.record_by_addr(city_country_data, '8.8.8.8');
// Return a Object of city information
//'{
  //"country_code":"US",
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
  //  }'

  geoip.close(city_data);


  /*
   * Region geographic information
   *
   * Before you can use region geographic information,
   * you need buy GeoIPRegion.dat from maxmind.com
   */

  var region_country_data = geoip.open('/path/to/GeoIPRegion.dat');

  geoip.Region.region_by_addr(region_country_data, '8.8.8.8');  // prints 'US,CO'

  geoip.close(region_country_data);


  /*
   * Organization geographic information
   * Before you can use Organization geographic information,
   * you need but GeoIPOrg.dat data file from maxmind.com
   */

  var org_data = geoip.open('/path/to/GeoIPOrg.dat');

  // Get Organization
  geoip.Org.org_by_addr(org_data, '8.8.8.8');   
  // Return an array of the name of organization
  // [
  // 'Genuity',
  // 'The United Way',
  // 'Education Management Corporation,
  // 'International Generating Co. (Intergen)'
  // ]



  geoip.close(org_data);

  // Get ASNumber
  var asn_data = geoip.open('/path/to/GeoIPASNum.dat');

  geoip.Org.asn_by_addr(org_data, '8.8.8.8');
  // Return an array of asn objects
  //[ { number: 'AS15169', description: 'Google Inc.' },
  //  { number: 'AS36597',
  //    description: 'OmniTI Computer Consulting Inc.' },
  //  { number: 'AS26471',
  //    description: 'Smart City Networks' } ]

  geoip.close(asn_data);


  /*
   * NetSpeed information
   * Before you can use NetSpeed information,
   * you need to by netspeed data from maxmind.com
   */

  var netspeed_data = geoip.open('/path/to/netspeed.dat');

  geoip.Netspeed.speed_by_addr(netspeed_data, '8.8.8.8'); // prints 'Dailup'

  geoip.close(netspeed_data);
