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

void PETs::addGene(const std::string& geneName, const std::vector<Tree*>& trees)
{
	m_geneTrees[geneName] = trees;
}

void PETs::identifyUniqueTaxa()
{
	// get set of taxa spanning all trees
  GeneTreeMap::iterator it;
	uint numUniqueTaxa = 0;
  for(it = m_geneTrees.begin(); it != m_geneTrees.end(); ++it)
	{
		std::vector<Tree*> trees = it->second;
    std::vector<Node*> leaves = trees.at(0)->root()->leaves();
		for(uint i = 0; i < leaves.size(); ++i)
		{
			std::string taxaId = leaves.at(i)->name();
			TaxaIdMap::iterator taxaIdIter = m_taxaIds.find(taxaId);
			if(taxaIdIter == m_taxaIds.end())
			{
				m_taxaIds[taxaId] = numUniqueTaxa;
				numUniqueTaxa++;
			}
		}
	}
}

void PETs::buildSplitSystems()
{
	// get set of taxa spanning all trees
	identifyUniqueTaxa();

	// build split system from forest of trees associated with each gene
	GeneTreeMap::iterator it;	
	for(it = m_geneTrees.begin(); it != m_geneTrees.end(); ++it)
	{
		std::string geneName = it->first;
		std::vector<Tree*> trees = it->second;

		SplitSystem splitSystem(geneName);
		
		// process each tree associates with this gene
		for(uint i = 0; i < trees.size(); ++i)
		{
			// check if tree is implicitly rooted
			bool bImplicitRoot = (trees.at(i)->root()->numberOfChildren() == 2);

			// create vector of missing taxa
			std::vector<byte> missingTaxa(m_taxaIds.size(), Split::MISSING_TAXA);
			std::vector<Node*> leaves = trees.at(i)->root()->leaves();
			for(uint j = 0; j < leaves.size(); ++j)
			{
				TaxaIdMap::iterator taxaIdIter = m_taxaIds.find(leaves.at(j)->name());
				missingTaxa.at(taxaIdIter->second) = Split::RIGHT_TAXA;
			}

			// build split for each node in the tree
			std::vector<Node*> nodes = trees.at(i)->root()->breadthFirstOrder();
			for(uint j = 0; j < nodes.size(); ++j)
			{
				Node* curNode = nodes.at(j);

				if(curNode->isRoot())
					continue;

				// if tree is implicitly rooted, only process the left child of
				// the implicit root in order to avoid double counting the split
				if(bImplicitRoot && curNode->parent()->isRoot())
				{
					if(curNode == curNode->parent()->children()[0])
						continue;
				}

				std::vector<byte> split = missingTaxa;	// initialize split with missing taxa and
																								// all others set to the right bipartition

				std::vector<Node*> leaves = curNode->leaves();
				for(uint k = 0; k < leaves.size(); ++k)
				{
					TaxaIdMap::iterator taxaIdIter = m_taxaIds.find(leaves.at(k)->name());
					split.at(taxaIdIter->second) = Split::LEFT_TAXA;
				}

				splitSystem.addSplit(Split(curNode->distanceToParent(), 1, split));
			}
		}

		m_geneSplitSystems[geneName] = splitSystem;
	}
}

void PETs::print(std::ofstream& fout) const
{
	fout << "Split systems: " << m_geneSplitSystems.size() << std::endl << std::endl;

	fout << "Taxa map: " << std::endl;
	TaxaIdMap::const_iterator taxaIdIt;	
	for(taxaIdIt = m_taxaIds.begin(); taxaIdIt != m_taxaIds.end(); ++taxaIdIt)
		fout << taxaIdIt->first << "\t" << taxaIdIt->second << std::endl;
	fout << std::endl;

	GeneSplitSystemMap::const_iterator it;	
	for(it = m_geneSplitSystems.begin(); it != m_geneSplitSystems.end(); ++it)
	{
		it->second.print(fout);
		fout << std::endl;
	}
}