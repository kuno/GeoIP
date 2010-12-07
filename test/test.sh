#!/usr/bin/env sh

GEOIP='http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz'
GEOLITECITY='http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz'
REGION='http://www.maxmind.com/sample/GeoIPRegion-515_20050401.tar.gz'
NETSPEED='http://www.maxmind.com/sample/GeoIP-171_20040418.tar.gz'
ORG='http://www.maxmind.com/sample/GeoIP-111_20030603.tar.gz'

if [ ! -e /tmp/GeoIP.dat ] || [ ! -e /tmp/GeoLiteCity.dat ] || 
  [ ! -e /tmp/GeoIP-111_20030603/GeoIPOrg-111.dat ] ||
    [ ! -e /tmp/GeoIP-171_20040418/GeoIP-171_20040418.dat ] ||
    [ ! -e /tmp/GeoIPRegion-515_20050401/GeoIPRegion-515.dat ]; then
  curl $GEOIP | gzip -d -c > /tmp/GeoIP.dat
  curl $GEOLITECITY | gzip -d -c > /tmp/GeoLiteCity.dat
  curl $REGION | tar zxvf -C /tmp/
  curl $NETSPEED | tar zxvf -C /tmp/
  curl $ORG | tar xvf -C /tmp/
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

echo ""

echo "Start to test Org module......"
echo "============================================================"
node ./org.js
echo "============================================================"
echo "Org module test is finished!"

echo ""

echo "Start to test Region module......"
echo "============================================================"
node ./region.js
echo "============================================================"
echo "Region module test is finished!"

echo ""

echo "Start to test NetSpeed module......"
echo "============================================================"
node ./netspeed.js
echo "============================================================"
echo "NetSpeed module test is finished!"
