dns = require 'dns'
ipToLong = (require './utils.js').ipToLong
seekCountry = (require './utils.js').seekCountry
CONST = Object.freeze require '../include/constants.js'

__idByAddr__ = (data, addr) ->
  unless addr == null
    return seekCountry data, ipToLong addr
  return false

speed_by_addr = (data, addr) ->
  speed = null

  id = __idByAddr__ data, addr

  if id == CONST.UNKNOW_SPEED
    speed = 'Unknow'
  else if id == CONST.DIALUP_SPEED
    speed = 'Dailup'
  else if id == CONST.CABLEDSL_SPEED
    speed = 'Cable'
  else if id == CONST.CORPORATE_SPEED
    speed = 'Corporate'

  return speed

speed_by_domain = (data, domain, callback) ->
  speed = null
  unless data.db_type == CONST.NETSPEED_EDITION
    err = new Error 'Not NetSpeed Data'
    callback err, speed
  else
    addresses = dns.resovle4 domain
    if err
      throw err
    else
      id = __idByAddr__ data, addresses[0]
      if id == CONST.UNKNOW_SPEED
        speed = 'Unknow'
      else if id == CONST.DIALUP_SPEED
        speed = 'Dailup'
      else if id == CONST.CABLEDSL_SPEED
        speed = 'Cable'
      else if id == CONST.CORPORATE_SPEED
        speed = 'Corporate'

  return speed

exports.speed_by_addr = speed_by_addr

