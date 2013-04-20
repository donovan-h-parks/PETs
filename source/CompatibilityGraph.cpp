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

#include "Precompiled.hpp"

#include "CompatibilityGraph.hpp"
#include "MCQD.hpp"

#include "Tools.hpp"

void CompatibilityGraph::buildByPercentage(const std::vector<SplitSystem*>& splitSystems, double bootstrapThreshold)
{
	// initialize matrices
	m_adjacencyMatrix.resize(splitSystems.size());
	m_sharedTaxa.resize(splitSystems.size());
	m_avgNonTrivialSupportedSplits.resize(splitSystems.size());
	for(uint i = 0; i < m_adjacencyMatrix.size(); ++i)
	{
		m_adjacencyMatrix.at(i).resize(splitSystems.size(), false);
		m_sharedTaxa.at(i).resize(splitSystems.size(), false);
		m_avgNonTrivialSupportedSplits.at(i).resize(splitSystems.size(), false);
	}

	// calculate distance between split systems
	for(uint i = 0; i < splitSystems.size(); ++i)
	{
		std::cout << "  " << splitSystems.at(i)->name() << std::endl;
		m_labels.push_back(splitSystems.at(i)->name());

		m_treeInfo[splitSystems.at(i)->name()] = TreeInfo(splitSystems.at(i)->numTaxa(), splitSystems.at(i)->supportedSplits(bootstrapThreshold));

		for(uint j = i+1; j < splitSystems.size(); ++j)
		{
			PairInfo pairInfo = isCompatible(splitSystems.at(i), splitSystems.at(j), bootstrapThreshold);
			
			m_adjacencyMatrix.at(i).at(j) = m_adjacencyMatrix.at(j).at(i) = pairInfo.compatibility;

			m_sharedTaxa.at(i).at(j) = m_sharedTaxa.at(j).at(i) = pairInfo.sharedTaxa;
			m_avgNonTrivialSupportedSplits.at(i).at(j) = m_avgNonTrivialSupportedSplits.at(j).at(i) = pairInfo.avgNonTrivialSupportedSplits;
		}
	}
}

void CompatibilityGraph::buildByFixedNumber(const std::vector<SplitSystem*>& splitSystems, uint fixedNumber)
{
	// initialize matrices
	m_adjacencyMatrix.resize(splitSystems.size());
	m_sharedTaxa.resize(splitSystems.size());
	m_avgNonTrivialSupportedSplits.resize(splitSystems.size());
	for(uint i = 0; i < m_adjacencyMatrix.size(); ++i)
	{
		m_adjacencyMatrix.at(i).resize(splitSystems.size(), false);
		m_sharedTaxa.at(i).resize(splitSystems.size(), false);
		m_avgNonTrivialSupportedSplits.at(i).resize(splitSystems.size(), false);
	}

	// calculate distance between split systems
	for(uint i = 0; i < splitSystems.size(); ++i)
	{
		std::cout << "  " << splitSystems.at(i)->name() << std::endl;
		m_labels.push_back(splitSystems.at(i)->name());

		m_treeInfo[splitSystems.at(i)->name()] = TreeInfo(splitSystems.at(i)->numTaxa(), int(splitSystems.at(i)->bootstrapAt(fixedNumber)*100 + 0.5));

		for(uint j = i+1; j < splitSystems.size(); ++j)
		{
			PairInfo pairInfo = isCompatible(splitSystems.at(i), splitSystems.at(j), fixedNumber);
			
			m_adjacencyMatrix.at(i).at(j) = m_adjacencyMatrix.at(j).at(i) = pairInfo.compatibility;

			m_sharedTaxa.at(i).at(j) = m_sharedTaxa.at(j).at(i) = pairInfo.sharedTaxa;
			m_avgNonTrivialSupportedSplits.at(i).at(j) = m_avgNonTrivialSupportedSplits.at(j).at(i) = pairInfo.avgNonTrivialSupportedSplits;
		}
	}
}

CompatibilityGraph::PairInfo CompatibilityGraph::isCompatible(const SplitSystem* const ss1, const SplitSystem* const ss2, double bootstrapThreshold)
{
	PairInfo pairInfo;
	pairInfo.compatibility = 0;

	// get taxa in common between split systems
	std::set<std::string> commonTaxa = ss1->commonTaxa(ss2);
	pairInfo.sharedTaxa = commonTaxa.size();

	// get projected splits in either split system
	std::vector<FreqPair> splits = ss1->projectedSplitFreq(ss2, commonTaxa);

	// determine splits in each system above bootstrap threshold
	std::vector<BoolVec> supportedSplits1;
	std::vector<BoolVec> supportedSplits2;
	for(uint i = 0; i < splits.size(); ++i)
	{
		if(splits.at(i).f1 > bootstrapThreshold)
			supportedSplits1.push_back(splits.at(i).split);

		if(splits.at(i).f2 > bootstrapThreshold)
			supportedSplits2.push_back(splits.at(i).split);
	}

	pairInfo.avgNonTrivialSupportedSplits = ((supportedSplits1.size() - commonTaxa.size()) + (supportedSplits2.size() - commonTaxa.size())) / 2.0;

	if(commonTaxa.size() < 4 || supportedSplits1.size() <= commonTaxa.size() || supportedSplits2.size() <= commonTaxa.size())
	{
		// no non-trivial, well-supported splits
		return pairInfo;
	}

	// determine if split system are compatible
	for(uint i = 0; i < supportedSplits1.size(); ++i)
	{
		BoolVec split1 = supportedSplits1.at(i);

		for(uint j = 0; j < supportedSplits2.size(); ++j)
		{
			BoolVec split2 = supportedSplits2.at(j);

			if(!isCompatible(split1, split2))	
				return pairInfo;
		}
	}

	pairInfo.compatibility = 1;
	return pairInfo;
}

struct sortByF1
{
    inline bool operator() (const FreqPair& struct1, const FreqPair& struct2)
    {
        return (struct1.f1 > struct2.f1);
    }
};

struct sortByF2
{
    inline bool operator() (const FreqPair& struct1, const FreqPair& struct2)
    {
        return (struct1.f2 > struct2.f2);
    }
};

CompatibilityGraph::PairInfo CompatibilityGraph::isCompatible(const SplitSystem* const ss1, const SplitSystem* const ss2, uint fixedNumber)
{
	PairInfo pairInfo;
	pairInfo.compatibility = 0;
	pairInfo.avgNonTrivialSupportedSplits = -1;

	// get taxa in common between split systems
	std::set<std::string> commonTaxa = ss1->commonTaxa(ss2);
	pairInfo.sharedTaxa = commonTaxa.size();

	if(fixedNumber >= commonTaxa.size() - 3)
	{
		// ensure there is sufficient non-trivial splits to calculate statistic
		// [there are N-3 non-trivial splits in an unrooted tree with N taxa]
		return pairInfo;
	}

	// get projected splits in either split system
	std::vector<FreqPair> splits = ss1->projectedSplitFreq(ss2, commonTaxa);

	// get 'fixedNumber' of best supported splits
	std::vector<BoolVec> supportedSplits1;
	std::sort(splits.begin(), splits.end(), sortByF1());
	for(uint i = 0; i < fixedNumber + commonTaxa.size(); ++i)
		supportedSplits1.push_back(splits.at(i).split);

	std::vector<BoolVec> supportedSplits2;
	std::sort(splits.begin(), splits.end(), sortByF2());
	for(uint i = 0; i < fixedNumber + commonTaxa.size(); ++i)
		supportedSplits2.push_back(splits.at(i).split);

	// determine if split system are compatible
	for(uint i = 0; i < supportedSplits1.size(); ++i)
	{
		BoolVec split1 = supportedSplits1.at(i);

		for(uint j = 0; j < supportedSplits2.size(); ++j)
		{
			BoolVec split2 = supportedSplits2.at(j);

			if(!isCompatible(split1, split2))	
				return pairInfo;
		}
	}

	pairInfo.compatibility = 1;
	return pairInfo;
}

bool CompatibilityGraph::isCompatible(const BoolVec& split1, const BoolVec& split2)
{
	assert(split1.size() == split2.size());

	std::set<uint> A1;
	std::set<uint> A2;
	for(uint i = 0; i < split1.size(); ++i)
	{
		if(split1.at(i))
			A1.insert(i);
		else
			A2.insert(i);
	}

	if(A1.size() == 1 || A2.size() == 1)
		return true;	// trivial splits are always compatible

	std::set<uint> B1;
	std::set<uint> B2;
	for(uint i = 0; i < split2.size(); ++i)
	{
		if(split2.at(i))
			B1.insert(i);
		else
			B2.insert(i);
	}

	if(B1.size() == 1 || B2.size() == 1)
		return true;	// trivial splits are always compatible

	// check for compatibility
	std::set<uint> intersectA1B1;
	std::set_intersection(A1.begin(), A1.end(), B1.begin(), B1.end(), std::inserter(intersectA1B1, intersectA1B1.begin()));
	if(intersectA1B1.empty())
		return true;

	std::set<uint> intersectA1B2;
	std::set_intersection(A1.begin(), A1.end(), B2.begin(), B2.end(), std::inserter(intersectA1B2, intersectA1B2.begin()));
	if(intersectA1B2.empty())
		return true;

	std::set<uint> intersectA2B1;
	std::set_intersection(A2.begin(), A2.end(), B1.begin(), B1.end(), std::inserter(intersectA2B1, intersectA2B1.begin()));
	if(intersectA2B1.empty())
		return true;

	std::set<uint> intersectA2B2;
	std::set_intersection(A2.begin(), A2.end(), B2.begin(), B2.end(), std::inserter(intersectA2B2, intersectA2B2.begin()));
	if(intersectA2B2.empty())
		return true;

	return false;
}

void CompatibilityGraph::findMaxClique()
{
	m_clusters.clear();

	// find maximum cliques
	std::set<uint> clusteredTrees;
	uint clusterIndex = 0;
	do
	{
		// get index mapping
		std::vector<uint> indexMapping;
		for(uint i = 0; i < m_adjacencyMatrix.size(); ++i)
		{
			if(clusteredTrees.find(i) != clusteredTrees.end())
				continue;

			indexMapping.push_back(i);
		}

		// create adjacency matrix
		uint size = indexMapping.size();
		bool **conn = new bool*[size];
		for(uint i=0; i < size; i++) 
		{
			conn[i] = new bool[size];
			memset(conn[i], 0, size * sizeof(bool));
		}

		for(uint i = 0; i < size; ++i)
		{
			uint indexI = indexMapping.at(i);
			for(uint j = i+1; j < size; ++j)
			{
				uint indexJ = indexMapping.at(j);
				if(m_adjacencyMatrix.at(indexI).at(indexJ) > 0)
					conn[i][j] = conn[j][i] = true;
			}
		}

		// find maximum clique
		Maxclique m(conn, size);
		
		int *qmax;
		int qsize;
		m.mcqdyn(qmax, qsize);

		Strings treeLabels;
		for(int i = 0; i < qsize; ++i)
		{
			uint index = indexMapping.at(qmax[i]);
			treeLabels.push_back(m_labels.at(index));
			clusteredTrees.insert(index);
		}

		m_clusters[clusterIndex] = treeLabels;
		clusterIndex++;

		// delete adjacency matrix and maximum clique
		for (uint i=0; i < size; ++i)
			delete[] conn[i];
		delete[] conn;

		delete [] qmax;
	} while(clusteredTrees.size() != m_adjacencyMatrix.size());
}

bool CompatibilityGraph::printMatrices(const std::string& prefix)
{
	if(!Tools::printMatrix(prefix + ".compatibility.tsv", m_adjacencyMatrix, m_labels))
		return false;

	if(!Tools::printMatrix(prefix + ".shared.tsv", m_sharedTaxa, m_labels))
		return false;

	if(!Tools::printMatrix(prefix + ".splits.tsv", m_avgNonTrivialSupportedSplits, m_labels))
		return false;

	return true;
}

bool CompatibilityGraph::printClustering(const std::string& filename)
{
	std::ofstream fout(filename.c_str());
	if(!fout.is_open())
	{
		std::cout << "[Error] Failed to output clustering file: " << filename << std::endl;
		return false;
	}

	std::map<uint, Strings>::iterator it;
	for(it = m_clusters.begin(); it != m_clusters.end(); ++it)
	{
		Strings treeLabels = it->second;

		fout << "%Cluster " << (it->first + 1) << " (" << treeLabels.size() << " trees)" << std::endl;		
		std::sort(treeLabels.begin(), treeLabels.end());
		for(uint i = 0; i < treeLabels.size(); ++i)
		{
			TreeInfo treeInfo = m_treeInfo[treeLabels.at(i)];
			fout << treeLabels.at(i) << '\t' << treeInfo.numTaxa << '\t' << treeInfo.nonTrivialSupportedSplits << std::endl;
		}
		fout << std::endl;
	}

	return true;
}