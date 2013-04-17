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

def readMatrix(file):
	idsToIndex = {}
	matrix = []
	index = 0
	for line in open(file):
		lineSplit = line.split('\t')
		idsToIndex[lineSplit[0]] = index
		row = [x.strip() for x in lineSplit[1:]]
		matrix.append(row)
		index += 1
		
	return matrix, idsToIndex
	
def writeSubMatrix(fout, subMatrixIds, matrix, idsToIndex):
	average = []
	for id1 in subMatrixIds:
		fout.write(id1)
		index1 = idsToIndex[id1]
		
		rowSum = 0
		outputStr = ''
		for id2 in subMatrixIds:
			index2 = idsToIndex[id2]
			
			d = matrix[index1][index2]
			outputStr += '\t' + d
			
			if id1 != id2:
				rowSum += float(d)
				average.append(float(d))
			
		if len(subMatrixIds) > 1:
			fout.write(' (%.1f' % (float(rowSum) / (len(subMatrixIds)-1))  + ')' + outputStr + '\n')
		else:
			fout.write(outputStr + '\n')
		
	if len(average) != 0:
		fout.write('Average: %.1f' % (float(sum(average))/len(average)) + '\n')

def doWork(args):
	# read clustering data
	clusterIdToGeneIds = {}
	clusterIds = []
	for line in open(args.inputDir + '/' + args.inputPrefix + '.clustering.txt'):
		if line.strip() == '':
			continue 
			
		if line[0] == '%':
			clusterId = line[1:].strip()
			clusterIds.append(clusterId)
		else:
			clusterIdToGeneIds[clusterId] = clusterIdToGeneIds.get(clusterId, []) + [line.split('\t')[0]]
			
	# read matrices
	compatibilityMatrix, compatibilityIdsToIndex = readMatrix(args.inputDir + '/' + args.inputPrefix + '.compatibility.tsv')
	sharedMatrix, sharedIdsToIndex = readMatrix(args.inputDir + '/' + args.inputPrefix + '.shared.tsv')
	splitsMatrix, splitsIdsToIndex = readMatrix(args.inputDir + '/' + args.inputPrefix + '.splits.tsv')
	
	# write out sub-matrices for each cluster
	fout = open(args.output, 'w')
	for clusterId in clusterIds:
		fout.write(clusterId + '\n')
		fout.write('\nCompatibility matrix:\n')
		writeSubMatrix(fout, clusterIdToGeneIds[clusterId], compatibilityMatrix, compatibilityIdsToIndex)
		fout.write('\nShared taxa matrix:\n')
		writeSubMatrix(fout, clusterIdToGeneIds[clusterId], sharedMatrix, sharedIdsToIndex)
		fout.write('\nSplits matrix:\n')
		writeSubMatrix(fout, clusterIdToGeneIds[clusterId], splitsMatrix, splitsIdsToIndex)
		fout.write('-'*40 + '\n')
	fout.close()
			
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('inputDir', help='Input directory.')
	parser.add_argument('inputPrefix', help='Input prefix.')
	parser.add_argument('output', help='Output file.')

	args = parser.parse_args()        

	doWork(args)