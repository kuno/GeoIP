#!/usr/bin/env sh

CITY_DB="http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz"
CITY6_DB="http://geolite.maxmind.com/download/geoip/database/GeoLiteCityv6-beta/GeoLiteCityv6.dat.gz"
COUNTRY_DB="http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz"
COUNTRY6_DB="http://geolite.maxmind.com/download/geoip/database/GeoIPv6.dat.gz"
ORG_DB="http://www.maxmind.com/sample/GeoIP-111_20030603.tar.gz"
REGION_DB="http://www.maxmind.com/sample/GeoIPRegion-515_20050401.tar.gz"
ASUM_DB="http://geolite.maxmind.com/download/geoip/database/asnum/GeoIPASNum.dat.gz"
NETSPEED_DB="http://www.maxmind.com/sample/GeoIP-171_20040418.tar.gz"
SPEEDCELL_DB="https://www.dropbox.com/s/7hv3rqzbw30th8y/GeoIP-177it_20120221.tar.gz"

if [ ! -z $FETCH_CMD ]; then
    FETCH_CMD=$FETCH_CMD
else
    FETCH_CMD=wget
fi

mkdir -p ./database || return 1
cd ./database || return 1

# Country
if [ ! -e "GeoIP.dat" ]; then
    $FETCH_CMD $COUNTRY_DB || return 1
    gunzip GeoIP.dat.gz
fi

# Country6
if [ ! -e "GeoIPv6.dat" ]; then
    $FETCH_CMD $COUNTRY6_DB || return 1
    gunzip GeoIPv6.dat.gz
fi

# City
if [ ! -e "GeoLiteCity.dat" ]; then
  $FETCH_CMD $CITY_DB || return 1
  gunzip GeoLiteCity.dat.gz
fi

# City6
if [ ! -e "GeoLiteCityv6.dat" ]; then
  $FETCH_CMD $CITY6_DB || return 1
  gunzip GeoLiteCityv6.dat.gz
fi

# Org
if [ ! -e "GeoIPOrg-111.dat" ]; then
    $FETCH_CMD $ORG_DB || return 1
    tar -zxvf GeoIP-111_20030603.tar.gz
    mv GeoIP-111_20030603/*.dat .
fi

if [ ! -e "GeoIPASNum.dat" ]; then
    $FETCH_CMD $ASUM_DB || return 1
    gunzip GeoIPASNum.dat.gz
fi

# Region
if [ ! -e "GeoIPRegion-515.dat" ]; then
    $FETCH_CMD $REGION_DB || return 1
    tar -zxvf GeoIPRegion-515_20050401.tar.gz
    mv GeoIPRegion-515_20050401/*.dat .
fi

# Netspeed
if [ ! -e "GeoIP-171_20040418.dat" ]; then
    $FETCH_CMD $NETSPEED_DB || return 1
    tar -zxvf GeoIP-171_20040418.tar.gz
    mv GeoIP-171_20040418/*.dat .
fi

# Speedcell
if [ ! -e "GeoIP-177it_20120221.tar.gz" ]; then
    $FETCH_CMD $SPEEDCELL_DB || return 1
    tar -zxvf GeoIP-177it_20120221.tar.gz
    mv GeoIP-177it_20120221/*.dat .
fi

exit 0
