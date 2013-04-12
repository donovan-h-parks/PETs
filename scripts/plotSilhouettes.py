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
from numpy import mean

def doWork(args):
	# read dissimilarity matrix
	geneIdsToIndex = {}
	dissMatrix = []
	index = 0
	for line in open(args.dissMatrix):
		lineSplit = line.split('\t')
		geneIdsToIndex[lineSplit[0]] = index
		row = [float(x) for x in lineSplit[1:]]
		dissMatrix.append(row)
		index += 1
	
	# read clustering data
	clusterIdToGeneIds = {}
	clusterIds  = []
	for line in open(args.cluster):
		if line.strip() == '':
			continue 
			
		if line[0] == '%':
			clusterId = line[1:].strip()
			clusterIds.append(clusterId)
		else:
			clusterIdToGeneIds[clusterId] = clusterIdToGeneIds.get(clusterId, []) + [line.strip()]
			
	# create colour map for clusters
	clusterIdToColour = {}
	index = 0
	for clusterId in sorted(clusterIds):
		rgb = tuple([int(c*255) for c in hsv_to_rgb(float(index)/len(clusterIds), 0.4, 1.0)])
		clusterIdToColour[clusterId] = '#%02X%02X%02X' % rgb
		index += 1
		
	# calculate silhouette value for each gene
	silhouettes = {}
	colours = []
	for srcClusterId in clusterIdToGeneIds:
		silhouettes[srcClusterId] = []
		minInterDiss = 1e6
		for srcGeneId in clusterIdToGeneIds[srcClusterId]:
			colours.append(clusterIdToColour[srcClusterId])
			row = dissMatrix[geneIdsToIndex[srcGeneId]]
			
			for dstClusterId in clusterIdToGeneIds:
				diss = []
				for dstGeneId in clusterIdToGeneIds[dstClusterId]:
					if srcGeneId == dstGeneId:
						continue
						
					diss.append(row[geneIdsToIndex[dstGeneId]])
					
				meanDiss = mean(diss)
				if srcClusterId == dstClusterId:
					intraDiss = meanDiss
				elif meanDiss < minInterDiss:
					minInterDiss = meanDiss
				
			s = (minInterDiss - intraDiss) / max(minInterDiss, intraDiss)
			silhouettes[srcClusterId].append(s)
		
	data = []
	for clusterId in silhouettes:
		sil = silhouettes[clusterId]
		data += sorted(sil, reverse=True)
		
	# plot results
	fig = plt.figure()
	fig.set_size_inches(args.width, args.width)
	ax = fig.add_axes([0.13,0.08,.82,.87])

	ax.set_title('Silhouettes (mean = %0.2f' % mean(data) + ')', fontsize=10)
	ax.set_xlabel('Cluster', fontsize=10)
	ax.set_ylabel('Silhouette', fontsize=10)

	ind = np.arange(len(data))
	ax.bar(ind, data, color = colours)

	for tick in ax.xaxis.get_major_ticks():
		tick.label.set_fontsize(8) 

	for tick in ax.yaxis.get_major_ticks():
		tick.label.set_fontsize(8)
		
	fig.savefig(args.output, dpi = args.dpi)
			
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('dissMatrix', help='Dissimilarity matrix.')
	parser.add_argument('cluster', help='File indicating clustering of genes.')
	parser.add_argument('output', help='Output image file. Specify format with extension: .jpg, .png, .pdf, .svg.')
	parser.add_argument('--dpi', help='Resolution of output image (default = 600).', type=int, default=600)
	parser.add_argument('-w', '--width', help='Width of image in inches (default = 6).', type=float, default=6)
	parser.add_argument('-l', '--legend', help='Show legend.', action='store_true')

	args = parser.parse_args()        

	doWork(args)