##GeoIP API for node, ported from GeoIP php API.##

###Data###

Befor you can use this package, you need to download or buy some data from www.maxmind.com.
There are two free versions data among with some commercial versions.
For convenience, this package includes a free version of GeoIP country data file.

###Install###

    npm install geoip

###Usage###

####Country Information#####

* Open the country data file
    var data = geoip.open();

* Synchronous methods, network independence.
    geoip.country.code_by_addr(data, '8.8.8.8'); // prints 'US'
    geoip.country.name_by_addr(data, '8.8.8.8'); // prints  'USA'

* Asynchronous methods, depends on node's async-style dns module.
    geoip.country.code_by_domain(data, 'www.google.com', function(err, code) {
        if (err) {throw err;}
        console.log(code);  // prints 'US'
    });
    geoip.country.name_by_domain(data, 'www.google.com', function(err, name) {
        if (err) {throw err;}
        console.log(name);  // prints 'USA'
    });

####City Information#####
* Dont forget to download free version GeoLiteCity.dat file first.
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
