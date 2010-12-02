#!/usr/bin/env sh

GEOIP = 'http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz'
GEOLITECITY = 'http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz'


if [ ! -e ../data/GeoIP.dat ]; then
    wget $GEOIP ../data/GeoIP.dat.gz || gzip -d ../data/GeoIP.dat.gz
elif [ ! -e ../data/GeoLiteCity.dat ]; then
    wget $GEOLITECITY ../data/GeoLiteCity.dat.gz || gzip -d ../data/GeoLiteCity.dat.gz
fi

node ./country.js
#node ./city.js    || return 1
#node ./org.js     || return 1


