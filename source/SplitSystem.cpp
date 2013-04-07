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

void SplitSystem::addSplit(const Split& split) 
{ 
	std::set<Split>::iterator it = m_splits.find(split);
	if(it == m_splits.end())
		m_splits.insert(split); 
	else
	{
		// increase frequecy of split
		Split split = *it;
		split.frequency(split.frequency() + 1);

		m_splits.erase(it);
		m_splits.insert(split);
	}
}