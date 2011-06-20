#!/usr/local/bin/python

import os.path
import sys

# Main program for the juce build system routes control to other scripts in the
# directory "commands".
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
  cmd = args.pop(0)

  try:
    exec 'from commands import %s' % cmd

    module = locals()[cmd]
    function = getattr(module, cmd)
    print function(*args)

  except ImportError:
    print "ERROR: Couldn't find file commands/%s.py" % cmd

  except AttributeError:
    print "ERROR: File commands/%s.py needs a function named %s()" % (cmd, cmd)

  except Exception as e:
    print 'ERROR: %s' % e
