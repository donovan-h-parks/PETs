//=======================================================================
// Author: Donovan Parks
//
// Copyright 2013 Donovan Parks
//
// This file is part of ClusterTree.
//
// ClusterTree is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ClusterTree is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ClusterTree. If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#include "Precompiled.hpp"

#include "HierarchicalClustering.hpp"
#include "NeighbourJoining.hpp"


void HierarchicalClustering::clustering(CLUSTER_TYPE clusterType, const Matrix& _distMatrix, const std::vector<std::string>& labels, Tree* tree)
{
	if(clusterType == NEIGHBOUR_JOINING)
	{
		NJ(_distMatrix, labels, tree);
	}
	else
	{
		// keep original distance matrix unmodified
		Matrix distMatrix = _distMatrix;

		// create initial singleton clusters
		std::vector<Node*> clusters;
		for(uint i = 0; i < labels.size(); ++i)
		{
			Node* node = new Node(labels.at(i));
			clusters.push_back(node);
		}

		// perform hierarchical clustering
		while(distMatrix.size() > 1)
		{
			// find nearest clusters
			uint row = 0, col = 0;
			findNearestClusters(distMatrix, row, col);
			
			// update distance matrix 
			double dist = distMatrix.at(row).at(col);
			updateDistanceMatrix(clusterType, distMatrix, clusters, row, col);

			// update clusters in the cluster tree
			updateClusters(clusters, row, col, dist);
		}

		if(clusters.size() != 1)
		{
			std::cout << "Error in clustering algorithm. Cluster size != 1.";
			return;
		}

		tree->root(clusters.at(0));
	}

		// set name of tree to reflect clustering method
	if(clusterType == HierarchicalClustering::COMPLETE_LINKAGE)
		tree->name("Complete linkage");
	else if(clusterType == HierarchicalClustering::SINGLE_LINKAGE)
		tree->name("Single linkage");
	else if(clusterType == HierarchicalClustering::AVERAGE_LINKAGE)
		tree->name("Average linkage (UPGMA)");
	else if(clusterType == HierarchicalClustering::NEIGHBOUR_JOINING) 
		tree->name("Neighbour joining");
}

void HierarchicalClustering::findNearestClusters(Matrix& distMatrix, uint& row, uint& col)
{
	double minEntry = DBL_MAX;

	for(uint i = 0; i < distMatrix.size()-1; ++i)
	{
		for(uint j = i+1; j < distMatrix.at(i).size(); ++j)
		{
			if(distMatrix.at(i).at(j) < minEntry)
			{
				row = i;
				col = j;
				minEntry = distMatrix.at(i).at(j);
			}
		}
	}
}

double HierarchicalClustering::distanceToNode(Node* node)
{
	std::vector<Node*> leafNodes = node->leaves();
	
	// trace path from leaf node up to given node
	double dist = 0;
	Node* curNode = leafNodes.at(0);
	while(curNode != node)
	{
		dist += curNode->distanceToParent();
		curNode = curNode->parent();
	}

	return dist;
}

void HierarchicalClustering::updateClusters(std::vector<Node*>& clusters, uint row, uint col, double value)
{
	// create new node
	Node* node = new Node;

	// set children of this node
	node->addChild(clusters.at(row));
	node->addChild(clusters.at(col));

	// set distance from children to parent
	clusters.at(row)->distanceToParent(value - distanceToNode(clusters.at(row)));
	clusters.at(col)->distanceToParent(value - distanceToNode(clusters.at(col)));

	// add in new cluster to cluster vector and remove children clusters
	clusters[row] = node;
	clusters.erase(clusters.begin() + col);
}

void HierarchicalClustering::updateDistanceMatrix(CLUSTER_TYPE clusterType, Matrix& distMatrix, std::vector<Node*>& clusters, uint row, uint col)
{
	// find distance from each element to the new cluster
	std::vector<double> dist;
	for(uint i = 0; i < distMatrix.size(); ++i)
	{
		dist.push_back(distMatrix.at(row).at(i));
	}

	for(uint i = 0; i < distMatrix.size(); ++i)
	{
		if(clusterType == COMPLETE_LINKAGE)
		{
			if(distMatrix.at(col).at(i) > dist.at(i))
				dist[i] = distMatrix.at(col).at(i);
		}
		else if(clusterType == SINGLE_LINKAGE)
		{
			if(distMatrix.at(col).at(i) < dist.at(i))
				dist[i] = distMatrix.at(col).at(i);
		}
		else if(clusterType == AVERAGE_LINKAGE)
		{
			uint sizeClusterI = clusters.at(row)->leaves().size();
			uint sizeClusterJ = clusters.at(col)->leaves().size();
			dist[i] = (sizeClusterI*dist[i] + sizeClusterJ*distMatrix.at(col).at(i)) / (sizeClusterI + sizeClusterJ);
		}
	}

	// update distance matrix (place new cluster at the row position)
	for(uint i = 0; i < distMatrix.size(); ++i)
	{
		if(row != (uint)i)
		{
			distMatrix[row][i] = dist.at(i);
			distMatrix[i][row] = dist.at(i);
		}
		else
			distMatrix[row][i] = 0;
	}

	// remove 'col' cluster from distance matrix
	distMatrix.erase(distMatrix.begin() + col);
	for(uint i = 0; i < distMatrix.size(); ++i)
	{
		distMatrix[i].erase(distMatrix[i].begin() + col);
	}
}

void HierarchicalClustering::NJ(const Matrix& _distMatrix, const std::vector<std::string>& labels, Tree* tree)
{
	// keep original distance matrix unmodified
	Matrix distMatrix = _distMatrix;

	NeighbourJoining nj;
	nj.buildTree(distMatrix, labels, tree);
}
