####Region Information

      // Open the GeoIPRegion.dat first.
      var region_data = geoip.open('/path/to/GeoIPRegion.dat');

 - Synchronous method

       geoip.Region.region_by_addr(region_data, '8.8.8.8');  // Return 'US,CO'

 - Asynchronous method

       geoip.Region.region_by_domain(region_data, 'www.google.com', function(err, region) {
          if (err) {throw err;}
          console.log(region);  // Maybe different from region_by_addr
       });

       geoip.close(region_data);
