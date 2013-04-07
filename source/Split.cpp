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

Split::Split(float weight, uint frequency, const std::vector<bool>& split)
	: m_weight(weight), m_frequency(frequency), m_split(split)
{

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

bool Split::isTrivial() const
{
	return true;
}