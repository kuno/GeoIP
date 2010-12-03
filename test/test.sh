#!/usr/bin/env sh

GEOIP = 'http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz'
GEOLITECITY = 'http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz'


if [ ! -e /tmp/GeoIP.dat ]; then
    wget $GEOIP /tmp/GeoIP.dat.gz || gzip -d /tmp/GeoIP.dat.gz
elif [ ! -e /tmp/GeoLiteCity.dat ]; then
    wget $GEOLITECITY ../data/GeoLiteCity.dat.gz || gzip -d /tmp/GeoLiteCity.dat.gz
fi

node ./country.js
#node ./city.js    || return 1
#node ./org.js     || return 1


