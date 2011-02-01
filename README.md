GeoIP API for node.
===================


##Description

Get geolocation information based on domain or IP address.

__Live Demo__

[http://64.30.136.194:8124/](http://64.30.136.194:8124)


##Compatibility

v0.3.2, Passed test on nodejs v0.2.0 ~ 0.2.6, v0.3.0 ~ 0.3.6.

__Note: next release(0.3.3)  will drop the support for 0.2.0.__


##Architecture

![architecture](https://github.com/kuno/GeoIP/raw/master/misc/architecture.png)


##Data

Befor you can use this package, you need to download or buy some data from [www.maxmind.com](http://www.maxmind.com/app/ip-location).

There are three free versions data among with some commercial versions.

__Note: This package only support binary data, not any other formats.__

GeoIP ASN Edition [Download](http://geolite.maxmind.com/download/geoip/database/asnum/GeoIPASNum.dat.gz)  

GeoIP City Lite Edition [Download](http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz)  

GeoIP Country Lite Edition [Download](http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz).



##Install

npm install geoip


##Usage

__Open the binary data file__

    var data = geoip.open('/path/to/file');

__Check the type of opened data__

    var type = geoip.check(data);
    // Return one of these: 'country', 'city', 'org', 'netspeed', 'region';
    // Or return null, if not a valid data

__Close the opened data__

    geoip.close(data);

##Examples

###Country Information

    // Open the country data file
    var country_data = geoip.open('/path/to/GeoIP.dat');

__Synchronous methods, network independence__

     geoip.Country.code_by_addr(country_data, '8.8.8.8'); // Return 'US'

     geoip.Country.name_by_addr(country_data, '8.8.8.8'); // Return  'United States'

__Asynchronous methods, depends on node's async-style dns module.__

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



###City Information

    // Open the GeoLiteCity.dat file first.
    var city_data = geoip.open('/path/to/GeoLiteCity.dat');

__Synchronous method__

    geoip.City.record_by_addr(city_data, '8.8.8.8');
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

__Asynchronous method__

    geoip.City.record_by_domain(city_data, 'www.google.com', function(err, reord) {
    if (err) {throw err;}
    var keys = Object.keys(record);
    keys.forEach(function(k) {  // Same as record_by_addr
      console.log(k + ':' + record[k]);
      });   
    });

    geoip.close(city_data);


###Organization Information

####Get Organization Information

    // Open the GeoIPOrg.dat first.
    var org_data = geoip.open('/path/to/GeoIPOrg.dat');

__Synchronous method__

   geoip.Org.org_by_addr(org_data, '8.8.8.8');
   // Return an array of the names of organization
   // [
   // 'Genuity',
   // 'The United Way',
   // 'Education Management Corporation,
   // 'International Generating Co. (Intergen)'
   // ]    

   geoip.close(org_data);

__Asynchronous method__

    // This method has a small bug that not resovled yet, not recommend use it.
    geoip.Org.org_by_domain(org_data, 'www.google.com', function(err, org) {
    if (err) {throw err;}
    if (typeof org === 'string') {
    console.log(org);  // Organization Not Found
    } else {  // Same as org_by_addr
    org.foreach(function(o) {
      console.log(o[0] + ':' + o[1]);
      });
    }
    });


####Get [ASN](http://www.apnic.net/services/services-apnic-provides/helpdesk/faqs/asn-faqs) informatioin

    // Open the GeoIPASNum.dat first.
    var asn_data = geoip.open('/path/to/GeoIPASNum.dat');

__Synchronous method__

    geoip.Org.asn_by_addr(asn_data, '8.8.8.8');
    // Return an array of asn objects
    //[ 
    //  { number: 'AS15169', description: 'Google Inc.' },
    //  { number: 'AS36597', description: 'OmniTI Computer Consulting Inc.' },
    //  { number: 'AS26471', description: 'Smart City Networks' } 
    //]

__Asynchronous method__

    geoip.Org.asn_by_domain(asn_data, 'www.google.com', function(err, asn) {
    if (err) {throw err;}
    if (typeof ans === 'string') {
    console.log(asn)  // ASNumber Not Found
    } else {  // Same as asn_by_addr
    asn.forEach(function(a) {
      var keys = object.keys(a);
      console.log(a[keys[0]] + ' : ' + a[keys[1]]);
      });
    }
    });

    geoip.close(asn_data);


####Region Information

    // Open the GeoIPRegion.dat first.
    var region_data = geoip.open('/path/to/GeoIPRegion.dat');

__Synchronous method__

    geoip.Region.region_by_addr(region_data, '8.8.8.8');  // Return 'US,CO'

__Asynchronous method__

    geoip.Region.region_by_domain(region_data, 'www.google.com', function(err, region) {
    if (err) {throw err;}
    console.log(region);  // Maybe different from region_by_addr
    });

    geoip.close(region_data);


####NetSpeed Information

    // Open the GeoIPNetSpeed.dat first.
    var netspeed_data = geoip.open('/path/to/GeoIPNetSpeed.dat');

__Synchronous method__

    geoip.NetSpeed.speed_by_addr(netspeed_data, '8.8.8.8');  // Return 'Dailup'

__Asynchronous method__

    NetSpeed.speed_by_domain(data, 'www.google.com', function(err, speed) {
    if (err) {throw err;}
    console.log(speed);  // Maybe different from speed_by_addr
    });

    geoip.close(netspeed_data);
