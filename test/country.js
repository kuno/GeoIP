var geoip = require('../geoip.js');


var data = geoip.open('/tmp/GeoIP.dat');

var Country = geoip.Country;

// Test name_by_addr method
console.log('The result of ');
console.log('Country.name_by_addr(data, \'8.8.8.8\')');
console.log('is ');
console.log(Country.name_by_addr(data, '8.8.8.8'));

// Test code_by_addr method
console.log('The result of ');
console.log('Country.code_by_addr(data, \'8.8.8.8\')');
console.log('is '); 
console.log(geoip.Country.code_by_addr(data, '8.8.8.8'));

// Test name_by_domain method
console.log('The result of ');
console.log('Country.name_by_domain(data, \'www.google.com\', callback())');
console.log('is ');
Country.name_by_domain(data, 'www.google.com', function(err, name) {
    if (err) {throw err;}
    console.log(name);
});


// Test code_by_domain method
console.log('The result of ');
console.log('Country.code_by_domain(data, \'www.google.com\', callback())');
console.log('is ');
Country.name_by_domain(data, 'www.google.com', function(err, code) {
    if (err) {throw err;}
    console.log(code);
});   



