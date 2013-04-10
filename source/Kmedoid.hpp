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

class Kmedoid
{
public:
	Kmedoid() {}
	~Kmedoid() {}

	std::vector<uint> cluster(const Matrix& distMatrix, const Strings& labels, uint numClusters, uint numIterations);

	bool print(const std::string& filename);

private:
	double determineClosestMedoids(const Matrix& distMatrix);
	bool swapMedoids(const Matrix& distMatrix, double& cost);

private:
	std::vector<uint> m_medoids;
	std::vector<uint> m_clusterId;

	std::vector<uint> m_bestClustering;

	Strings m_labels;
};
