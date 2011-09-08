#!/usr/bin/env sh

TEST_DIR=$PWD
BUILD=$TEST_DIR/../build/default/geoip.node

if   [ ! -e ../database/GeoIP.dat ] || [ ! -e ../database/GeoIPv6.dat ] || [ ! -e ../database/GeoLiteCity.dat ] || [ ! -e ../database/GeoLiteCityv6.dat ] || [ ! -e ../database/GeoIPOrg-111.dat ] || [ ! -e ../database/GeoIPASNum.dat ] || [ ! -e ../database/GeoIPRegion-515.dat ] || [ ! -e ../database/GeoIP-171_20040418.dat ]; then
  echo "You need downlaod dababases first"
  echo "See database/database.html"
  return 1
fi

if [ ! -e $BUILD ]; then
  echo "Start to building..."
  cd $TEST_DIR/.. || return 1
  node-waf configure build || return 1
  echo "Building is finished!"
fi

cd $TEST_DIR || return 1

echo "Start to test Country ipv4 module......"
echo "============================================================"
node ./country.js 
echo "============================================================"
echo "Country ipv4 module test is finished!"

echo ""

echo "Start to test Country ipv6 module......"
echo "============================================================"
node ./country6.js 
echo "============================================================"
echo "Country ipv6 module test is finished!"

echo ""                                                             

echo "Start to test City moduel......"
echo "============================================================"
node ./city.js
echo "============================================================"
echo "City moduel test is finished!"

echo ""

echo "Start to test City ipv6 moduel......"
echo "============================================================"
node ./city6.js
echo "============================================================"
echo "City ipv6 moduel test is finished!"

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
