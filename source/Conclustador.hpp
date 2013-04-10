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

class Conclustador
{
public:
	Conclustador() {}
	
	void calculateDist(const std::vector<SplitSystem*>& splitSystems);

	bool print(const std::string& filename);

	Matrix distMatrix() const { return m_dist; }
	Strings labels() const { return m_labels; }

private:
	double calculateDist(const SplitSystem* const ss1, const SplitSystem* const ss2);
	SplitSystem* project(const SplitSystem* const splitSystem, const std::set<std::string>& commonTaxa);

private:
	Strings m_labels;
	Matrix m_dist;
};
