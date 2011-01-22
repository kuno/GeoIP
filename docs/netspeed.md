####NetSpeed Information

      // Open the GeoIPNetSpeed.dat first.
      var netspeed_data = geoip.open('/path/to/GeoIPNetSpeed.dat');

 - Synchronous method

       geoip.NetSpeed.speed_by_addr(netspeed_data, '8.8.8.8');  // Return 'Dailup'

 - Asynchronous method
      
       NetSpeed.speed_by_domain(data, 'www.google.com', function(err, speed) {
          if (err) {throw err;}
          console.log(speed);  // Maybe different from speed_by_addr
       });

       geoip.close(netspeed_data);
