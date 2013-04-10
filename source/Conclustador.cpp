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

#include "Conclustador.hpp"

void Conclustador::calculateDist(const std::vector<SplitSystem>& splitSystems)
{
	// initialize distance matrix
	m_dist.resize(splitSystems.size());
	for(uint i = 0; i < m_dist.size(); ++i)
		m_dist.at(i).resize(splitSystems.size(), 0.0f);

	// calculate distance between split systems
	for(uint i = 0; i < splitSystems.size(); ++i)
	{
		std::cout << "  " << splitSystems.at(i).name() << std::endl;
		m_labels.push_back(splitSystems.at(i).name());

		for(uint j = i+1; j < splitSystems.size(); ++j)
		{
			double dist = calculateDist(splitSystems.at(i), splitSystems.at(j));
			m_dist.at(i).at(j) = m_dist.at(j).at(i) = dist;
		}
	}
}

double Conclustador::calculateDist(const SplitSystem& ss1, const SplitSystem& ss2)
{
	// get taxa in common between split systems
	std::set<std::string> commonTaxa = ss1.commonTaxa(ss2);
	
	if(commonTaxa.size() < 4)
		return INF;

	// project split systems onto set of common taxa
	SplitSystem projSS1 = project(ss1, commonTaxa);
	SplitSystem projSS2 = project(ss2, commonTaxa);

	// calculate Euclidean distance between splits
	std::set<Split> splits1 = projSS1.uniqueSplits();
	std::set<Split> splits2 = projSS2.uniqueSplits();
	
	std::set<Split> allSplits;
	std::set_union(splits1.begin(), splits1.end(),
									splits2.begin(), splits2.end(),
									std::inserter(allSplits, allSplits.begin()));

	double dist = 0.0f;
	std::set<Split>::iterator allSplitsIt;
	for(allSplitsIt = allSplits.begin(); allSplitsIt != allSplits.end(); ++allSplitsIt)
	{
		std::set<Split>::iterator it1 = splits1.find(*allSplitsIt);
		double f1 = 0.0;
		if(it1 != splits1.end())
			f1 = double(it1->frequency()) / ss1.numTrees();

		std::set<Split>::iterator it2 = splits2.find(*allSplitsIt);
		double f2 = 0.0;
		double freq = it2->frequency();
		double trees = ss2.numTrees();
		if(it2 != splits2.end())
			f2 = double(it2->frequency()) / ss2.numTrees();

		dist += (f1-f2)*(f1-f2);
	}

	dist = sqrt(dist);

	// apply correction for number of taxa
	dist = dist / sqrt(2.0f*commonTaxa.size() - 6.0f);

	return dist;
}

SplitSystem Conclustador::project(const SplitSystem& splitSystem, const std::set<std::string>& commonTaxa)
{
	SplitSystem projSplitSystem;
	
	std::vector<const Tree* const> trees = splitSystem.trees();
	for(uint i = 0; i < trees.size(); ++i)
	{
		Tree* projTree = trees.at(i)->clone();
		projTree->project(commonTaxa);
		projSplitSystem.addTree(projTree);
	}

	return projSplitSystem;
}

bool Conclustador::printMatrix(const std::string& filename)
{
	std::ofstream fout(filename.c_str());
	if(!fout.is_open())
	{
		std::cout << "[Error] Failed to output matrix file: " << filename << std::endl;
		return false;
	}

	for(uint r = 0; r < m_dist.size(); ++r)
	{
		fout << m_labels.at(r);

		for(uint c = 0; c < m_dist.size(); ++c)
			fout << '\t' << m_dist.at(r).at(c);
		fout << std::endl;
	}

	fout.close();

	return true;
}