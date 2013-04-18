#!/usr/bin/env python

__author__ = 'Donovan Parks'
__copyright__ = 'Copyright 2013'
__credits__ = ['Donovan Parks']
__license__ = 'GPL3'
__version__ = '1.0.0'
__maintainer__ = 'Donovan Parks'
__email__ = 'donovan.parks@gmail.com'
__status__ = 'Development'

import sys, argparse, random, math

import matplotlib
import matplotlib.pyplot as plt
from matplotlib import cm
from numpy import concatenate, linspace
import numpy as np

# Perform simulated annealing to minimize cost of dissimilarity matrix
def simulatedAnnealingSort(matrix, labels):
	N = len(matrix)
	
	curOrdering = []
	for i in xrange(0, N):
		curOrdering.append(i)

	# calculate cost of initial ordering
	C = 0.0
	for i in xrange(0, N):
		for j in xrange(i+1, N):
			C += (1.0 - matrix[i][j])*(j-i)
	C = 2*C / N

	notChanged = 0
	T = -1.0
	iter = 0
	currentBestC = C
	while notChanged < 20:
		lastIterBestC = currentBestC;
		for move in xrange(0, max(N, 100)):
			# generate new state by swapping two adjacent elements
			r = random.randint(0, N-2)
			
			temp = curOrdering[r]
			curOrdering[r] = curOrdering[r+1]
			curOrdering[r+1] = temp

			# calculte cost of new ordering
			C = 0.0
			for i in xrange(0, N):
				for j in xrange(i+1, N):
					C += (1.0 - matrix[curOrdering[i]][curOrdering[j]])*(j-i)
			C = 2*C / N

			# set initial temperature on first iteration
			if T < 0.0:
				T = -abs(currentBestC - C) / math.log(0.5)

			# accept new ordering with a given probability
			acceptanceProb = math.exp((currentBestC - C)/T);
			prob = random.random()
			if prob <= acceptanceProb:
				currentBestC = C
			else:
				temp = curOrdering[r]
				curOrdering[r] = curOrdering[r+1]
				curOrdering[r+1] = temp

		if currentBestC == lastIterBestC:
			notChanged += 1
		else:
			notChanged = 0

		T *= 0.98
		iter += 1

	# Set sorted matrix and labels
	sortedMatrix = []
	sortedLabels = []
	for indexR in curOrdering:
		row = []
		for indexC in curOrdering:
			row.append(matrix[indexR][indexC])
		sortedMatrix.append(row)
		sortedLabels.append(labels[indexR])

	return sortedMatrix, sortedLabels

def cmapDiscretize(cmap, N):
	"""Return a discrete colormap from the continuous colormap cmap.

		cmap: colormap instance, eg. cm.jet. 
		N: number of colors.

	Example
		x = resize(arange(100), (5,100))
		djet = cmap_discretize(cm.jet, 5)
		imshow(x, cmap=djet)
	"""

	if type(cmap) == str:
		cmap = get_cmap(cmap)
	colors_i = concatenate((linspace(0, 1., N), (0.,0.,0.,0.)))
	colors_rgba = cmap(colors_i)
	indices = linspace(0, 1., N+1)
	cdict = {}
	for ki,key in enumerate(('red','green','blue')):
		cdict[key] = [ (indices[i], colors_rgba[i-1,ki], colors_rgba[i,ki]) for i in xrange(N+1) ]
	# Return colormap object.
	return matplotlib.colors.LinearSegmentedColormap(cmap.name + "_%d"%N, cdict, 1024)

def doWork(args):
	# read data
	labels = []
	matrix = []
	for line in open(args.matrix):
		lineSplit = line.split('\t')
		label = lineSplit[0]
		labels.append(label)
		row = [float(x) for x in lineSplit[1:]]
		matrix.append(row)
		
	# sort matrix
	sortedMatrix, sortedLabels = simulatedAnnealingSort(matrix, labels)
	sortedMatrix = np.array(sortedMatrix)

	# create heatmap
	fig = plt.figure()
	fig.set_size_inches(args.width, args.height)
	ax = fig.add_axes([0.1,0.05,.85,.85])

	heatmap = ax.imshow(sortedMatrix, cmap=cmapDiscretize(plt.cm.binary, 20), interpolation='nearest')
	
	if args.legend:
		cbar = fig.colorbar(heatmap)

	# put the major ticks at the middle of each cell
	ax.set_xticks(np.arange(sortedMatrix.shape[0]), minor=False)
	ax.set_yticks(np.arange(sortedMatrix.shape[1]), minor=False)

	# want a more natural, table-like display
	ax.invert_yaxis()
	ax.invert_xaxis()
	ax.xaxis.tick_top()

	ax.set_xticklabels(sortedLabels, minor=False, rotation = 90, size = 8)
	ax.set_yticklabels(sortedLabels, minor=False, size = 8)
	
	fig.savefig(args.output, dpi = args.dpi)
	
if __name__ == '__main__':
	random.seed()
	
	parser = argparse.ArgumentParser()
	parser.add_argument('matrix', help='Compatibility matrix.')
	parser.add_argument('output', help='Output file.')
	parser.add_argument('--dpi', help='Resolution of output image (default = 600).', type=int, default=600)
	parser.add_argument('--width', help='Width of image in inches (default = 6).', type=float, default=6)
	parser.add_argument('--height', help='Height of image in inches (default = 6).', type=float, default=6)
	parser.add_argument('-l', '--legend', help='Show legend.', action='store_true')

	args = parser.parse_args()        

	doWork(args)