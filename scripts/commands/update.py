#!/usr/local/bin/python

# Copyright (c) 2011 Tom Swirly
#
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!


# A Juce command that just prints the version number!

def update(*packages):
  if not packages:
    raise Exception('No packages to update')
  return 'Updated packages %s.' % ', '.join(packages)
