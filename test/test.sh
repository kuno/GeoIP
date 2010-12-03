#!/usr/bin/env sh

GEOIP='http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz'
GEOLITECITY='http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz'


if [ ! -e /tmp/GeoIP.dat ] || [ ! -e /tmp/GeoLiteCity.dat ]; then
    curl $GEOIP | gzip -d -c > /tmp/GeoIP.dat
    curl $GEOLITECITY | gzip -d -c > /tmp/GeoLiteCity.dat
fi

echo "Start to test Country module......"
echo "============================================================"
node ./country.js 
echo "============================================================"
echo "Country module test is finished!"

echo ""

echo "Start to test City moduel......"
echo "============================================================"
node ./city.js
echo "============================================================"
echo "City moduel test is finished!"


