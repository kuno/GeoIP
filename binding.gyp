{
  'targets': [
    {
      'target_name': 'native',
      'sources': [
        'src/global.cc'
        , 'src/init.cc'
        , 'src/country.cc'
        , 'src/country6.cc'
        , 'src/city.cc'
        , 'src/city6.cc'
        , 'src/org.cc'
        , 'src/region.cc'
        , 'src/netspeed.cc'
        , 'src/netspeedcell.cc'
        , 'src/utils.cc'
      ],
      'link_settings': {
      'libraries': [
        ],
      },
      'include_dirs': [
          'src',
          'deps/GeoIP-1.6.2/libGeoIP',
          "<!(node -e \"require('nan')\")"
      ],
      'dependencies': [
        'deps/GeoIP-1.6.2/libgeoip.gypi:libgeoip'
      ],
      'conditions': [
        ['OS=="win"', {
          'defines': [
            ]
        }]
      ]
    },
  ]
}
