import Options
from os import popen, unlink, symlink, getcwd
from os import name as platform
from os.path import exists

srcdir = "."
blddir = "build"
VERSION = "0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "geoip"
  obj.source = ['src/init.cc', 
                'src/netspeed.cc', 
                'src/country6.cc', 
                'src/country.cc',
                'src/region.cc',
                'src/city6.cc',
                'src/city.cc', 
                'src/org.cc', 
                'src/utils.cc', 
                'src/global.cc']

  obj.lib = ["GeoIP"]

def link(bld):
  # HACK to get binding.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists(getcwd() + '/geoip.node'): unlink('geoip.node')
  else:
    if exists(getcwd() + '/build/default/geoip.node') and not exists(getcwd() + 'geoip.node'):
      symlink(getcwd()+'/build/default/geoip.node', getcwd() + 'geoip.node')
