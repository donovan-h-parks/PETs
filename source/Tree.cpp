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

#include "Tree.hpp"

Tree::~Tree()
{
	if(m_root)
	{
		destroySubtree(m_root);

		if(m_root)
		{
			delete m_root;
			m_root = NULL;
		}
	}
}
	
void Tree::destroySubtree(Node* node)
{
	for(uint i = 0; i < node->numberOfChildren(); i++)
	{
		Node* child = node->child(i);
		destroySubtree(child);

		if(child)
		{
			delete child;
			child = NULL;
		}
	}
}

void Tree::root(Node* root)
{
	m_root = root;
	m_root->distanceToParent(Node::NO_DISTANCE);
	m_root->parent(NULL);
}

std::vector<Node*> Tree::leaves(Node* subtree) const
{
	std::vector<Node*> leafList;
	leaves(subtree, leafList);
	return leafList;
}

void Tree::leaves(Node* node, std::vector<Node*>& leafList) const
{
	if(node->isLeaf())
		leafList.push_back(node);
	else
	{
		for(uint i = 0; i < node->numberOfChildren(); i++)
			leaves(node->child(i), leafList);
	}
}

std::vector<Node*> Tree::postOrder(Node* subtree) const
{
	std::vector<Node*> nodes;
	postOrder(subtree, nodes);

	return nodes;
}

void Tree::postOrder(Node* node, std::vector<Node*>& nodes) const
{
	for(uint i = 0; i < node->numberOfChildren(); i++)
		postOrder(node->child(i), nodes);

	nodes.push_back(node);
}

std::vector<Node*> Tree::breadthFirstOrder(Node* subtree) const
{
	std::vector<Node*> nodes;

	std::queue<Node*> q;
	q.push(subtree);
	while(!q.empty())
	{
		Node* curNode = q.front();
		q.pop();

		nodes.push_back(curNode);
		std::vector<Node*> children = curNode->children();
		for(uint i = 0; i < children.size(); ++i)
			q.push(children[i]);
	}

	return nodes;
}

float Tree::patristicDistance(Node* node1, Node* node2)
{
	std::vector<Node*> path;
	std::vector<Node*> pathMatrix1;
	std::vector<Node*> pathMatrix2;

	// path from node 1 to the root
	Node* nodeUp = node1;
	while(!nodeUp->isRoot())  
	{ 
		pathMatrix1.push_back(nodeUp);
		nodeUp = nodeUp->parent();
	}
	pathMatrix1.push_back(nodeUp); // The root.

	// path from node 2 to the root
	nodeUp = node2;
	while(!nodeUp->isRoot())  
	{
		pathMatrix2.push_back(nodeUp);
		nodeUp = nodeUp->parent();
	}
	pathMatrix2.push_back(nodeUp); // The root.

	// find most terminal node where paths diverge
	int tmp1 = pathMatrix1.size() - 1;
	int tmp2 = pathMatrix2.size() - 1;

	while((tmp1 >= 0) && (tmp2 >= 0)) 
	{
		if (pathMatrix1[tmp1] != pathMatrix2[tmp2]) 
			break;
		tmp1--; tmp2--;
	}

	// get distance from node 1 to node 2
	float dist = 0;
	for (int i = 0; i <= tmp1; ++i)
		dist += pathMatrix1[i]->distanceToParent();

	for (int i = tmp2; i >= 0; --i) 
		dist += pathMatrix2[i]->distanceToParent();

	return dist;
}

double Tree::distanceToRoot(Node* node)
{
	double dist = 0;
	Node* curNode = node;
	while(!curNode->isRoot())
	{
		dist += curNode->distanceToParent();
		curNode = curNode->parent();
	}

	return dist;
}