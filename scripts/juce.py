#!/usr/local/bin/python

import os.path
import sys

# Main program for the juce build system routes control to other scripts.
#
# Copyright (c) 2011 Tom Swirly
#
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!


if __name__ == '__main__':
  # print os.path.abspath(sys.argv[0])
  args = sys.argv[1:]
  command = args.pop(0)

  try:
    exec 'from commands import %s' % command
  except ImportError:
    print "Couldn't find command/%s.py" % command
    raise

  f = getattr(locals()[command], command)
  print f(*args)
