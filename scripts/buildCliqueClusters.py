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
import random

from numpy import argsort, mean

'''
Simple class for working with unweighted undirected graphs
@author: Oleksii Kuchaiev; http://www.kuchaev.com
'''
class Graph(object):
	'''
	A class for representing and manipulation undirected, unweighted simple graphs without self-loops
	'''
	def __init__(self, userID=None):
		'''
		Constructor
		'''
		self.Nodes=set() #set of nodes
		self.AdjList=dict() #Adjacency list

	def add_node(self,node):
		'''
		Adds node to the graph.
		'''
		if node in self.Nodes:
			raise Exception("Node "+node+" is already present in the graph.")
		else:
			self.Nodes.add(node)
			self.AdjList[node]=set()

	def add_edge(self,nd1,nd2):
		'''
		Adds edge (nd1,nd2) to the graph.
		'''
		if nd1 not in self.Nodes:
			raise Exception("Node "+nd1+" is not present in the graph.")
		if nd2 not in self.Nodes:
			raise Exception("Node "+nd2+" is not present in the graph.")
		if nd1 not in self.AdjList.keys():
			self.AdjList[nd1]=set()
			self.AdjList[nd1].add(nd2)
		else:
			self.AdjList[nd1].add(nd2)
		if nd2 not in self.AdjList.keys():
			self.AdjList[nd2]=set()
			self.AdjList[nd2].add(nd1)
		else:
			self.AdjList[nd2].add(nd1)

	def get_edge_set(self):
		'''
		Returns set of edges in the graph.        
		'''
		Edges=set()
		for nd1 in self.Nodes:
			N=self.get_node_neighbors(nd1)
			for nd2 in N:
				if (nd2,nd1) not in Edges:
					Edges.add((nd1,nd2))
		return Edges
       
	def number_of_nodes(self):
		'''
		Returns number of nodes in the graph.
		'''
		return len(self.Nodes)

	def number_of_edges(self):
		'''
		Returns number of edges in the graph.
		'''
		num_edg=0.0;
		for key in self.AdjList.keys():
			num_edg=num_edg+(float(len(self.AdjList[key]))/2)
		return int(num_edg)  

	def degree(self,node):
		'''
		Returns the degree of a node 
		'''
		if node not in self.Nodes:
			raise Exception("There is no node with name: "+str(node)+" in this graph. The id of the graph is: "+str(self.Id))
		return len(self.AdjList[node])

	def get_node_clust_coef(self,node):
		'''
		Returns the clustering coefficient of the node
		'''
		deg=self.degree(node)
		if deg<=1:
			return 0
		Ev=0
		neighbors=self.get_node_neighbors(node)
		for nd in neighbors:
			if self.are_adjacent(node, nd):
				Ev+=1
		cc=float(2*Ev)/(deg*(deg-1))
		return cc  

	def get_node_eccentricity(self,node):
		'''
		Returns the eccentricity of the node.
		Note that this function returns the eccentricity of a node within its
		connected component
		'''
		D=self.BFS(node)
		ec=0
		for key, value in D:
			if value>ec:
				ec=value
		return ec

	def get_node_eccentricity_avg(self,node):
		'''
		Returns the averaged eccentricity of the node. That is, "avg", not "max" distance
		Note that this function returns the eccentricity of a node within its
		connected component
		'''        
		D=self.BFS(node)
		ec=0.0
		counter=0.0
		for key, value in D.items():
			if value>0:
				ec+=value
				counter+=1
		if counter>0:
			return ec/counter
		else:
			return 0

	def get_node_eccentricities_both(self,node):
		'''
		This function is for performance purposes.
		This is function returns standard and averaged eccentricities of the node.
		Note that both eccentricities of the node are within its connected component
		'''
		D=self.BFS(node)
		ec=0
		ecA=0.0
		counter=0.0
		for key, value in D.items():
			if value>0:
				ecA+=value
				counter+=1
				if value>ec:
					ec=value
		if counter>0:
			return (ec,ecA/counter)
		else:
			return (ec,0)

	def are_adjacent(self,nd1,nd2):
		'''
		Checks if nd1 and nd2 are connected
		'''
		if nd1 not in self.Nodes:
			raise Exception("Node "+str(nd1)+" is not in the graph with id="+str(self.Id))    
		if nd2 not in self.Nodes:
			raise Exception("Node "+str(nd2)+" is not in the graph with id="+str(self.Id))
		if nd2 in self.AdjList[nd1]:
			return True
		else:
			return False

	def get_node_neighbors(self,nd):
		'''
		Returns set of node neighbors
		'''
		#if nd not in self.Nodes:
		   # raise Exception("Node "+str(nd)+" is not in the graph with id="+str(self.Id))
		return self.AdjList[nd]

	def BFS(self,source):
		'''
		Implements Breadth-first search from node 'source' in graph 'self'.
		Returns dictionary D {node: distance from source}
		distance=-1 if 'node' is unreachable from 'source'        
		'''
		#if source not in self.Nodes:
		 #   raise Exception("Node "+str(source)+" is not in the graph with id="+str(self.Id))        
		D=dict();
		for node in self.Nodes:
			D[node]=-1
		level=0;
		Que0=set()
		Que0.add(source)
		Que1=set()
		while len(Que0)!=0:
			while len(Que0)!=0:
				cur_node=Que0.pop()
				D[cur_node]=level
				N=self.AdjList[cur_node]
				for nd in N:
					if D[nd]==-1:
						Que1.add(nd)                        
			level=level+1
			Que0=Que1
			Que1=set()
		return D
		
	def connected_components(self):
		cc = {}
		ccId = 0
		processedNodes = set()
		for node in self.Nodes:
			if node in processedNodes:
				continue
				
			bfs = self.BFS(node)
			connectedNodes = set()
			for testNode in bfs:
				if bfs[testNode] != -1:
					connectedNodes.add(testNode)
					processedNodes.add(testNode)
					
			cc[ccId] = connectedNodes
			ccId += 1
			
		return cc

	def dist(self,nd1,nd2):
		'''
		Returns shortest-path distance between nd1 and nd2
		'''
		if nd1 not in self.Nodes:
			raise Exception("Node "+str(nd1)+" is not in the graph with id="+str(self.Id))
		if nd2 not in self.Nodes:
			raise Exception("Node "+str(nd2)+" is not in the graph with id="+str(self.Id))
		D=dict();
		for node in self.Nodes:
			D[node]=-1
		level=0;
		Que0=set()
		Que0.add(nd1)
		Que1=set()
		while len(Que0)!=0:
			while len(Que0)!=0:
				cur_node=Que0.pop()
				D[cur_node]=level
				if cur_node==nd2:
					return level
				N=self.get_node_neighbors(cur_node)
				for nd in N:
					if D[nd]==-1:
						Que1.add(nd)
			level=level+1;
			Que0=Que1;
			Que1=set()
		return -1  

	def all_pairs_dist(self):
		'''
		Returns dictionary of all-pairs shortest paths in 'self'
		The dictionary has format {t=(nd1,nd2): distance},
		where t is a tuple.                
		'''
		Distances=dict()
		count=0
		for nd in self.Nodes:
			DD1=self.BFS(nd)
			for key, value in DD1.items():
				t1=nd, key
				t2=key, nd
				Distances[t1]=float(value)
				Distances[t2]=float(value)
		return Distances

	def find_all_cliques(self):
		'''
		Implements Bron-Kerbosch algorithm, Version 2
		'''
		Cliques=[]
		Stack=[]
		nd=None
		disc_num=len(self.Nodes)
		search_node=(set(),set(self.Nodes),set(),nd,disc_num) 
		Stack.append(search_node)
		while len(Stack)!=0:
			(c_compsub,c_candidates,c_not,c_nd,c_disc_num)=Stack.pop()
			if len(c_candidates)==0 and len(c_not)==0:
				Cliques.append(c_compsub)
				continue
			for u in list(c_candidates):
				if (c_nd==None) or (not self.are_adjacent(u, c_nd)):
					c_candidates.remove(u)
					Nu=self.get_node_neighbors(u)                                
					new_compsub=set(c_compsub)
					new_compsub.add(u)
					new_candidates=set(c_candidates.intersection(Nu))
					new_not=set(c_not.intersection(Nu))                    
					if c_nd!=None:
						if c_nd in new_not:
							new_disc_num=c_disc_num-1
							if new_disc_num>0:
								new_search_node=(new_compsub,new_candidates,new_not,c_nd,new_disc_num)                        
								Stack.append(new_search_node)
						else:
							new_disc_num=len(self.Nodes)
							new_nd=c_nd
							for cand_nd in new_not:
								cand_disc_num=len(new_candidates)-len(new_candidates.intersection(self.get_node_neighbors(cand_nd))) 
								if cand_disc_num<new_disc_num:
									new_disc_num=cand_disc_num
									new_nd=cand_nd
							new_search_node=(new_compsub,new_candidates,new_not,new_nd,new_disc_num)                        
							Stack.append(new_search_node)                
					else:
						new_search_node=(new_compsub,new_candidates,new_not,c_nd,c_disc_num)
						Stack.append(new_search_node)
					c_not.add(u) 
					new_disc_num=0
					for x in c_candidates:
						if not self.are_adjacent(x, u):
							new_disc_num+=1
					if new_disc_num<c_disc_num and new_disc_num>0:
						new1_search_node=(c_compsub,c_candidates,c_not,u,new_disc_num)
						Stack.append(new1_search_node)
					else:
						new1_search_node=(c_compsub,c_candidates,c_not,c_nd,c_disc_num)
						Stack.append(new1_search_node)     
		return Cliques

def doWork(args):
	# read sequence length
	geneIdToLen = {}
	for line in open(args.seq_len):
		lineSplit = line.split('\t')
		geneIdToLen[lineSplit[0]] = float(lineSplit[1])
	
	# read compatibility matrix
	connections = {}
	ids = []
	index = 1
	possibleLinks = 0
	numLinks = 0
	for line in open(args.input):
		lineSplit = line.split('\t')
		id = lineSplit[0]
		ids.append(id)
		
		connections[id] = []
		for i in xrange(index, len(lineSplit)):
			possibleLinks += 1
			if float(lineSplit[i]) > 0:
				connections[id].append(i-1)
				numLinks +=1
		index += 1
		
	# calculate fraction of possible edges that have been made
	f = float(numLinks) / possibleLinks
	print '  Fraction of edges: %.2f' % f
		
	# create graph
	graph = Graph()
	for id in connections:
		graph.add_node(id)
		
	for id in connections:
		for x in connections[id]:
			graph.add_edge(id, ids[x])

	# find all maximal cliques
	cliques = graph.find_all_cliques()
	
	# write out cliques
	fout = open(args.output_prefix + '.cliques.txt', 'w')
	sortedCliques = sorted(cliques, key=len, reverse=True)
	i = 0
	for clique in sortedCliques:
		fout.write('Clique' + str(i) + '\t')
		fout.write('\t'.join(clique) + '\n')
		i += 1
	fout.close()
	
	# write out co-clique matrix
	fout = open(args.output_prefix + '.co-clique.tsv', 'w')
	coCliqueMatrix = []
	i = 0
	for cliqueI in sortedCliques:
		fout.write('Clique' + str(i))
		row = []
		for cliqueJ in sortedCliques:
			sharedNodes = len(cliqueI.intersection(cliqueJ))
			fout.write('\t' + str(sharedNodes))
			row.append(sharedNodes)
		coCliqueMatrix.append(row)
		i += 1
		fout.write('\n')
	fout.close()
	
	# build neighbourhood graph from co-clique matrix
	coCliqueGraph = Graph()
	for i in xrange(0, len(coCliqueMatrix)):
		coCliqueGraph.add_node(i)

	for i in xrange(0, len(coCliqueMatrix)):
		for j in xrange(i+1, len(coCliqueMatrix)):
			if coCliqueMatrix[i][j] >= (args.clique_size - 1):
				coCliqueGraph.add_edge(i, j)
		
	# find connected components (clique clusters) in co-clique matrix
	fout = open(args.output_prefix + '.cliqueClusters.tsv', 'w')
	connectedComponents = coCliqueGraph.connected_components()
	nodesInCliqueClusters = []
	for cc in connectedComponents:
		# get nodes in clique cluster
		uniqueNodes = set()
		for cliqueId in connectedComponents[cc]:
			uniqueNodes = uniqueNodes.union(sortedCliques[cliqueId])
			
		# get length of genes
		seqLens = []
		nodes = []
		for node in uniqueNodes:
			seqLens.append(geneIdToLen[node])
			nodes.append(node)
			
		lenAndNodes = zip(seqLens, nodes)
		lenAndNodes.sort(reverse = True)
		seqLens, nodes = zip(*lenAndNodes)
		
		fout.write('Cluster ' + str(cc) + '\n')
		cliqueStr = []
		for cliqueId in connectedComponents[cc]:
			cliqueStr.append('Clique' + str(cliqueId))
		fout.write('Cliques (' + str(len(connectedComponents[cc])) + '): ' + ','.join(cliqueStr) + '\n')
		
		fout.write('Nodes (' + str(len(nodes)) + '): ' + ','.join(nodes) + '\n')
		
		seqLenStr = []
		for seqLen in seqLens:
			seqLenStr.append(str(seqLen))
		fout.write('Seq. Length (%.2f' % mean(seqLens) + '): ' + ','.join(seqLenStr) + '\n\n')
		nodesInCliqueClusters.append(len(nodes))
	fout.close()
	
	nodesInCliqueClusters.sort(reverse=True)
	print '  Size of largest cluster: ' + str(nodesInCliqueClusters[0])
	print '  Size of 2nd largest cluster: ' + str(nodesInCliqueClusters[1])
	print ''
	
	if nodesInCliqueClusters[0] > 2*nodesInCliqueClusters[0]:
		print '  Community is highly structured according to Vicsek criterion.'
		return True
		
	return False
			
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('input', help='Compatibility matrix.')
	parser.add_argument('seq_len', help='File indicating length of genes.')
	parser.add_argument('output_prefix', help='Output prefix.')
	parser.add_argument('-k', '--clique_size', help='Build clusters from adjacent k-cliques.', type=int, default=4)

	args = parser.parse_args()
	
	doWork(args)