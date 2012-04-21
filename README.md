__GeoIP binding for nodejs__

Get geolocation information based on domain or IP address.

##Important Notification

###Technical Architecture

![new_architecture](https://github.com/toots/GeoIP/raw/master/misc/new_architecture.png)


###Dependency

####libgeoip C library

This module requires the C libgeoip library. You can either use a version installed on your system or 
let the module use the version included with it.

If your os has a system package manager (e.g, on linux, apt of ubuntu, yum of fedora, pacman of arch. MacPorts and homebrew on OSX), 
then, there is a very high possibility that there is alreay a geoip c library package inside the repository.

If not, you can build for yourself:

    wget http://geolite.maxmind.com/download/geoip/api/c/GeoIP-1.4.7.tar.gz
    tar -xvzf GeoIP-1.4.7.tar.gz
    cd GeoIP-1.4.7
    ./configure --prefix=/usr
    make
    sudo make install


##Data

Befor you can use this package, you need to download or buy some data from 
[www.maxmind.com](http://www.maxmind.com/app/ip-location).

There are some free databases among some commercial versions, the free databases can be 
found [here](http://geolite.maxmind.com/download/geoip/database/).


##Install

    [sudo] npm install geoip-static [-g]


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
