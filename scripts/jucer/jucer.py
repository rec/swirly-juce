#!/usr/local/bin/python

# Build a structure representing a .jucer file, and modify it.
#
# Copyright (c) 2010 Tom Swirly
# This software is licensed under the terms of the artistic license,
# http://www.opensource.org/licenses/artistic-license-2.0.php
#
# Essentially, you can do as you please with it.  Have fun!


import dom_file
import filetree
import library


class Jucer(dom_file.DomFile):
  JUCE_ROOT = '../../../../..'
  LIBRARY_ROOT = JUCE_ROOT + '/build/mac'
  CHARS = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
  ID_SIZE = 9

  def __init__(self, filename, is_test, libraries, root):
    dom_file.DomFile.__init__(self, filename)
    self.is_test = is_test
    self.libraries = libraries
    self.root = root


  def modify(self):
    self.setLibraries()
    self.setHeaders()
    self.setMaingroup()


  def toxml(self):
    self.modify()
    return self.dom.toprettyxml()


  def setLibraries(self, config='debug'):
    xcode = self.element('EXPORTFORMATS', 'XCODE_MAC')
    links = library.getLinks(self.libraries, Jucer.LIBRARY_ROOT, config)
    libs = self.join(links)
    xcode.setAttribute('extraLinkerFlags', libs)


  def setHeaders(self):
    configurations = self.element('CONFIGURATIONS')
    for c in configurations.getElementsByTagName('CONFIGURATION'):
      config = 'build/mac/%s' % c.getAttribute('name').lower()
      headers = library.getHeaders(self.libraries, Jucer.JUCE_ROOT, config)
      headerPath = self.join(headers, ';')
      c.setAttribute('headerPath', headerPath)


  def setMaingroup(self):
    old = self.element('MAINGROUP')
    maingroup = self.create('MAINGROUP', name=old.getAttribute('name'))
    self.documentElement.replaceChild(maingroup, old)

    for prefix, name in (('', 'src'),
                         ('genfiles', 'proto')):
      self.createCPPFileGroup(maingroup, prefix, name, '%s/rec' % self.root)
    maingroup.appendChild(self.createFile('Main.cpp', 'Main.cpp'))


  def join(self, files, joiner=' '):
    return joiner.join(filter(self.accept, files))


  def createCPPFileGroup(self, parent, prefix, name, root):
    tree = filetree.filetree('%s/%s/%s' % (root, prefix, name), self.acceptCpp)
    parent.appendChild(self.createFileOrGroup(prefix, name, tree))


  def createFile(self, name, file):
    return self.create('FILE',
                       name=name,
                       resource='0',
                       file=file,
                       compile=str(int(not file.endswith('.h'))))


  def createFileOrGroup(self, prefix, name, tree):
    if type(tree) is str:
      return self.createFile(name, '../../%s/%s' % (prefix, name))

    else:
      group = self.create('GROUP', name=name)
      if prefix:
        prefix = '%s/%s' % (prefix, name)
      else:
        prefix = name

      for k, v in tree.iteritems():
        group.appendChild(self.createFileOrGroup(prefix, k, v))

      return group


  def acceptCpp(self, s):
    return (self.accept(s) and
            ('.' + s).split('.')[-1] in ['h', 'cpp', 'cc', 'c'] and
            not (self.is_test and 'Main.c' in s))


  def accept(self, s):
    return self.is_test or not 'test' in s
