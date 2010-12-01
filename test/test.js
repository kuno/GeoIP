#!/usr/bin/env node

GEOIP = 'http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz'
GEOLITECITY = 'http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz'

var geoip = require('geoip');


var country_data = geoip.open();

var city_data = geoip.open();
