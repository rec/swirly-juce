#!/usr/local/bin/python

# Copyright (c) 2011 Tom Swirly
#
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!

import stat
import os
import os.path

import util.config
import util.root
import util.source

# A Juce command that updates the package

class Updater(object):
  def __init__(self, package):
    self.package = package
    self.config = util.config.config(package)
    self.path = util.root.root(package)

  def __call__(self):
    isUpdate = os.path.isdir(self.path)
    if not isUpdate and os.path.isfile(self.path):
      raise Exception("There's a file on %s" % self.path)
    cmd = util.source.getSourceCommand('update' if isUpdate else 'download',
                                       self.config)
    # res = os.system(cmd)
    print "COMMAND: ", cmd
    print '%s %s' % (('Updated' if isUpdate else 'Downloaded'), self.package)


def update(*packages):
  if not packages:
    raise Exception('No packages to update')

  for p in packages:
    Updater(p)()

  return 'Updated all packages: %s.' % ', '.join(packages)
