#!/usr/bin/python

import os, sys

total = len(sys.argv)
if total == 4:
  # Open a file
  numberOfRandomVector = str(sys.argv[1])
  numberOfIteration = str(sys.argv[2])
  numberOfCluster = str(sys.argv[3])
  path = "./test"
  dirs = os.listdir( path )
  # This would print all the files and directories
  for file in dirs:
    print file
    os.system('./run ./test/'+file+" "+numberOfRandomVector+" "+numberOfIteration+" "+numberOfCluster)
else:
  print 'argv[0]: name of python script.'
  print 'argv[1]: number of random vector.'
  print 'argv[2]: number of iteration.'
  print 'argv[3]: Number of cluster. '
