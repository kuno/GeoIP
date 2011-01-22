###Country Information

      // Open the country data file
      var country_data = geoip.open('/path/to/GeoIP.dat');

 - Synchronous methods, network independence.

       geoip.Country.code_by_addr(country_data, '8.8.8.8'); // Return 'US'

       geoip.Country.name_by_addr(country_data, '8.8.8.8'); // Return  'United States'

 - Asynchronous methods, depends on node's async-style dns module.

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
