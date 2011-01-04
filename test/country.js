var geoip = require('../geoip.js');

var data = geoip.open('/media/d5fc189b-3c6b-4947-bda3-b7d0890fe6ca/Archive/GeoIP/GeoIP.dat');

var Country = geoip.Country;

// Test name_by_addr method
setTimeout(function() {
    console.log('The result of synchronized method');
    console.log('Country.name_by_addr(data, \'8.8.8.8\')');
    console.log('is \'' + Country.name_by_addr(data, '8.8.8.8') + '\'');
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 1000);

// Test code_by_addr method
setTimeout(function() {
    console.log('The result of synchronized method');
    console.log('Country.code_by_addr(data, \'8.8.8.8\')');
    console.log('is \'' + Country.code_by_addr(data, '8.8.8.8') + '\'');
    console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
}, 2000);

// Test name_by_domain method
setTimeout(function() {
    console.log('The result of asynchronous method');
    console.log('Country.name_by_domain(data, \'www.google.com\', callback())');
    Country.name_by_domain(data, 'www.google.com', function(err, name) {
        if (err) {throw err;}
        console.log('is \'' + name + '\'');
        console.log('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>');
    });
}, 3000);

// Test code_by_domain method
setTimeout(function() {
    console.log('The result of asynchronous method');
    console.log('Country.code_by_domain(data, \'www.google.com\', callback())');
    Country.code_by_domain(data, 'www.google.com', function(err, code) {
        if (err) {throw err;}
        console.log('is \'' + code + '\'');
    });
}, 4000);
