#!/usr/bin/env node

var path = require('path');
var geoip = require(path.resolve(__dirname, '../index'));

// City
var city_db = path.resolve(__dirname, '../database/GeoLiteCity.dat');

var city = new geoip.City(city_db);

console.log(city.lookupSync('8.8.8.8'));

city.lookup('8.8.8.8', function(err, data) {
    console.log(data);
});

// City6
var city6_db = path.resolve(__dirname, '../database/GeoLiteCityv6.dat');

var city6 = new geoip.City6(city6_db);

console.log(city6.lookupSync('2406:a000:f0ff:fffe::122d'));

city6.lookup('2406:a000:f0ff:fffe::122d', function(err, data) {
    console.log(data);
});

// Country
var country_db = path.resolve(__dirname, '../database/GeoIP.dat');

var country = new geoip.Country(country_db);

console.log(country.lookupSync('8.8.8.8'));

country.lookup('8.8.8.8', function(err, data) {
    console.log(data);
});

// Country6
var country6_db = path.resolve(__dirname, '../database/GeoIPv6.dat');

var country6 = new geoip.Country6(country6_db);

console.log(country6.lookupSync('2406:a000:f0ff:fffe::122d'));

country6.lookup('2406:a000:f0ff:fffe::122d', function(err, data) {
    console.log(data);
});

// Netspeed
var netspeed_db = path.resolve(__dirname, '../database/GeoIP-171_20040418.dat');

var netspeed = new geoip.NetSpeed(netspeed_db);

console.log(netspeed.lookupSync('8.8.8.8'));

netspeed.lookup('8.8.8.8.', function(err, data) {
    console.log(data);
});

// org
var org_db = path.resolve(__dirname, '../database/GeoIPASNum.dat');

var org = new geoip.Org(org_db);

console.log(org.lookupSync('8.8.8.8'));

org.lookup('8.8.8.8', function(err, data) {
    console.log(data);
});

// region
var region_db = path.resolve(__dirname, '../database/GeoIPRegion-515.dat');

var region = new geoip.Region(region_db);

console.log(region.lookupSync('8.8.8.8'));

region.lookup('8.8.8.8', function(err, data) {
    console.log(data);
});





