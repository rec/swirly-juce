#!/usr/local/bin/python

# Copyright (c) 2011 Tom Swirly
#
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!

"""
 Get configuration for a package from a json file.
  1. If there is no filename, it is juce.
  2. If there is no .json suffix, it is added
  3. If it is a url with a scheme like http: or https: or ftp: it is opened
  4. If it is a local URL and the config file exists, we take that file
  5. If it is a local URL and the config file does not exist, we take the file
     from the corresponding location in Juce.
  6. If the result is a "redirect" then we do it all again!"""

import json
import os
import urlparse
import urllib2

import util.root

CONFIG_PACKAGE = 'swirly-juce/config/'
CONFIG_ROOT = 'https://github.com/rec/swirly-juce/config/'

def config(package):
  def complete(package):
    if package.endswith('/'):
      package += 'juce'
    if not package.endswith('.json'):
      package += '.json'
    return package

  def configOnce(package):
    package = complete(package)
    if not urlparse.urlparse(package).scheme:
      p = util.root.root(package)
      if os.path.isfile(p):
        return open(p)

      p = util.root.root(CONFIG_PACKAGE + package)
      if os.path.isfile(p):
        return open(p)

    p = CONFIG_ROOT + package
    try:
      return urllib2.urlopen(p)
    except:
      raise Exception("Can't open URL %s" % p)


  result = None
  while package:
    result = json.load(configOnce(package))
    package = result.get('redirect', None)

  return result
