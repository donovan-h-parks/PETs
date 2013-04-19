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
	# read clique clusters
	nodesInSubgraph = set()
	clusterCount = 0
	nodeToClusterAttr = {}
	for line in open(args.clique_clusters):
		if 'Node' in line:
			lineSplit = line.split(':')
			nodes = set([x.strip() for x in lineSplit[1].split(',')])
			nodesInSubgraph = nodesInSubgraph.union(nodes)
			
			for node in nodes:
				nodeToClusterAttr[node] = nodeToClusterAttr.get(node, '') + 'Cluster' + str(clusterCount)
			
			clusterCount += 1
			if clusterCount == args.num_clusters:
				break
		
	# read compatibility matrix
	connections = {}
	ids = []
	index = 1
	for line in open(args.compatibility_matrix):
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
		if id not in nodesInSubgraph:
			continue
			
		fout.write(id + '\tcompatible')
		for x in connections[id]:
			if ids[x] in nodesInSubgraph:
				fout.write('\t' + ids[x])
		fout.write('\n')
			
	fout.close()
	
	# write node attribute file
	fout = open(args.output + '.nodeAttr.txt', 'w')
	fout.write('ClusterAttribute\n')
	for node in nodeToClusterAttr:
		fout.write(node + ' = ' + nodeToClusterAttr[node] + '\n')
	fout.close()

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('compatibility_matrix', help='Compatibility matrix.')
	parser.add_argument('clique_clusters', help='Clique clusters.')
	parser.add_argument('num_clusters', help='Build subgraph from top N clique clusters.', type = int)
	parser.add_argument('output', help='Output file in SIF format.')

	args = parser.parse_args()

	doWork(args)