#!/usr/bin/env python

__author__ = 'Donovan Parks'
__copyright__ = 'Copyright 2013'
__credits__ = ['Donovan Parks']
__license__ = 'GPL3'
__version__ = '1.0.0'
__maintainer__ = 'Donovan Parks'
__email__ = 'donovan.parks@gmail.com'
__status__ = 'Development'

import os, sys, argparse

def doWork(args):
	fout = open(args.output, 'w')
	
	files = os.listdir(args.inputDir)
	for file in files:
		if not file.endswith('.seq'):
			continue
			
		alignmentLen = 0
		seqLen = []
		for line in open(args.inputDir + '/' + file):
			if line[0] != '>':
				alignmentLen = len(line.strip())
				seqLen.append(len(line.replace('-','').strip()))
			
		fout.write(file[0:file.rfind('.')] + '\t%.2f' % (float(sum(seqLen))/len(seqLen)) + '\t' + str(alignmentLen) + '\n')
	fout.close()
		
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('inputDir', help='Input directory containing sequence files (.seq extension).')
	parser.add_argument('output', help='Output file.')

	args = parser.parse_args()        

	doWork(args)