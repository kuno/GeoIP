var geoip = require('geoip');


/*
 * Country geographic infomation
 *
 * Befor you can use geoip country information,
 * you need download GeoIP.dat file first. 
 * wget http://geolite.maxmind.com/download/geoip/country_database/GeoLiteCountry/GeoIP.dat.gz
 */
var country_data = geoip.open('/path/to/GeoIP.dat');

var Country = geoip.Country;

// Synchronous methods, network independence.
var code = Country.code_by_addr(country_data, '8.8.8.8'); // prints 'US'
var name = Country.name_by_addr(country_data, '8.8.8.8'); // prints  'United States'

// Asynchronous methods, depends on node's async-style dns module.
Country.code_by_domain(country_data, 'www.google.com', function(err, code) {
  if (err) {throw err;}
  console.log(code);  // prints 'US'
});

Country.name_by_domain(country_data, 'www.google.com', function(err, name) {
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

var City = geoip.City;

// Synchronous method
var record = City.record_by_addr(city_country_data, '8.8.8.8');
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

  // Asynchronous method
  City.record_by_domain(city_country_data, 'www.google.com', function(err, record) {
    if (err) {throw err;}
    console.log('is \'' + JSON.stringify(record) + '\'');
    var keys = Object.keys(record);
    keys.forEach(function(k) {
      console.log(k + ':' + record[k]);
    });
  });

  geoip.close(city_data);


  /*
   * Region geographic information
   *
   * Before you can use region geographic information,
   * you need buy GeoIPRegion.dat from maxmind.com
   */

  var region_country_data = geoip.open('/path/to/GeoIPRegion.dat');

  var Region = geoip.Region;

  // Synchronous method
  var reg = geoip.Region.region_by_addr(region_country_data, '8.8.8.8');  // prints 'US,CO'

  // Asynchronous method
  Region.region_by_domain(data, 'www.google.com', function(err, region) {
    if (err) {throw err;}
    console.log('is ' + region);
  });

  geoip.close(region_country_data);


  /*
   * Organization geographic information
   * Before you can use Organization geographic information,
   * you need but GeoIPOrg.dat data file from maxmind.com
   */
  // Get Organization    
  var org_data = geoip.open('/path/to/GeoIPOrg.dat');

  var Org = geoip.Org;

  // Synchronous method
  var org = Org.org_by_addr(org_data, '8.8.8.8');   
  // Return an array of the name of organization
  // [
  // 'Genuity',
  // 'The United Way',
  // 'Education Management Corporation,
  // 'International Generating Co. (Intergen)'
  // ]

  // Synchronous methos
  Org.org_by_domain(org_data, 'www.google.com', function(err, org) {
    if (err) {throw err;}
    if (typeof org === 'string') {
      console.log(org);  // Organization Not Found
    } else {  // Same as org_by_addr
      org.forEach(function(o) {
        console.log(o[0] + ' : ' + o[1]);
      });
    }
  });

  geoip.close(org_data);

  // Get ASNumber
  var asn_data = geoip.open('/path/to/GeoIPASNum.dat');

  // Synchronous method
  var asn = Org.asn_by_addr(org_data, '8.8.8.8');
  // Return an array of asn objects
  //[ { number: 'AS15169', description: 'Google Inc.' },
  //  { number: 'AS36597',
  //    description: 'OmniTI Computer Consulting Inc.' },
  //  { number: 'AS26471',
  //    description: 'Smart City Networks' } ]

  // Asynchronous method
  Org.asn_by_domain(asn_data, 'www.google.com', function(err, asn) {
    if (err) {throw err;}
    if (typeof asn === 'string') { 
      console.log(asn); // ASNumber Not Found
    } else {  // Same as asn_by_addr
    asn.forEach(function(a) {
      var keys = Object.keys(a);
      console.log(a[keys[0]] + ' : ' + a[keys[1]]);
    });
  }
  });

  geoip.close(asn_data);


  /*
   * NetSpeed information
   * Before you can use NetSpeed information,
   * you need to by netspeed data from maxmind.com
   */
  var NetSpeed = geoip.NetSpeed;

  var netspeed_data = geoip.open('/path/to/netspeed.dat');

  // Synchronous method
  var speed = Netspeed.speed_by_addr(netspeed_data, '8.8.8.8'); // prints 'Dailup'

  // Asynchronous method
  NetSpeed.speed_by_domain(data, 'www.google.com', function(err, speed) {
    if (err) {throw err;}
    console.log(speed);  // Maybe difference from speed_by_addr
  });

  geoip.close(netspeed_data);
