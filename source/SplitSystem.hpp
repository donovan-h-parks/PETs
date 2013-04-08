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

#include "Split.hpp"
#include "Tree.hpp"
#include "Node.hpp"

class SplitSystem
{
public:
	SplitSystem() {}
	SplitSystem(const std::string& name): m_name(name) {}

	std::string name() const { return m_name; }
	void name(const std::string& name) { m_name = name; }

	uint numUniqueSplits() const { return m_uniqueSplits.size(); }
	uint numTrees() const { return m_treeSplits.size(); }
	std::set<Split> uniqueSplits() const { return m_uniqueSplits; }

	void addTree(const Tree* const tree);
	std::vector<const Tree* const> trees() const { return m_trees; }

	std::map<std::string, uint> taxaIdMap() const { return m_taxaIdMap; }

	std::set<std::string> commonTaxa(const SplitSystem& splitSystem) const;

	bool isCompatible();
	void createTree(Tree& tree);

	//void project(const std::set<std::string> taxa);

	void print(std::ofstream& fout) const;

private:
	void addSplit(const Split& split);

private:
	std::string m_name;

	std::vector<const Tree* const> m_trees;
	std::vector<std::set<Split> > m_treeSplits;
	std::set<Split> m_uniqueSplits;

	typedef std::map<std::string, uint> TaxaIdMap;
	TaxaIdMap m_taxaIdMap;
};
