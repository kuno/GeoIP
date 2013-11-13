#!/usr/bin/perl

# Used to generate regionName.c
# usage: ./generate_regionName.pl > ../libGeoIP/regionName.c

# Usually run update-region-codes.pl before generate_regionName.pl like
#
# ./update-region-codes.pl
# ./generate_regionName.pl > ../libGeoIP/regionName.c


use strict;
use warnings;

#print qq(#include "regionname.h"\n);
print <<__C_CODE__;
#include <string.h>
#include <stdio.h>

const char * GeoIP_region_name_by_code(const char * country_code,const char * region_code) {
  const char * name = NULL;
  int region_code2 = -1;
  if (region_code == NULL) { return NULL; }

  if (   ((region_code[0] >= 48) && (region_code[0] < (48 + 10)))
      && ((region_code[1] >= 48) && (region_code[1] < (48 + 10)))
  ) {

    /* only numbers, that shorten the large switch statements */
    region_code2 = (region_code[0] - 48) * 10 + region_code[1] - 48;
  }

  else if (    (    ((region_code[0] >= 65) && (region_code[0] < (65 + 26)))
                 || ((region_code[0] >= 48) && (region_code[0] < (48 + 10))))
            && (    ((region_code[1] >= 65) && (region_code[1] < (65 + 26)))
                 || ((region_code[1] >= 48) && (region_code[1] < (48 + 10))))
  ) {

    region_code2 = (region_code[0] - 48) * (65 + 26 - 48) + region_code[1] - 48 + 100;
  }

  if (region_code2 == -1) {return NULL;}

__C_CODE__

# iso3166_2.txt extracted from http://www.maxmind.com/app/iso3166_2
open( FILE, "iso3166_2.txt" ) or die $!;
<FILE>;
my $last_country_code = "";
while ( my $str = <FILE> ) {
  chomp($str);
  my ( $country_code, $region_code, $name ) = split( /,/, $str, 3 );
  $region_code =~ /^[A-Z]{2}$/ or die "Wrong region code";
  my $region_code2 =
    ( ( ord( substr( $region_code, 0, 1 ) ) - 48 ) * ( 65 + 26 - 48 ) ) +
    ord( substr( $region_code, 1, 1 ) ) - 48 + 100;
  readcode( $last_country_code, $country_code, $region_code, $region_code2,
            $name );
  $last_country_code = $country_code;
}
close(FILE);
#print "    }\n";
#print "  }\n";

# fips10_4.txt extracted from http://www.maxmind.com/app/fips10_4
open( FILE, "fips10_4.txt" ) or die $!;
<FILE>;
while ( my $str = <FILE> ) {
  chomp($str);
  my ( $country_code, $region_code, $name ) = split( /,/, $str, 3 );
  next if ( $country_code eq "US" );
  next if ( $country_code eq "CA" );
  my $region_code2;
  if ( $region_code =~ /^\d\d$/ ) {
    $region_code2 =
      ( ord( substr( $region_code, 0, 1 ) ) - 48 ) * 10 +
      ord( substr( $region_code, 1, 1 ) ) - 48;

  }
  elsif ( $region_code =~ /^[A-Z0-9]{2}$/ ) {
    $region_code2 =
      ( ( ord( substr( $region_code, 0, 1 ) ) - 48 ) * ( 65 + 26 - 48 ) ) +
      ord( substr( $region_code, 1, 1 ) ) - 48 + 100;
  }
  else {
    die "Region code seems wrong $region_code\n";
  }
  readcode( $last_country_code, $country_code, $region_code, $region_code2,
            $name );
  $last_country_code = $country_code;
}
print "    }\n";
print "  }\n";
print "  return name;\n";
print "}\n";

close(FILE);

sub readcode {
  my ( $last_country_code, $country_code, $region_code, $region_code2, $name ) =
    @_;
  if ( $country_code ne $last_country_code ) {
    if ( $last_country_code ne "" ) {
      print "    }\n";
      print "  }\n";
    }

    my $prefix = $last_country_code ? "  else " : "  ";
    print "${prefix}if (strcmp(country_code," . qq(") . $country_code . qq(") . ") == 0) {\n";
    print "    switch (region_code2) {\n";
  }

  #  $name =~ s!\s+!!g;
  $name =~ s!\"!!g;
  $name = qq(") . $name . qq(");
  print "      case " . $region_code2 . ":\n";
  print "      return " . $name . ";\n";
}
