###City Information

      // Open the GeoLiteCity.dat file first.
      var city_data = geoip.open('/path/to/GeoLiteCity.dat');

 - Synchronous method
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

 - Asynchronous method

       geoip.City.record_by_domain(city_data, 'www.google.com', function(err, reord) {
            if (err) {throw err;}
            var keys = Object.keys(record);
            keys.forEach(function(k) {  // Same as record_by_addr
                console.log(k + ':' + record[k]);
            });   
       });

       geoip.close(city_data);
