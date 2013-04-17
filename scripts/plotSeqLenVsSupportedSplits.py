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

from colorsys import hsv_to_rgb

from math import atan2, pi, sin, cos

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.patches import Ellipse

import numpy as np
from numpy import mean, min, cov, dot, linalg

def doWork(args):
	# read clustering data
	geneIdToSupportedSplits = {}
	for line in open(args.cluster):
		if line.strip() == '' or line[0] == '%':
			continue 
			
		lineSplit = line.split('\t')
		geneIdToSupportedSplits[lineSplit[0]] = int(lineSplit[2])
		
	# read sequence length
	geneIdToLen = {}
	for line in open(args.seqLen):
		lineSplit = line.split('\t')
		geneIdToLen[lineSplit[0]] = float(lineSplit[1])
			
	# plot results
	x = []
	y = []
	for geneId in geneIdToSupportedSplits:
		x.append(geneIdToLen[geneId])
		y.append(geneIdToSupportedSplits[geneId])

	fig = plt.figure()
	fig.set_size_inches(args.width, args.width)
	ax = fig.add_axes([0.13,0.08,.82,.87])

	ax.set_xlabel('Sequence Length', fontsize=10)
	ax.set_ylabel('Supported Splits', fontsize=10)

	ax.scatter(x, y, s = 18, lw=0.5)

	for tick in ax.xaxis.get_major_ticks():
		tick.label.set_fontsize(8) 

	for tick in ax.yaxis.get_major_ticks():
		tick.label.set_fontsize(8)
		
	fig.savefig(args.output, dpi = args.dpi)
			
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('cluster', help='File indicating clustering of genes.')
	parser.add_argument('seqLen', help='File indicating length of genes.')
	parser.add_argument('output', help='Output image file. Specify format with extension: .jpg, .png, .pdf, .svg.')
	parser.add_argument('--dpi', help='Resolution of output image (default = 600).', type=int, default=600)
	parser.add_argument('-w', '--width', help='Width of image in inches (default = 6).', type=float, default=6)

	args = parser.parse_args()        

	doWork(args)