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

#include "SplitSystem.hpp"

class CompatibilityGraph
{
public:
	CompatibilityGraph() {}
	
	void build(const std::vector<SplitSystem*>& splitSystems, double bootstrapThreshold);
	void cluster();

	bool printMatrices(const std::string& prefix);
	bool printClustering(const std::string& filename);

	AdjacencyMatrix adjacencyMatrix() const { return m_adjacencyMatrix; }
	Strings labels() const { return m_labels; }

private:
	struct TreeInfo
	{
		TreeInfo(): numTaxa(0), nonTrivialSupportedSplits(0) {}
		TreeInfo(uint _numTaxa, uint _nonTrivialSupportedSplits): numTaxa(_numTaxa), nonTrivialSupportedSplits(_nonTrivialSupportedSplits) {}

		uint numTaxa;
		uint nonTrivialSupportedSplits;
	};

	struct PairInfo
	{
		PairInfo(): compatibility(0), sharedTaxa(0), avgNonTrivialSupportedSplits(0) {}
		PairInfo(uint _compatibility, uint _sharedTaxa, uint avgNonTrivialSupportedSplits): compatibility(_compatibility), sharedTaxa(_sharedTaxa), avgNonTrivialSupportedSplits(avgNonTrivialSupportedSplits) {}

		uint compatibility;
		uint sharedTaxa;
		double avgNonTrivialSupportedSplits;
	};

private:
	PairInfo isCompatible(const SplitSystem* const ss1, const SplitSystem* const ss2, double bootstrapThreshold);
	bool isCompatible(const BoolVec& split1, const BoolVec& split2);

private:
	Strings m_labels;
	AdjacencyMatrix m_adjacencyMatrix;
	AdjacencyMatrix m_sharedTaxa;
	Matrix m_avgNonTrivialSupportedSplits;

	std::map<uint, Strings> m_clusters;
	std::map<std::string, TreeInfo> m_treeInfo;
};
