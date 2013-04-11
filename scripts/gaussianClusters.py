#!/usr/bin/env python

__author__ = 'Donovan Parks'
__copyright__ = 'Copyright 2013'
__credits__ = ['Donovan Parks']
__license__ = 'GPL3'
__version__ = '1.0.0'
__maintainer__ = 'Donovan Parks'
__email__ = 'donovan.parks@gmail.com'
__status__ = 'Development'

import os, argparse, random
from math import sqrt
from colorsys import hsv_to_rgb

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.patches import Ellipse

def doWork(args):
	# calculate random gaussian clusters
	clusters = []
	for c in xrange(0, args.clusters):
		muX = random.uniform(0, 100)
		muY = random.uniform(0, 100)
		sigmaX = random.uniform(0, 10)
		sigmaY = random.uniform(0, 10)
		
		x = []
		y = []
		for pt in xrange(0, args.pts):
			x.append(random.gauss(muX, sigmaX))
			y.append(random.gauss(muY, sigmaY))
			
		clusters.append([x, y])
	
	# generate plot of clusters
	if(args.img):
		fig = plt.figure()
		fig.set_size_inches(6, 6)
		ax = fig.add_axes([0.13,0.08,.82,.87])

		index = 0
		for cluster in clusters:
			colour = hsv_to_rgb(float(index)/len(clusters), 0.4, 1.0)
			ax.scatter(cluster[0], cluster[1], s = 18, lw=0.5, c = colour, label='Cluster ' + str(index))
			index += 1

		for tick in ax.xaxis.get_major_ticks():
			tick.label.set_fontsize(8) 

		for tick in ax.yaxis.get_major_ticks():
			tick.label.set_fontsize(8)
		
		fig.savefig(args.img, dpi = 96)
		
	# calculate Euclidean distance between clusters
	ptsX = []
	ptsY = []
	for cluster in clusters:
		ptsX += cluster[0]
		ptsY += cluster[1]
		
	dist = []
	for i in xrange(0, len(ptsX)):
		row = []
		for j in xrange(0, len(ptsX)):
			d = (ptsX[i]-ptsX[j])*(ptsX[i]-ptsX[j]) + (ptsY[i]-ptsY[j])*(ptsY[i]-ptsY[j])
			d = sqrt(d)
			row.append(d)
			
		dist.append(row)
	
	# write results
	fout = open(args.output, 'w')
	for r in xrange(0, len(dist)):
		fout.write('Pt ' + str(r+1))
		for c in xrange(0, len(dist[r])):
			fout.write('\t' + str(dist[r][c]))
		fout.write('\n')
	fout.close()
	
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('output', help='Output file.')
	parser.add_argument('-c','--clusters', help='Number of clusters to generate (default = 3).', type=int, default=3)
	parser.add_argument('-p','--pts', help='Points per cluster (default = 100).', type=int, default=100)
	parser.add_argument('-i','--img', help='Create plot of random gaussian clusters.')

	args = parser.parse_args()        

	doWork(args)