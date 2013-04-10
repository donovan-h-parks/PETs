#!/usr/bin/env python

__author__ = 'Donovan Parks'
__copyright__ = 'Copyright 2013'
__credits__ = ['Donovan Parks']
__license__ = 'GPL3'
__version__ = '1.0.0'
__maintainer__ = 'Donovan Parks'
__email__ = 'donovan.parks@gmail.com'
__status__ = 'Development'

import os, argparse
from ete2 import Tree, NodeStyle, TreeStyle, TextFace, CircleFace, AttrFace, faces
from colorsys import hsv_to_rgb

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

	# read tree
	tree = Tree(args.tree)

	# define node properties
	for node in tree.traverse('levelorder'):
		if node.is_leaf():
			name = node.name.replace("'", '')
			clusterId = geneIdToClusterId[name]
			node.add_feature('clusterId', clusterId)
			
			if args.label:
				node.name = ' ' + name + ' [' + clusterId + ']'
			
			ns = NodeStyle()
			ns['shape'] = 'circle'
			ns['fgcolor'] = 'black'
			ns['size'] = 4
			node.set_style(ns)
		else:
			ns = NodeStyle()
			ns['size'] = 0
			node.set_style(ns)
		
	# determine colour for each node
	assignedColor = set()
	for node in tree.traverse('levelorder'):
		if node.is_leaf() or node.is_root():
			continue
			
		# determine clusters descendent from this node
		clusterSet = set()
		for leaf in node.get_leaves():
			clusterSet.add(leaf.clusterId)
			
		# check if parent has already been assigned a colour
		bContinue = False
		parentNode = node.up
		while not parentNode.is_root():
			if parentNode in assignedColor:
				bContinue = True
				break
				
			parentNode = parentNode.up
			
		if bContinue:
			continue

		# colour node if all descendants are from the same cluster
		if len(clusterSet) == 1:
			clusters = list(clusterSet)
			ns = NodeStyle()
			ns['bgcolor'] = clusterIdToColour[clusters[0]]
			ns['size'] = 0
			node.set_style(ns)
			assignedColor.add(node)

	# create tree style for plotting consisteny index trees
	treeStyle = TreeStyle()
	treeStyle.margin_left = 10
	treeStyle.margin_right = 10
	treeStyle.margin_top = 10
	treeStyle.margin_bottom = 10
	treeStyle.guiding_lines_type = 1
	treeStyle.legend_position=1
	treeStyle.scale = 500
	
	# add legend
	rowIndex = 0
	for clusterId in sorted(clusterIdToColour.keys()):
		treeStyle.legend.add_face(CircleFace(4, clusterIdToColour[clusterId]), column=0)
		treeStyle.legend.add_face(TextFace(' ' + clusterId), column=1)
		rowIndex += 1

	tree.render(args.output, dpi=args.dpi, w = int(args.width*args.dpi + 0.5), tree_style=treeStyle)

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('tree', help='Hierarchical cluster tree.')
	parser.add_argument('cluster', help='File indicating clustering of genes.')
	parser.add_argument('output', help='Output image file. Specify format with extension: .jpg, .png, .pdf, .svg.')
	parser.add_argument('--dpi', help='Resolution of output image (default = 600).', type=int, default=600)
	parser.add_argument('-w', '--width', help='Width of image in inches (default = 6).', type=float, default=6)
	parser.add_argument('-l', '--label', help='Label leaf nodes with cluster id.', action='store_true')

	args = parser.parse_args()        

	doWork(args)