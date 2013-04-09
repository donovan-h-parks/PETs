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

#include "PETs.hpp"
#include "Conclustador.hpp"

void PETs::addGene(const std::string& geneName, const std::vector<Tree*>& trees)
{
	SplitSystem splitSystem(geneName);

	for(uint i = 0; i < trees.size(); ++i)
		splitSystem.addTree(trees.at(i));

	m_geneSplitSystems.push_back(splitSystem);
}

void PETs::print(std::ofstream& fout) const
{
	for(uint i = 0; i < m_geneSplitSystems.size(); ++i)
	{
		m_geneSplitSystems.at(i).print(fout);
		fout << std::endl;
	}
}

void PETs::conclustador(const std::string& file)
{
	Conclustador c;
	c.calculateDist(m_geneSplitSystems);
	c.printMatrix(file);


}