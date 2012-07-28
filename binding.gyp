{
  'targets': [
    {
      'target_name': 'geoip',
      'sources': [
        'deps/geoip/libGeoIP/GeoIP.h',
        'deps/geoip/libGeoIP/GeoIPCity.h',
        'src/global.h', 'src/global.cc'
        ,'src/node_geoip.h', 'src/init.cc'
        , 'src/country.h', 'src/country.cc'
        , 'src/country6.h' ,'src/country6.cc'
        , 'src/city.h', 'src/city.cc'
        , 'src/city6.h', 'src/city6.cc'
        , 'src/org.h', 'src/org.cc'
        , 'src/region.h', 'src/region.cc'
        , 'src/netspeed.h', 'src/netspeed.cc'
        , 'src/utils.h', 'src/utils.cc'
      ],
      'include_dirs': [
          'deps/geoip/libGeoIP'
      ],
      'conditions': [
        ['OS=="win"', {
          # no Windows support yet...
        }
      ],
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '10.5',
            'OTHER_CFLAGS': [
              '<!@(xml2-config --cflags)'
            ]
          },
        }]
      ]
    },
  ]
}
