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

def boundingBox(data, ax):
	A = np.array(data)

	M = (A-mean(A.T,axis=1)).T 			# subtract the mean (along columns)
	[latent,coeff] = linalg.eig(cov(M)) # attention: not always sorted
	score = dot(coeff.T,M) 				# projection of the data in the new space
	
	# determine centre of cluster
	projCenterXY = (score.min(axis=1)[0] + 0.5*(score.max(axis=1)[0]-score.min(axis=1)[0]), score.min(axis=1)[1] + 0.5*(score.max(axis=1)[1]-score.min(axis=1)[1]))
	centreXY = dot(coeff, projCenterXY) + mean(A, axis=0)

	width = max(score[0])-min(score[0])
	height = max(score[1])-min(score[1])

	angle = atan2(coeff[0,1], coeff[0,0])*(180.0/pi)
	if(angle < 0):
		angle = atan2(coeff[0,1], -coeff[0,0])*(180.0/pi)
	
	# bounding rectangle
	r = Rectangle((centreXY[0] - width*0.5, centreXY[1] - 0.5*height), width, height)
	t = mpl.transforms.Affine2D().rotate_deg_around(centreXY[0], centreXY[1], angle) + ax.transData
	r.set_transform(t)

	ax.add_artist(r)
	r.set_clip_box(ax.bbox)
	r.set_alpha(0.1)
	r.set_facecolor((0.5, 0.5, 0.5))
	
	return Ellipse(xy = centreXY, width = width, height = height, angle = angle)

def doWork(args):
	# read clustering data
	geneIdToClusterId = {}
	clusterIds  = []
	for line in open(args.cluster):
		if line[0] == '%':
			clusterId = line[1:].strip()
			clusterIds.append(clusterId)
		else:
			geneIdToClusterId[line.strip()] = clusterId
			
	# create colour map for clusters
	clusterIdToColour = {}
	index = 0
	for clusterId in clusterIds:
		rgb = tuple([int(c*255) for c in hsv_to_rgb(float(index)/len(clusterIds), 0.4, 1.0)])
		clusterIdToColour[clusterId] = '#%02X%02X%02X' % rgb
		index += 1
			
	# read PCoA results
	fin = open(args.pcoa)
	data = fin.readlines()
	fin.close()
	
	clusterIds = []
	geneIds = data[1].split('\t')
	for geneId in geneIds:
		clusterId = geneIdToClusterId[geneId.strip()]
		clusterIds.append(clusterId)
		binId = line[0:line.rfind('.')]

	x = {}
	y = {}
	xPts = data[2].split('\t')
	yPts = data[3].split('\t')
	for i in xrange(0, len(geneIds)):
		clusterId = clusterIds[i]
		x[clusterId] = x.get(clusterId, []) + [float(xPts[i])]
		y[clusterId] = y.get(clusterId, []) + [float(yPts[i])]
		
	var1 = float(data[len(geneIds)+4])
	var2 = float(data[len(geneIds)+5])
		
	# plot results
	fig = plt.figure()
	fig.set_size_inches(args.width, args.width)
	ax = fig.add_axes([0.13,0.08,.82,.87])

	ax.set_xlabel('PC1 (%0.1f' % (var1*100) + '%)', fontsize=10)
	ax.set_ylabel('PC2 (%0.1f' % (var2*100) + '%)', fontsize=10)

	#boundingBox(binPts[binId], ax)
	for clusterId in sorted(x.keys()):
		ax.scatter(x[clusterId], y[clusterId], s = 18, lw=0.5, c = clusterIdToColour[clusterId], label=clusterId)

	for tick in ax.xaxis.get_major_ticks():
		tick.label.set_fontsize(8) 

	for tick in ax.yaxis.get_major_ticks():
		tick.label.set_fontsize(8)

	if args.legend:
		legend = ax.legend(loc=0, scatterpoints = 1, fontsize=8)
		legend.get_frame().set_linewidth(0.25)
		
	fig.savefig(args.output, dpi = args.dpi)
			
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('pcoa', help='Results of PCoA analysis.')
	parser.add_argument('cluster', help='File indicating clustering of genes.')
	parser.add_argument('output', help='Output image file. Specify format with extension: .jpg, .png, .pdf, .svg.')
	parser.add_argument('--dpi', help='Resolution of output image (default = 600).', type=int, default=600)
	parser.add_argument('-w', '--width', help='Width of image in inches (default = 6).', type=float, default=6)
	parser.add_argument('-l', '--legend', help='Show legend.', action='store_true')

	args = parser.parse_args()        

	doWork(args)