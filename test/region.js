var geoip = require('../index.js');

var Region = geoip.Region;

geoip.filter('/tmp/GeoIPRegion.dat', function(err, type, data) {

  if (type === 'region') {
    setTimeout(function() {
      console.log('The result of synchronized method');
      console.log('Region.region_by_addr(data, \'8.8.8.8\')');
      console.log('is \'' + Region.region_by_addr(data, '8.8.8.8') + '\'');
      console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
    }, 1000);
  } else {
    console.log('Not region data');
  }
});

var data = geoip.open('/tmp/GeoIPRegion.dat'); 

setTimeout(function() {
  console.log('The result of asynchronous method');
  console.log('Region.region_by_domain(data, \'www.sina.com\', callback())');
  Region.region_by_domain(data, 'www.sina.com', function(err, region) {
    if (err) {throw err;}
    console.log('is ' + region);
  });
}, 2000);

