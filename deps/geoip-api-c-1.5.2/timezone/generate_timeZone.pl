#!/usr/bin/perl
use strict;

# Obtain timezone.txt from http://www.maxmind.com/timezone.txt

# Used to generate timeZone.c
# usage: ./generate_timeZone.pl > ../libGeoIP/timeZone.c

my $tz;

open my $fh, '<', 'timezone.txt' or die $!;

print "#include <string.h> \n";
print
"const char* GeoIP_time_zone_by_country_and_region(const char * country,const char * region) {\n";
print "  const char* timezone = NULL;\n";
print "  if (country == NULL) {\n";
print "    return NULL;\n";
print "  }\n";
print "  if (region == NULL) {\n";
print '    region = "";', "\n";
print "  }\n";

local $_ = <$fh>;    # skip first line
while (<$fh>) {
  chomp;
  my ( $country, $region, $timezone ) = split /\t/;
  die "$_ $.\n" unless $timezone;
  $tz->{$country}->{ $region || '' } = $timezone;
}

my $first_country;
for my $c ( sort keys %$tz ) {
  print( ( defined $first_country ) ? '  else if' : '  if' );
  $first_country ||= $c;
  my $def = delete $tz->{$c}->{''};
  if ( my @reg = sort keys %{ $tz->{$c} } ) {
    my @tz = map{ $tz->{$c}->{$_} } @reg;

    printf( qq! ( strcmp (country, "%s") == 0 ) {\n!, $c );
    for ( 0 .. $#reg ) {

      # have regions

      print( ( $_ == 0 ) ? '    if' : '    else if' );
      printf( qq! ( strcmp (region, "%s") == 0 ) {\n!, $reg[$_] );
      printf( qq!      timezone = "%s";\n!,            $tz[$_] );
      printf( qq!    }\n! );
    }
    printf( qq!  else { timezone = "%s"; }\n!, $def ) if defined $def;
    print qq[  }\n];
  }
  else {

    # only default tz
    printf( qq! ( strcmp (country, "%s") == 0 ) {\n!, $c );
    printf( qq!    timezone = "%s";\n!,               $def );
    printf(qq!  }\n!);

  }
}

print qq[  return timezone;\n}\n];
