##GeoIP API for node.

####Emergent Update####

__Warning: Due to the Asynchronous programming problome,__

__Any version that below the 0.1.3 dosen't work in any real script.__

__I highly recommond that every user should update to at least v0.1.3!__

###Description

Get geolocation information based on domain or IP address.

###Architecture

![architecture](https://github.com/kuno/GeoIP/raw/master/misc/architecture.png)

###Changelog

__v0.1.0, 2010-11-25__

1, Initial release;

__v0.1.1, 2010-11-27:__

1, Removed GeoIP.dat, due to licence(or money) problem.

2, renamed city property to City, renamed country property to Country.

3, Allow close method to set all properties of an exsiting obj to undefined.

__v0.1.2, 2010-12-02:__

1, Fixed missing of isIP method in node 0.2.x series.

2, Hidden some unsafe an useless funcions.  

__v0.2.0, 2010-12-09:__

1, added org, region, netspeed submodule.

2, added data type check in every submodule

3, re-factoring whloe project.

__v0.2.1, 2010-12-10:__

1, added support for GeoIPASNum.dat binary data.

2, make org_by_addr method return an array of org name string.

###Data

Befor you can use this package, you need to download or buy some data from [www.maxmind.com](http://www.maxmind.com/app/ip-location).

There are three free versions data among with some commercial versions.

__Note: This package only support binary data, not any other formats.__

GeoIP Country Lite Edition [Download](http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz).

GeoIP City Lite Edition [Download](http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz)

GeoIP ASN Edition [download](http://geolite.maxmind.com/download/geoip/database/asnum/GeoIPASNum.dat.gz)


###Install

    npm install geoip

###Usage

####Country Information

      // Open the country data file
      var country_data = geoip.open('/path/to/GeoIP.dat');

* Synchronous methods, network independence.

      geoip.Country.code_by_addr(country_data, '8.8.8.8'); // Return 'US'

      geoip.Country.name_by_addr(country_data, '8.8.8.8'); // Return  'United States'

* Asynchronous methods, depends on node's async-style dns module.

      geoip.Country.code_by_domain(country_data, 'www.google.com', function(err, code) {
            if (err) {throw err;}
            console.log(code);  // prints 'US'
      });

      geoip.Country.name_by_domain(country_data, 'www.google.com', function(err, name) {
            if (err) {throw err;}
            console.log(name);  // prints 'United States'
      });

      //Close the opened file.
      geoip.close(country_data);

####City Information

      // Open the GeoLiteCity.dat file first.
      var city_data = geoip.open('/path/to/GeoLiteCity.dat');

      geoip.City.record_by_addr(data, '8.8.8.8');
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

      geoip.close(city_data);

####Organization Information

* Get Organization Information

      // Open the GeoIPOrg.dat first.
      var org_data = geoip.open('/path/to/GeoIPOrg.dat');

      geoip.Org.org_by_addr(org_data, '8.8.8.8');
      // Return an array of the names of organization
      // [
      // 'Genuity',
      // 'The United Way',
      // 'Education Management Corporation,
      // 'International Generating Co. (Intergen)'
      // ]    

      geoip.close(org_data);

* Get ASN informatioin

      // Open the GeoIPASNum.dat first.

      var asn_data = geoip.open('/path/to/GeoIPASNum.dat');

      geoip.Org.asn_by_addr(asn_data, '8.8.8.8');
      // Return an array of asn objects
      //[ 
      //  { number: 'AS15169', description: 'Google Inc.' },
      //  { number: 'AS36597',
      //    description: 'OmniTI Computer Consulting Inc.' },
      //  { number: 'AS26471',
      //    description: 'Smart City Networks' } 
      //]       

      
####Region Information

      // Open the GeoIPRegion.dat first.
      var region_data = geoip.open('/path/to/GeoIPRegion.dat');

      geoip.Region.region_by_addr(region_data, '8.8.8.8');  // Return 'US,CO'

      geoip.close(region_data);


####NetSpeed Information

      // Open the GeoIPNetSpeed.dat first.
      var netspeed_data = geoip.open('/path/to/GeoIPNetSpeed.dat');

      geoip.NetSpeed.speed_by_addr(netspeed_data, '8.8.8.8');  // Return 'Dailup'

      geoip.close(netspeed_data);
