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

#include "Split.hpp"

Split::Split(double weight, uint frequency, const BoolVec& split)
	: m_weight(weight), m_frequency(frequency), m_split(split)
{
	canonicalFormat();
}

Split::Split(const Split& other)
	: m_weight(other.weight()), m_frequency(other.frequency()), m_split(other.split())
{

}

Split& Split::operator=(Split const& other)
{
	if((void*)this == (void*)&other)
		return *this;

	m_weight = other.weight();
	m_frequency = other.frequency();
	m_split = other.split();

	return *this;
}

bool Split::operator<(const Split& other) const
{
	return m_split < other.split();
}

void Split::canonicalFormat()
{
	// make sure split is in the canonical format
	// (first taxa present should be encoded to be on the left of the split)
	bool bReverseSplit = (m_split.at(0) == RIGHT_TAXA);

	if(bReverseSplit)
	{
		for(uint i = 0; i < m_split.size(); ++i)
		{
			if(m_split.at(i) == RIGHT_TAXA)
				m_split.at(i) = LEFT_TAXA;
			else
				m_split.at(i) = RIGHT_TAXA;
		}
	}
}

bool Split::isTrivial() const
{
	uint leftTaxa = 0;
	uint rightTaxa = 0;
	for(uint i = 0; i < m_split.size(); ++i)
	{
		if(m_split.at(i) == LEFT_TAXA)
			leftTaxa++;

		if(m_split.at(i) == RIGHT_TAXA)
			rightTaxa++;

		if(leftTaxa > 1 && rightTaxa > 1)
			return false;
	}

	return true;
}

void Split::print(std::ofstream& fout) const
{
	for(uint i = 0; i < m_split.size(); ++i)
	{
		if(m_split.at(i) == RIGHT_TAXA)
			fout << "0";
		else if(m_split.at(i) == LEFT_TAXA)
			fout << "1";
	}

	fout << "\t" << m_weight << "\t" << m_frequency << std::endl;
}