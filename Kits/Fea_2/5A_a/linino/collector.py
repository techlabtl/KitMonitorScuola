#!/usr/bin/env python

import sys

if len(sys.argv) < 2:
  print "An argument is required"
  sys.exit(1)

fromArduino = sys.argv[1]

print fromArduino

with open("/mnt/sda1/linino/collector/arduinoData", "a") as out_file:
  out_file.write(fromArduino + "\n")
