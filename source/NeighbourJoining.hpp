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

class NeighbourJoining
{
 public:
	/** Constructor. */
	NeighbourJoining() {}

	/** Destructor. */
	~NeighbourJoining();

	/** 
	 * @brief Build neighbour joining (NJ) tree from a distance matrix.
	 * @param distMatrix Matrix indicating pairwise distance between objects.
	 * @param labels Labels identifying each row/col of the distance matrix.
	 * @param tree Resulting NJ tree.
	 */
	void buildTree(Matrix& distMatrix, const Strings& labels, Tree* tree);
  
private:
	double* m_separationSums;
	double* m_separations;	
	int m_rowIndex, m_colIndex;

	int m_numActiveClusters;
	bool* m_activeClusters;

	void findNearestClusters(Matrix& distMatrix);
	void updateClusters(Matrix& distMatrix, std::vector<Node*>& clusters);
	void updateDistanceMatrix(Matrix& distMatrix);
};