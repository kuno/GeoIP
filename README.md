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

__v0.1.0, 2010-11-25

1, Initial release;

__v0.1.1, 2010-11-27:

1, Removed GeoIP.dat, due to licence(or money) problem.

2, renamed city property to City, renamed country property to Country.

3, Allow close method to set all properties of an exsiting obj to undefined.

__v0.1.2, 2010-12-02

1, Fixed missing of isIP method in node 0.2.x series.

2, Hidden some unsafe an useless funcions.  

__v0.2.0, 2010-12-09:

1, added org, region, netspeed submodule.

2, added data type check in every submodule

3, re-factoring whloe project.

###Data

Befor you can use this package, you need to download or buy some data from [www.maxmind.com](http://www.maxmind.com/app/ip-location).

There are two free versions data among with some commercial versions.

__Note: This package only support binary data, not any other formats.__

GeoIP Country Lite Edition [Download](http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz).

GeoIP City Lite Edition [Download](http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz)

###Install

    npm install geoip

###Usage

####Country Information

* Open the country data file

      var country_data = geoip.open('/path/to/GeoIP.dat');

* Synchronous methods, network independence.

      geoip.Country.code_by_addr(data, '8.8.8.8'); // prints 'US'

      geoip.Country.name_by_addr(data, '8.8.8.8'); // prints  'United States'

* Asynchronous methods, depends on node's async-style dns module.

      geoip.Country.code_by_domain(data, 'www.google.com', function(err, code) {
            if (err) {throw err;}
            console.log(code);  // prints 'US'
      });

      geoip.Country.name_by_domain(data, 'www.google.com', function(err, name) {
            if (err) {throw err;}
            console.log(name);  // prints 'United States'
      });

      //Close the opened file.
      geoip.close(data);

####City Information

* Open the GeoLiteCity.dat file first.

      var city_data = geoip.open('/path/to/GeoLiteCity.dat');

      geoip.City.record_by_addr(data, '8.8.8.8');  // You will get something like this:

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

     geoip.close(city_data);

####Organization Information

* Open the GeoIPOrg.dat first.

      var org_data = geoip.open('/path/to/GeoIPOrg.dat');

      geoip.Org.org_by_addr(data, '8.8.8.8');
      // You wikk get something like below:
      // 'GenuityThe United WayEducation Management CorporationInternational Generating Co. 
      // (Intergen)GoldK.com LLCCisco Systems, 
      // Inc.Verizon/SixContinents HotelsDrug Enforcement AdmFloors IncUS Dept of Treasury - TIGTAHTS Engineering, 
      // LTDITS Caleb BrettAutomated Power Exchange, 
      // Inc.Neuberger Ber'

      geoip.close(org_data);


####Region Information

* Open the GeoIPRegion.dat first.

      var region_data = geoip.open('/path/to/GeoIPRegion.dat');

      geoip.Region.region_by_addr(region_data, '8.8.8.8');  // prints 'US,CO'

      geoip.close(region_data);


####NetSpeed Information

* Open the GeoIPNetSpeed.dat first.

      var netspeed_data = geoip.open('/path/to/GeoIPNetSpeed.dat');

      geoip.NetSpeed.speed_by_addr(netspeed_data, '8.8.8.8');  // prints 'Dailup'

      geoip.close(netspeed_data);
