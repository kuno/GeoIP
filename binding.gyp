{
  'targets': [
    {
      'target_name': 'geoip',
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
        , 'src/utils.cc'
      ],
      'link_settings': {
      'libraries': [
       '-lGeoIP'
        ],
      },
      'include_dirs': [
          '<(node_root_dir)/src',
          '<(node_root_dir)/deps/geoip/libGeoIP'
      ],
      'conditions': [
           # TODO
      ]
    },
  ]
}
