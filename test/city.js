var geoip = require('../geoip.js');

var data = geoip.open('/tmp/GeoLiteCity.dat');

var City = geoip.City;

setTimeout(function() {
    console.log('The result of ');
    console.log('City.record_by_addr(data, \'8.8.8.8\')');
    console.log('is \'' + JSON.stringify(City.record_by_addr(data, '8.8.8.8')) + '\'');
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);
