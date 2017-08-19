#!/usr/bin/python
import os, sys, time


def partition(numberList, si, ei):
    pivot = numberList[ei]
    index = si
    #print "si",si, "ei",ei, "pivot", pivot, "index", index
    for i in range(si,ei):
	#print "i=",i
	if numberList[i] <= pivot:
	    numberList[i], numberList[index] = numberList[index], numberList[i]
	    index = index + 1
	    #print "Index changes to:",index
    #swap the pivot with the number at index
    numberList[ei], numberList[index] = numberList[index], numberList[ei]
    return index




def quick_sort(numberList, si, ei):
    nele = ei - si 
    #print "qs from", si, "to", ei, "nele", nele
    if nele >= 1:
	index = partition(numberList, si, ei)
	quick_sort(numberList, si, index-1)
	quick_sort(numberList, index+1, ei)
    #print "qs numberList", numberList




def measure_time(f):
    def timed(*args, **kw):
        ts = time.time()
        result = f(*args, **kw)
        te = time.time()
        print '%r (%r, %r) %2.2f sec' % \
          (f.__name__, args, kw, te-ts)
        return result
    return timed

@measure_time
def qs(numberList, si, ei):
    quick_sort(numberList, si,ei)
