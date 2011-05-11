__GeoIP binding for nodejs__

Get geolocation information based on domain or IP address.

__New Architecture

From v0.4.0, geoip will be bind to libgeoip >= 1.4.6, which is a C library.

![new_architecture](https://github.com/kuno/GeoIP/raw/master/misc/new_architecture.png)  


##Data

Befor you can use this package, you need to download or buy some data from [www.maxmind.com](http://www.maxmind.com/app/ip-location).

There are three free versions data among with some commercial versions, the free database can be found [here](http://geolite.maxmind.com/download/geoip/database/).


##Install

    npm install geoip


##Compatibility

From v0.4.0, geoip need nodejs >= 0.4.0, if you want to use it on old nodejs, you can:

    npm install geoip@0.3.4-1


##Usage

###Check edition

    var edition = geoip.check('/path/to/file');

    console.log(edition); // output 'country', 'city', 'org'... so on

###Create a new instance of sub-module, for example:

    var city = new geoip.City('/path/to/GeoLiteCity.dat', false);  // not to cache the database

    var city = new geoip.City('/path/to/GeoLiteCity.dat');  // the default option is cache

##Modules

###Country

    // Open the country data file
    var Country = geoip.Country;
    var country = new Country('/path/to/GeoIP.dat');

__Synchronous methods, network independence

    var country_obj = country.lookup('8.8.8.8');

    console.log(country_obj);
    /*
      { country_code: 'US',
        country_code3: 'USA',
        country_name: 'United States',
        continent_code: 'NA' }
    */

__Asynchronous methods

    country.lookup('www.google.com', function(data) {
        if (data) { // no err, if not found, just return null
            console.log(data);  // same as lookup method
        }
    });


    //Close the opened file.
    country.close();


###City

    // Open the GeoLiteCity.dat file first.
    var City = geoip.City;
    var city = new City('/path/to/GeoLiteCity.dat');

__Synchronous method

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

__Asynchronous method

    city.lookup('www.google.com', function(data) {
        if (data) {
            console.log(data);
        }
    });

    city.close();


###Organization

    var Org = geoip.Org;
    var org = new Org('/path/to/file')  // Org module can open three edition database 'org', 'asnum', 'isp'

__Synchronous method

    var org_str = org.lookup('8.8.8.8');

    console.log(org_str);
    /*
      The type of returned data is string, for example:

      'Genuity'
      'AS15169 Google Inc.'
      
      no longer an object
    */

    org.close();

__Asynchronous method

    org.lookup('www.google.com', function(data) {
        if (data) {
            console.log(data);
        }
    });


###Region

    var Region = geoip.Region;
    var region = new Region('/path/to/GeoIPRegion.dat');

__Synchronous method

    var region_obj = region.lookupSync('8.8.8.8'); 
    
    console.log('region_obj);
    /*
      region object has two properties:
      { country_code: 'US', region: 'CO' }

    */

__Asynchronous method

    region.lookup('www.google.com', function(data) {
        if (data) {
          console.log(data);
        }
    });

    region.close();


###NetSpeed

    var NetSpeed = geoip.NetSpeed;
    var netspeed = new NetSpeed('/path/to/GeoIPNetSpeed.dat');

__Synchronous method

    var netspeed_str = netspeed.lookupSync('8.8.8.8');
    
    console.log(netspeed_str);
    /*
      netspeed_str just a simple string, 'Dialup', 'Corprate'... so on
    */

__Asynchronous method

    netspeed.lookup('www.google.com', function(data) {
        if (data) {
          console.log(data);  // Maybe return 'unknow' or different from lookup method
        }
    });

    netspeed.close();
