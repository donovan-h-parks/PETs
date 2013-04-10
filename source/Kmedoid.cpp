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

#include "Kmedoid.hpp"

std::vector<uint> Kmedoid::cluster(const Matrix& distMatrix, const std::vector<std::string>& labels, uint numClusters, uint numIterations)
{
	m_labels = labels;

	m_medoids.resize(numClusters);
	m_clusterId.resize(distMatrix.size());

	double minCost = std::numeric_limits<double>::max();
	for(uint i = 0; i < numIterations; ++i)
	{
		// pick k unique random medoids
		std::set<uint> randMedoids;
		while(randMedoids.size() != m_medoids.size())
			randMedoids.insert(rand() % distMatrix.size());

		std::copy(randMedoids.begin(), randMedoids.end(), m_medoids.begin());

		// swap mediods
		bool bConverged = false;
		double cost;
		while(!bConverged)
			bConverged = swapMedoids(distMatrix, cost);

		if(cost < minCost)
		{
			minCost = cost;
			m_bestClustering = m_clusterId;
		}
	}

	return m_bestClustering;
}

double Kmedoid::determineClosestMedoids(const Matrix& distMatrix)
{
	double cost = 0;
	for(uint i = 0; i < distMatrix.size(); ++i)
	{
		double minDist = std::numeric_limits<double>::max();
		uint clusterId = std::numeric_limits<uint>::max();
		for(uint j = 0; j < m_medoids.size(); ++j)
		{
			if(distMatrix.at(i).at(m_medoids.at(j)) < minDist)
			{
				minDist = distMatrix.at(i).at(m_medoids.at(j));
				clusterId = j;
			}
		}

		m_clusterId.at(i) = clusterId;
		cost += minDist;
	}

	return cost;
}

bool Kmedoid::swapMedoids(const Matrix& distMatrix, double& cost)
{
	cost = determineClosestMedoids(distMatrix);

	std::set<uint> mediodIndices(m_medoids.begin(), m_medoids.end());

	std::vector<uint> bestMedoids = m_medoids;
	std::vector<uint> bestClusterId = m_clusterId;
	bool bConverged = true;
	for(uint i = 0; i < m_medoids.size(); ++i)
	{
		uint curMediod = m_medoids.at(i);
		for(uint j = 0; j < distMatrix.size(); ++j)
		{
			if(mediodIndices.find(j) == mediodIndices.end())
			{
				// make point the medoid
				m_medoids.at(i) = j;

				double curCost = determineClosestMedoids(distMatrix);
				if(curCost < cost)
				{
					cost = curCost;
					bestMedoids = m_medoids;
					bestClusterId = m_clusterId;
					bConverged = false;
				}
			}
		}

		m_medoids.at(i) = curMediod;
	}

	m_medoids = bestMedoids;
	m_clusterId = bestClusterId;

	return bConverged;
}

bool Kmedoid::print(const std::string& filename)
{
	std::ofstream fout(filename.c_str());
	if(!fout.is_open())
	{
		std::cout << "[Error] Failed to output clustering file: " << filename << std::endl;
		return false;
	}

	for(uint i = 0; i < m_medoids.size(); ++i)
	{
		fout << "%Cluster " << (i+1) << std::endl;
		for(uint j = 0; j < m_bestClustering.size(); ++j)
		{
			if(m_bestClustering.at(j) == i)
				fout << m_labels.at(j) << std::endl;
		}
		fout << std::endl;
	}

	return true;
}