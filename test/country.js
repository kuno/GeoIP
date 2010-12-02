#!/usr/bin/env node

var geoip = require('geoip');


var country_data = geoip.open('/tmp/GeoIP.dat');

var Country = geoip.Country;

Country.code_by_addr(country_data, '8.8.8.8');

Country.name_by_addr(country_data, '8.8.8.8');

Country.code_by_domain(country_data, 'www.google.com', function(err, code) {
    if (err) {throw err;}
    console.log(code);
});

Country.name_by_addr(country_data, 'www.google.com', function(err, name) {
    if (err) {throw err;}
    console.log(name);
});
