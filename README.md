__GeoIP binding for nodejs__

[![NPM version](https://badge.fury.io/js/geoip.png)](http://badge.fury.io/js/geoip)
[![Build status](https://secure.travis-ci.org/kuno/GeoIP.png)](http://travis-ci.org/kuno/GeoIP)
[![Dependencies Status](https://david-dm.org/kuno/GeoIP.png)](https://david-dm.org/kuno/GeoIP)
[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/kuno/geoip/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

Get geolocation information based on domain or IP address.

**Pure javascript implementation**

If you are looking for a pure javascript implementation, the [`node-geoip`](https://github.com/bluesmoon/node-geoip) maybe your choice.

###Technical Architecture

![new_architecture](https://github.com/kuno/GeoIP/raw/master/misc/new_architecture.png)

###Memory leak issue
As up to the version 0.4.7 (commit [22dadd98646f75318412b54b3de90cc079d34b1d](https://github.com/kuno/GeoIP/commit/22dadd98646f75318412b54b3de90cc079d34b1d)), there will be some memory leak when new a object from GeoIP class.

Here is the [report](https://gist.github.com/4357339), I'll update it frequently.

Fortunately, the number of leaked bytes is not huge, and you wont new a object very offen.

If you are willing to fix this but, please fork this project.

###Need support special database?
Corrently, this package supports the most common databases from MaxMind.

But, it still a lot of special databases out there.

Since I am not a user of any product/service of maxmind, if you require this pacakge to support special db, please make sure you can delivery the testing db and the ip address/domain that can be used to against the db.


##Installation

### for nodejs >= 0.8.x

    [sudo] npm install geoip [-g]

### for nodejs 0.6.x

    [sudo] npm install geoip@0.4.5 [-g]

### for nodejs 0.4.x

    [sudo] npm install geoip@0.4.4 [-g]

### for nodejs older than 0.4.0

From v0.4.0, geoip need nodejs >= 0.4.0, if you want to use it on old nodejs, you can:

    npm install geoip@0.3.4-1


##Usage

###Check edition

    var edition = geoip.check('/path/to/file');

    console.log(edition); // output 'country', 'city', 'org'... so on

###Create a new instance of sub-module, for example:

    var city = new geoip.City('/path/to/GeoLiteCity.dat', false);  // not to cache the database

    var city = new geoip.City('/path/to/GeoLiteCity.dat');  // the default option is cache

###Update database on the fly:

    city.update('/path/to/new/GeoLiteCity.dat');

##Modules

###Country

ipv4 address lookup

    // Open the country data file
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

ipv6 address lookup (Currently only Country and City module supports ipv6)

    // Open the ipv6 edition of country module
    var Country6 = geoip.Country6;

    var country_v6 = new Country6('/path/to/GeoIPv6.dat');

    // Synchronous method
    var country_obj_v6 = country_v6.lookupSync('2607:f0d0:1002:0051:0000:0000:0000:0004');

    console.log(country_obj_v6); // Same as ipv4
    /*
      { country_code: 'US',
        country_code3: 'USA',
        country_name: 'United States',
        continent_code: 'NA'
      }
    */

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


###City

ipv4 address lookup

    // Open the GeoLiteCity.dat file first.
    var City = geoip.City;
    var city = new City('/path/to/GeoLiteCity.dat');

  Synchronous method:

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

  Asynchronous method:

      city.lookup('www.google.com', function(err, data) {
        if (err) {throw err;}
        if (data) {
            console.log(data);
        }
      });


ipv6 address lookup

    // Open the GeoLiteCityv6.dat file first.
    var City6 = geoip.City6;
    var city6 = new City6('/path/to/GeoLiteCityv6.dat');

  Synchronous method:

      var city6_obj = city6.lookupSync('2002:7679:b9af:db9b:ab51:501a:db4e:2d');
      console.log(city6_obj);
      /*
      {
       country_code: 'CN',
       country_code3: 'CHN',
       country_name: 'China',
       region: '32',
       city: 'Chengdu',
       latitude: 30.66670036315918,
       longitude: 104.06670379638672,
       continent_code: 'AS'
      }
      */

  Asynchronous method:

      city6.lookup('2002:c6aa:2dc8:b719:4314:2a7d:e31c:fe73', function(err, data) {
        if (err) {throw err;}
        if (data) {
            console.log(data);
            /* As expected ,return an object that contains city information
            {
             country_code: 'US',
             country_code3: 'USA',
             country_name: 'United States',
             region: 'CO',
             city: 'Englewood',
             postal_code: '80112',
             latitude: 39.569000244140625,
             longitude: -104.85820007324219,
             continent_code: 'NA'
            }
            */
          }
      });

###Organization

    var Org = geoip.Org;
    var org = new Org('/path/to/file')  // Org module can open three edition database 'org', 'asnum', 'isp'

Synchronous method:

    var org_str = org.lookupSync('8.8.8.8');

    console.log(org_str);
    /*
      The type of returned data is string, for example:

      'Genuity'
      'AS15169 Google Inc.'

      no longer an object
    */

Asynchronous method:

    org.lookup('www.google.com', function(err, data) {
        if (err) {throw err;}
        if (data) {
            console.log(data);
        }
    });


###Region

    var Region = geoip.Region;
    var region = new Region('/path/to/GeoIPRegion.dat');

Synchronous method:

    var region_obj = region.lookupSync('8.8.8.8');

    console.log('region_obj);
    /*
      region object has two properties:
      { country_code: 'US', region: 'CO' }

    */

Asynchronous method:

    region.lookup('www.google.com', function(err, data) {
        if (err) {throw err;}
        if (data) {
          console.log(data);
        }
    });


###NetSpeed

    var NetSpeed = geoip.NetSpeed;
    var netspeed = new NetSpeed('/path/to/GeoIPNetSpeed.dat');

Synchronous method:

    var netspeed_str = netspeed.lookupSync('8.8.8.8');

    console.log(netspeed_str);
    /*
      netspeed_str just a simple string, 'Dialup', 'Corprate'... so on
    */

Asynchronous method:

    netspeed.lookup('www.google.com', function(err, data) {
        if (err) {throw err;}
        if (data) {
          console.log(data);  // Maybe return 'unknow' or different from lookup method
        }
    });
