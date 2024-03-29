#!/usr/bin/env python

__author__ = 'Donovan Parks'
__copyright__ = 'Copyright 2013'
__credits__ = ['Donovan Parks']
__license__ = 'GPL3'
__version__ = '1.0.0'
__maintainer__ = 'Donovan Parks'
__email__ = 'donovan.parks@gmail.com'
__status__ = 'Development'

import sys, argparse

def doWork(args):
	# read adjacency matrix
	connections = {}
	ids = []
	index = 1
	for line in open(args.input):
		lineSplit = line.split('\t')
		id = lineSplit[0]
		ids.append(id)
		
		connections[id] = []
		for i in xrange(index, len(lineSplit)):
			if float(lineSplit[i]) > 0:
				connections[id].append(i-1)
		index += 1
		
	# write SIF format graph file
	fout = open(args.output, 'w')
	for id in connections:
		fout.write(id + '\tcompatible')
		for x in connections[id]:
			fout.write('\t' + ids[x])
		fout.write('\n')
			
	fout.close()
			
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('input', help='Compatibility matrix.')
	parser.add_argument('output', help='Output file in SIF format.')

	args = parser.parse_args()        

	doWork(args)