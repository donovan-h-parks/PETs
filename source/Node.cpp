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

#include "Node.hpp"

const float Node::NO_DISTANCE = -1.0f;

Node::Node(const std::string& name)
	: m_name(name), m_parent(NULL), m_distanceToParent(NO_DISTANCE)
{

}

Node::Node(const Node &rhs)
{
	m_name = rhs.name();
	m_distanceToParent = rhs.distanceToParent();
	
	m_parent = NULL;
	m_children.clear();
}

Node& Node::operator=(const Node &rhs)
{
	if(this != &rhs)
	{
		m_name = rhs.name();
		m_distanceToParent = rhs.distanceToParent();
	
		m_parent = NULL;
		m_children.clear();
	}

  return *this;
}

std::vector<Node*> Node::leaves()
{
	std::vector<Node*> leafVector;
	leaves(this, leafVector);
	return leafVector;
}

void Node::leaves(Node* node, std::vector<Node*>& leafVector)
{
	if(node->isLeaf())
		leafVector.push_back(node);

	for(unsigned int i = 0; i < node->numberOfChildren(); i++)
		leaves(node->child(i), leafVector);
}

std::vector<Node*> Node::nodes()
{
	std::vector<Node*> nodes;

	std::queue<Node*> q;
	q.push(this);
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

void Node::addChild(Node* node)
{
	m_children.push_back(node);
	node->parent(this);
}