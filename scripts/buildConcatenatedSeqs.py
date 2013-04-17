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
	# read cluster file
	clusters = {}
	clusterId = ''
	for line in open(args.input):
		if line.strip() == '':
			continue
			
		if line[0] == '%':
			if clusterId != '':
				clusters[clusterId] = geneTrees
				
			clusterId = line[1:].strip()
			geneTrees = []
		else:
			geneTrees.append(line.strip())
		clusters[clusterId] = geneTrees
		
	# build concatenated alignments
	for clusterId in clusters:
		geneTrees = clusters[clusterId]
		
		if len(geneTrees) < args.minClusterSize:
			continue
			
		# read alignment for each gene
		taxa = set()
		concatenate = {}
		for geneTreeId in geneTrees:
			seqs = {}
			taxaId = ''
			for line in open(args.inputDir + '/' + geneTreeId + '.seq'):
				if line[0] == '>':
					if taxaId != '':
						seqs[taxaId] = seq
					taxaId = line[1:].strip()
					taxa.add(taxaId)
				else:
					seq = line.strip()
					
			seqs[taxaId] = seq
			concatenate[geneTreeId] = seqs
			
		# write out concatenated alignment
		fout = open(args.outputDir + '/' + clusterId + '.fna', 'w')
		for taxaId in taxa:
			for geneTreeId in concatenate:
				seqs = concatenate[geneTreeId]
				
				fout.write('>' + taxaId + '\n')
				if taxaId in seqs:
					fout.write(seqs[taxaId] + '\n')
				else:
					seqLen = len(seqs[seqs.keys()[0]])
					fout.write('-'*seqLen + '\n')
		fout.close()
		
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('input', help='Cluster file.')
	parser.add_argument('inputDir', help='Directory containing sequence alignments.')
	parser.add_argument('outputDir', help='Output directory.')
	parser.add_argument('-m','--minClusterSize', help='Required gene trees in cluster required before inferring a concatenated tree.', type=int, default=5)

	args = parser.parse_args()        

	doWork(args)