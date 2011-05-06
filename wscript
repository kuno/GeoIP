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
  obj.find_sources_in_dirs("src")
  obj.lib = ["GeoIP"]

def shutdown(bld):
  # HACK to get binding.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists('geoip.node'): unlink('geoip.node')
  else:
    if exists('build/default/geoip.node') and not exists('geoip.node'):
      symlink(getcwd()+'/build/default/geoip.node', 'geoip.node')
