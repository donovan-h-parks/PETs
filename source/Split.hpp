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

/**
 * @class Split
 * @brief Holds data and functions for a single split.
 */
class Split
{
public:
	static const bool RIGHT_TAXA = false;
	static const bool LEFT_TAXA = true;	

public:
	Split();
	Split(const Split& other);
	Split(float weight, uint frequency, const std::vector<bool>& split);
	~Split() {}

	Split& operator=(Split const& other);
	bool operator< (const Split& other) const;

	float weight() const { return m_weight; }
	void weight(float weight) { m_weight = weight; }

	uint frequency() const { return m_frequency; }
	void frequency(uint freq) { m_frequency = freq; }

	uint splitSize() const { return m_split.size(); }
	bool taxaState(uint taxaIndex) const { return m_split.at(taxaIndex); }

	bool isTrivial() const;

	std::vector<bool> split() const { return m_split; }

	void print(std::ofstream& fout) const;

private:
	void canonicalFormat();

private:
	float m_weight;
	uint m_frequency;
	std::vector<bool> m_split;
};
