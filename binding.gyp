{
  'targets': [
    {
      'target_name': 'geoip',
      'type': 'shared_library',
      'sources': [
          'src/init.cc'
        , 'src/country.cc','src/country6.cc'
        , 'src/city.cc','src/city6.cc'
        , 'src/org.cc','src/region.cc'
        , 'src/netspeed.cc', 'src/utils.cc'
      ],
      'include_dirs': [
          'deps/geoip/libGeoIP'
      ],
      'dependencies': [
      ],
      'conditions': [
        ['OS=="win"', {
          'libraries': [
          ],
          'dependencies': [
          ]
        }, {
          'libraries': [
          ],
        }],
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '10.5',
            'OTHER_CFLAGS': [
                '-ObjC++'
            ]
          },
          'libraries': [
              '-lobjc'
          ],
        }]
      ]
    },
  ]
}
