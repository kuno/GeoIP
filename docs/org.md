###Organization Information

####Get Organization Information

      // Open the GeoIPOrg.dat first.
      var org_data = geoip.open('/path/to/GeoIPOrg.dat');

- Synchronous method

      geoip.Org.org_by_addr(org_data, '8.8.8.8');
      // Return an array of the names of organization
      // [
      // 'Genuity',
      // 'The United Way',
      // 'Education Management Corporation,
      // 'International Generating Co. (Intergen)'
      // ]    

      geoip.close(org_data);

 - Asynchronous method

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

 - Synchronous method

       geoip.Org.asn_by_addr(asn_data, '8.8.8.8');
       // Return an array of asn objects
       //[ 
       //  { number: 'AS15169', description: 'Google Inc.' },
       //  { number: 'AS36597', description: 'OmniTI Computer Consulting Inc.' },
       //  { number: 'AS26471', description: 'Smart City Networks' } 
       //]

 - Asynchronous method
      
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
