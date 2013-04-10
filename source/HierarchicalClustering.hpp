//=======================================================================
// Author: Donovan Parks
//
// Copyright 2013 Donovan Parks (donovan.parks@gmail.com)
//
// This file is part of PETs.
//
// PETs is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// PETs is distributed in the hope that it will be useful, bu
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with PETs. If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#pragma once

#include "Precompiled.hpp"

#include "Tree.hpp"
#include "Node.hpp"

class HierarchicalClustering
{
public:
		/** Type of clustering to perform. */
	enum CLUSTER_TYPE { COMPLETE_LINKAGE, SINGLE_LINKAGE, AVERAGE_LINKAGE, NEIGHBOUR_JOINING };

public:
	/** Constructor. */
	HierarchicalClustering() {}

	/** Destructor. */
	~HierarchicalClustering() {}

	/** 
	 * @brief Cluster distance matrix.
	 * @param clusterType Type of clustering to perform.
	 * @param distMatrix Matrix indicating pairwise distance between objects.
	 * @param labels Labels identifying each row/col of the distance matrix.
	 * @param tree Resulting hierarchical tree.
	 */
	static void clustering(CLUSTER_TYPE clusterType, const Matrix& distMatrix, const Strings& labels, Tree* tree);


	/** 
	 * @brief Complete linkage clustering (aka, farthest neighbour clustering).
	 * @param distMatrix Matrix indicating pairwise distance between objects.
	 * @param labels Labels identifying each row/col of the distance matrix.
	 * @param tree Resulting hierarchical tree.
	 */
	static void completeLinkage(const Matrix& distMatrix, const Strings& labels, Tree* tree)
	{
		clustering(COMPLETE_LINKAGE, distMatrix, labels, tree);
	}

	/** 
	 * @brief Single linkage clustering (aka, nearest neighbour clustering).
	 * @param distMatrix Matrix indicating pairwise distance between objects.
	 * @param labels Labels identifying each row/col of the distance matrix.
	 * @param tree Resulting hierarchical tree.
	 */
	static void singleLinkage(const Matrix& distMatrix, const Strings& labels, Tree* tree)
	{
		clustering(SINGLE_LINKAGE, distMatrix, labels, tree);
	}

	/**
	 * @brief Unweighted Pair Group Method with Arithmetic (UPGMA) mean clustering (aka, average linkage clustering).
	 * @param distMatrix Matrix indicating pairwise distance between objects.
	 * @param labels Labels identifying each row/col of the distance matrix.
	 * @param tree Resulting hierarchical tree.
	 */
	static void UPGMA(const Matrix& distMatrix, const Strings& labels, Tree* tree)
	{
		clustering(AVERAGE_LINKAGE, distMatrix, labels, tree);
	}

	/** 
	 * @brief Build neighbour joining (NJ) tree from a distance matrix.
	 * @param distMatrix Matrix indicating pairwise distance between objects.
	 * @param labels Labels identifying each row/col of the distance matrix.
	 * @param tree Resulting NJ tree.
	 */
	static void NJ(const Matrix& distMatrix, const Strings& labels, Tree* tree);

protected:
	static void findNearestClusters(Matrix& distMatrix, uint& row, uint& col);

	static double distanceToNode(Node* node);

	static void updateClusters(std::vector<Node*>& clusters, uint row, uint col, double value);

	static void updateDistanceMatrix(CLUSTER_TYPE clusterType, Matrix& distMatrix, std::vector<Node*>& clusters, uint row, uint col);
};