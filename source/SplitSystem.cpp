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

#include "SplitSystem.hpp"

#include "NewickIO.hpp"

SplitSystem::~SplitSystem()
{
	for(uint i = 0; i < m_trees.size(); ++i)
		delete m_trees.at(i);

	m_trees.clear();
}

bool SplitSystem::isCompatible()
{
	/*
	for(int i = 0; i < m_splits.size()-1; ++i)
	{
		QSet<uint> iLeft;
		foreach(uint id, m_splits.at(i).GetLeftSequenceIds())
			iLeft.insert(id);

		QSet<uint> iRight;
		foreach(uint id, m_splits.at(i).GetRightSequenceIds())
			iRight.insert(id);

		bool bCompatible = false;
		for(int j = i+1; j < m_splits.size(); ++j)
		{
			QSet<uint> jLeft;
			foreach(uint id, m_splits.at(j).GetLeftSequenceIds())
				jLeft.insert(id);

			QSet<uint> jRight;
			foreach(uint id, m_splits.at(j).GetRightSequenceIds())
				jRight.insert(id);

			QSet<uint> intersectionLeft = iLeft;
			if(intersectionLeft.intersect(jLeft).empty())
			{
				bCompatible = true;
				break;
			}

			intersectionLeft = iLeft;
			if(intersectionLeft.intersect(jRight).empty())
			{
				bCompatible = true;
				break;
			}

			QSet<uint> intersectionRight = iRight;
			if(intersectionRight.intersect(jLeft).empty())
			{
				bCompatible = true;
				break;
			}

			intersectionRight = iRight;
			if(intersectionRight.intersect(jRight).empty())
			{
				bCompatible = true;
				break;
			}
		}

		if(!bCompatible)
			return false;
	}
	*/

	return true;
}

/*
bool splitSizeLessThan(const Split& s1, const Split& s2)
{
	uint s1Size = s1.GetSize();
	uint s2Size = s2.GetSize();

	return s1Size < s2Size;
}
*/

void SplitSystem::createTree(Tree& tree)
{
	/*
	QList<Split> splits = m_splits;

	// sort splits according to size of each split
	qSort(splits.begin(), splits.end(), splitSizeLessThan);

	// find implicit root in tree (i.e., set of identical splits).
	bool bFound = false;
	for(int i = 0; i < splits.size() && !bFound; ++i)
	{
		Split iSplit = splits.at(i);
		for(int j = i+1; j < splits.size() && !bFound; ++j)
		{
			Split jSplit = splits.at(j);

			// the identical splits will actually be complements of each other as the left
			// and right bipartitions will be swapped
			QBitArray count = iSplit.GetSplitArray() ^ jSplit.GetSplitArray();
			if(count.count(false) == 0)
			{
				splits[i].SetWeight(splits.at(i).GetWeight() + splits.at(j).GetWeight());
				splits.removeAt(j);
				bFound = true;
			}
		}
	}

	// Create tree starting from leaf nodes and working towards root.
	QList<VisualNode*> topNodes;
	int nodeId = splits.at(0).GetSplitArray().size();
	for(int i = 0; i < splits.size(); ++i)
	{
		Split curSplit = splits.at(i);

		QVector<uint> ids = splits.at(i).GetSequencesIdsInSmallestBipartition();
		if(ids.size() == 1)
		{
			//process leaf node
			VisualNode* leaf = new VisualNode(ids.at(0));

			std::string seqName;
			GetSamples()->GetSequences()->GetSequenceName(ids.at(0), seqName);
			leaf->SetName(seqName);
			leaf->SetDistanceToParent(curSplit.GetWeight());
			topNodes.push_back(leaf);
		}
		else
		{
			QSet<uint> idSet;
			foreach(uint id, ids)
				idSet.insert(id);

			// process internal node
			VisualNode* node = new VisualNode(nodeId++);
			node->SetDistanceToParent(curSplit.GetWeight());
			node->SetName("Node" + std::string::number(nodeId));
			QStack<uint> nodesToRemove;
			for(int j = 0; j < topNodes.size(); ++j)
			{
				VisualNode* topNode = topNodes.at(j);

				QList<uint> childrenIds;
				if(topNode->IsLeaf())
					childrenIds.push_back(topNode->GetId());
				else
					childrenIds = TreeTools<VisualNode>::GetLeafIds(topNode);

				if(childrenIds.toSet().subtract(idSet).empty())
				{
					idSet -= childrenIds.toSet();
					nodesToRemove.push(j);
					node->AddChild(topNode);
				}
				
				if(idSet.empty())
					break;
			}

			while(!nodesToRemove.empty())
			{
				topNodes.removeAt(nodesToRemove.pop());
			}

			topNodes.push_back(node);
		}
	}

	VisualNode* root = new VisualNode(nodeId++);
	root->SetName("Root");
	tree.SetRootNode(root);

	VisualNode* internalNode = new VisualNode(nodeId++);
	internalNode->SetName("internalNode");
	for(int i = 0; i < topNodes.size()-1; ++i)
		internalNode->AddChild(topNodes.at(i));

	internalNode->SetDistanceToParent(topNodes.at(topNodes.size()-1)->GetDistanceToParent()*0.5);
	topNodes.at(topNodes.size()-1)->SetDistanceToParent(internalNode->GetDistanceToParent());

	root->AddChild(topNodes.at(topNodes.size()-1));
	root->AddChild(internalNode);
	*/
}

void SplitSystem::addTree(const Tree* const tree)
{
	// save tree
	m_trees.push_back(tree);

	// create taxa map for first tree
	if(m_taxaIdMap.size() == 0)
	{
		Strings leafNames = tree->root()->leafNames();
		std::sort(leafNames.begin(), leafNames.end());
		for(uint i = 0; i < leafNames.size(); ++i)
			m_taxaIdMap[leafNames.at(i)] = i;
	}

	// check if tree is implicitly rooted
	bool bImplicitRoot = (tree->root()->numberOfChildren() == 2);

	// build split for each node in the tree
	std::set<Split> geneTreeSplits;
	std::vector<Node*> nodes = tree->root()->breadthFirstOrder();
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

		BoolVec split(m_taxaIdMap.size(), Split::RIGHT_TAXA);	// initialize split all taxa in the right bipartition

		std::vector<Node*> leaves = curNode->leaves();
		for(uint k = 0; k < leaves.size(); ++k)
		{
			TaxaIdMap::iterator taxaIdIter = m_taxaIdMap.find(leaves.at(k)->name());
			split.at(taxaIdIter->second) = Split::LEFT_TAXA;
		}

		addSplit(Split(curNode->distanceToParent(), 1, split));
		geneTreeSplits.insert(Split(curNode->distanceToParent(), 1, split));
	}

	m_treeSplits.push_back(geneTreeSplits);
}

void SplitSystem::addSplit(const Split& split) 
{ 
	// add split to split system
	std::set<Split>::iterator it = m_uniqueSplits.find(split);
	if(it == m_uniqueSplits.end())
	{
		m_uniqueSplits.insert(split);
	}
	else
	{
		// increase frequecy of split
		Split newSplit = *it;
		m_uniqueSplits.erase(it);

		uint freq = newSplit.frequency();
		double avgWeight = (double(freq)/(freq+1))*newSplit.weight() + (1.0f/(freq+1))*split.weight();
		newSplit.weight(avgWeight);
		newSplit.frequency(freq + split.frequency());

		m_uniqueSplits.insert(newSplit);
	}
}

void SplitSystem::print(std::ofstream& fout) const
{
	fout << "Name: " << m_name << std::endl;
	fout << "Unique splits: " << m_uniqueSplits.size() << std::endl;
	fout << "Trees in split system: " << m_trees.size() << std::endl;
	fout << "Taxa map: " << std::endl;
	TaxaIdMap::const_iterator taxaIdIt;	
	for(taxaIdIt = m_taxaIdMap.begin(); taxaIdIt != m_taxaIdMap.end(); ++taxaIdIt)
		fout << taxaIdIt->first << "\t" << taxaIdIt->second << std::endl;
	fout << std::endl;

	fout << "Unique splits (split, weight, frequency): " << std::endl;
	std::set<Split>::const_iterator it;
	for(it = m_uniqueSplits.begin(); it != m_uniqueSplits.end(); ++it)
		it->print(fout);
}

std::set<std::string> SplitSystem::commonTaxa(const SplitSystem* const splitSystem) const
{
	std::set<std::string> taxa1;
	TaxaIdMap::const_iterator it;
	for(it = m_taxaIdMap.begin(); it != m_taxaIdMap.end(); ++it)
		taxa1.insert(it->first);

	std::map<std::string, uint> taxaIdMap = splitSystem->taxaIdMap();
	std::set<std::string> taxa2;
	for(it = taxaIdMap.begin(); it != taxaIdMap.end(); ++it)
		taxa2.insert(it->first);

	std::set<std::string> intersect;
	std::set_intersection(taxa1.begin(), taxa1.end(),
													taxa2.begin(), taxa2.end(),
													std::inserter(intersect, intersect.begin()));
	return intersect;
}

BoolVec SplitSystem::projectionMask(const std::set<std::string>& commonTaxa) const
{
	BoolVec projMask(m_taxaIdMap.size(), false);

	uint index = 0;
	TaxaIdMap::const_iterator taxaIt;
	for(taxaIt = m_taxaIdMap.begin(); taxaIt != m_taxaIdMap.end(); ++taxaIt)
	{
		if(commonTaxa.find(taxaIt->first) != commonTaxa.end())
			projMask.at(index) = true;

		index++;
	}

	return projMask;
}

bool SplitSystem::project(const Split& split, const BoolVec& projectionMask, BoolVec& projectedSplit) const
{
	BoolVec curSplit = split.split();

	uint leftCount = 0;
	uint rightCount = 0;
	for(uint i = 0; i < projectionMask.size(); ++i)
	{
		if(projectionMask.at(i))
		{
			projectedSplit.push_back(curSplit.at(i));

			if(curSplit.at(i) == Split::LEFT_TAXA)
				leftCount++;
			else
				rightCount++;
		}
	}

	if(leftCount == 0 || rightCount == 0)
	{
		// this is a degenerate split caused by the projection
		return false;
	}

	return true;
}

std::map<BoolVec, uint> SplitSystem::projectedSplitCount(const std::set<std::string>& commonTaxa) const
{
	BoolVec projMask = projectionMask(commonTaxa);
	std::map<BoolVec, uint> splitsFreq;
	for(uint i = 0; i < m_treeSplits.size(); ++i)
	{
		// determine projected splits in tree
		std::set<BoolVec > projTreeSplits;
		std::set<Split>::const_iterator it;
		for(it = m_treeSplits.at(i).begin(); it != m_treeSplits.at(i).end(); ++it)
		{
			BoolVec projSplit;
			bool bValidSplit = project(*it, projMask, projSplit);

			if(bValidSplit)
				projTreeSplits.insert(projSplit);
		}

		// add tree splits into map of all splits
		std::set<BoolVec >::iterator projTreeSplitIt;
		for(projTreeSplitIt = projTreeSplits.begin(); projTreeSplitIt != projTreeSplits.end(); ++projTreeSplitIt)
		{
			std::map<BoolVec, uint>::iterator itFind = splitsFreq.find(*projTreeSplitIt);
			if(itFind == splitsFreq.end())
				splitsFreq[*projTreeSplitIt] = 1;
			else
				itFind->second += 1;
		}
	}

	return splitsFreq;
}

std::vector<FreqPair> SplitSystem::projectedSplitFreq(const SplitSystem* const splitSystem, const std::set<std::string>& commonTaxa) const
{
	// get frequency of each projected split
	std::map<BoolVec, uint> splitsFreq1 = projectedSplitCount(commonTaxa);
	std::map<BoolVec, uint> splitsFreq2 = splitSystem->projectedSplitCount(commonTaxa);

	// get splits contained in either split system
	std::set<BoolVec> allSplits;
	std::map<BoolVec, uint>::iterator mapIt;
	for(mapIt = splitsFreq1.begin(); mapIt != splitsFreq1.end(); ++mapIt)
		allSplits.insert(mapIt->first);

	for(mapIt = splitsFreq2.begin(); mapIt != splitsFreq2.end(); ++mapIt)
		allSplits.insert(mapIt->first);

	// get frequency of each split across all splits in the two split systems
	std::vector<FreqPair> splitFreqs;
	std::set<BoolVec>::iterator splitIt;
	for(splitIt = allSplits.begin(); splitIt != allSplits.end(); ++splitIt)
	{
		BoolVec split = *splitIt;

		double f1 = 0;
		std::map<BoolVec, uint>::const_iterator it1 = splitsFreq1.find(split);
		if(it1 != splitsFreq1.end())
			f1 = float(it1->second) / numTrees();

		double f2 = 0;
		std::map<BoolVec, uint>::const_iterator it2 = splitsFreq2.find(split);
		if(it2 != splitsFreq2.end())
			f2 = float(it2->second) / splitSystem->numTrees();

		splitFreqs.push_back(FreqPair(split, f1, f2));
	}

	return splitFreqs;
}

uint SplitSystem::supportedSplits(double bootstrapThreshold) const
{
	uint count = 0;
	std::set<Split>::iterator it;
	for(it = m_uniqueSplits.begin(); it != m_uniqueSplits.end(); ++it)
	{
		if(double(it->frequency()) / numTrees() >= bootstrapThreshold)
			count++;
	}

	return (count - numTaxa()); // number of supported, non-trivial splits
}

double SplitSystem::bootstrapAt(uint number) const
{
	std::vector<double> bootstraps;
	std::set<Split>::iterator it;
	for(it = m_uniqueSplits.begin(); it != m_uniqueSplits.end(); ++it)
		bootstraps.push_back(double(it->frequency()) / numTrees());

	std::sort(bootstraps.begin(), bootstraps.end(), std::greater<double>());

	return bootstraps.at(number + numTaxa());	// return bootstrap support of the nth best supported, non-trivial split

}