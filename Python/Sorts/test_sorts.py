#!/usr/bin/python

import quick
import numpy.random as nprand

print "Generating random number list (b/w 0-100) of size 1 million"
numberList = nprand.randint(100, size=10000)

print numberList
quick.qs(numberList, 0, len(numberList)-1)
print numberList
