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
          'deps/geoip-api-c-1.6.0/libGeoIP',
          "<!(node -e \"require('nan')\")"
      ],
      'dependencies': [
        'deps/geoip-api-c-1.6.0/libgeoip.gypi:libgeoip'
      ],
      'conditions': [
        ['OS=="win"', {
          'defines': [
            ]
        }]
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "native" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/native.node" ],
          "destination": "./lib/binding/"
        }
      ]
    }
  ]
}
