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

#include "Node.hpp"

class Tree
{
public:	
	Tree(): m_root(NULL), m_name("")  {}
	Tree(Node* root): m_root(root), m_name("") {}
	~Tree();

	Tree(const Tree& tree);
	Tree* clone() const { return new Tree(*this); }

	std::string name() const  { return m_name; }
	void name(const std::string& name) { m_name = name; }

	Node* root() const { return m_root; }
	void root(Node* root);

	std::vector<Node*> leaves(Node* subtree) const;
	std::vector<Node*> nodes(Node* subtree) const { return breadthFirstOrder(subtree); }

	std::set<std::string> commonTaxa(const Tree* tree);

	uint numNodes() const { return m_numNodes; }
	void numNodes(uint numNodes) { m_numNodes = numNodes; }

	uint numLeaves() const { return m_numLeaves; }
	void numLeaves(uint numLeaves) { m_numLeaves = numLeaves; }

	std::vector<Node*> postOrder(Node* subtree) const;
	std::vector<Node*> breadthFirstOrder(Node* subtree) const;

	double patristicDistance(Node* node1, Node* node2);
	double distanceToRoot(Node* node);

	void project(const std::set<std::string>& leavesToRetain);

private:
	void destroySubtree(Node* node);

	Node* cloneSubtree(Node* node);

	void leaves(Node* node, std::vector<Node*>& leaves) const;

	void postOrder(Node* node, std::vector<Node*>& nodes) const;

private:
	Node* m_root;
	std::string m_name;		

	uint m_numNodes;
	uint m_numLeaves;
};