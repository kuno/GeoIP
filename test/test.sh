#!/usr/bin/env sh

GEOIP = 'http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz'
GEOLITECITY = 'http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz'


if [ ! -e /tmp/GeoIP.dat ]; then
    wget $GEOIP /tmp/GeoIP.dat.gz || gzip -d GeoIP.dat.gz
elif [ ! -e /tmp/GeoLiteCity.dat ]; then
    wget $GEOLITECITY /tmp/GeoLiteCity.dat.gz || gzip -d GeoLiteCity.dat.gz
fi

node ./country.js || return 1
#node ./city.js    || return 1
#node ./org.js     || return 1


