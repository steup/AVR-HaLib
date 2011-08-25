#!/bin/env python2.7

from numpy import *
import sys

def eval(filename):
	data=recfromcsv(filename)
	for (a,b,plus,mult) in data:
		print "[%f, %f] +: %f\t*: %f\te(+): %f\te(*): %f"%(a,b,plus, mult, abs(plus-(a+b)), abs(mult-(a*b)))

if __name__=="__main__":
	if len(sys.argv) <=1:
		print "usage: %s <csv datafile>\n" %(sys.argv[0])
	else:
		eval(sys.argv[1])
