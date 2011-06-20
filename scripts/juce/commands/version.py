#!/usr/local/bin/python

# Copyright (c) 2011 Tom Swirly
#
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!


VERSION = '0.1'

# A Juce command that just prints the version number!

def version(*args):
  return 'juce version %s' % VERSION
