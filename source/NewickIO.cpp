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

#include "NewickIO.hpp"

bool NewickIO::read(Tree& tree, const std::string& filename)
{
	// parse Newick file
	std::string newickFile = filename;
	std::replace(newickFile.begin(), newickFile.end(), '\\', '/');

	std::ifstream file(newickFile.c_str());
	if(!file.is_open())
	{
		std::cout << "[Error] Unable to open Newick file: " << filename << std::endl;
		return false;
	}

	// concatenate lines in file until we reach the end of the file
	std::string line, newickStr;
	while(file.good())
	{
		getline(file, line);
		newickStr += line;
	}

	file.close();

	// parse tree and build tree structure
	return parseNewickString(tree, newickStr);
}

void NewickIO::write(Tree& tree, const std::string & filename, bool overwrite) const
{
	std::ofstream output(filename.c_str(), overwrite ? (std::ios::out) : (std::ios::out|std::ios::app));
	write(tree, output); 
	output.close();
}

void NewickIO::parseNodeInfo(Node* node, std::string& nodeInfo)
{	
	std::string length;
	std::string name;

	// check if this element has length
	int colon = nodeInfo.rfind(':');
	if(colon != std::string::npos)
	{
		length = nodeInfo.substr(colon + 1);
		nodeInfo = nodeInfo.substr(0, colon);
	}

	// check for name and/or support value
	int lastP = nodeInfo.rfind('\'');
	int firstP = nodeInfo.rfind('\'');
	if(firstP != std::string::npos)
	{
		name = nodeInfo.substr(firstP+1, lastP-firstP-1);
	}
	else
	{
		int spacePos = nodeInfo.find(' ');
		if(spacePos != std::string::npos)
		{
			// parse the name, we do not need the support value
			name = nodeInfo.substr(0, spacePos-1);	
		}
		else
		{
			// assume the remaining description is either a name
			name = nodeInfo;
		}
	}	

	if(!name.empty())
		node->name(name);

	if(!length.empty())
		node->distanceToParent((double)::atof(length.c_str()));
}

bool NewickIO::parseNewickString(Tree& tree, const std::string& newickStr)
{
	// create root node
	Node* root = new Node("root");
	tree.root(root);

	int lastP  = newickStr.rfind(')');
	int firstP = newickStr.find('(');
	int semi = newickStr.rfind(';');

	std::string content = newickStr.substr(firstP + 1, lastP - firstP);
	std::string rootElements = newickStr.substr(lastP + 1, semi - lastP - 1);
  
	parseNodeInfo(root, rootElements);

	// parse newick string
	std::stack<Node*> nodeStack;
	nodeStack.push(root);
	std::string nodeInfo;
	Node* activeNode = NULL;	
	bool bInQuotes = false;
	uint numInternalNodes = 1;
	uint numLeafNodes = 0;

	for(uint i = 0; i < content.size(); ++i)
	{
		char ch = content[i];

		if(ch == '(')
		{
			// create a new internal node which will be the child 
			// of the node on the top of the stack
			Node* node(new Node());
			numInternalNodes++;
			nodeStack.top()->addChild(node);
			nodeStack.push(node);
		}
		else if(ch == ')' || ch == ',')
		{
			if(activeNode)
			{
				// if there is a currently active node, then we are
				// processing an internal node
				parseNodeInfo(activeNode, nodeInfo);				
			}
			else
			{
				// if there is no currently active node, then we
				// must create a new leaf node
				Node* node(new Node());
				numLeafNodes++;
				nodeStack.top()->addChild(node);

				parseNodeInfo(node, nodeInfo);		
			}

			activeNode = NULL;
			nodeInfo = "";

			// we are finished processing all children of the node on the top of the stack
			if(ch == ')')
			{
				activeNode = nodeStack.top();
				nodeStack.pop();
			}
		}
		else
		{
			// character is indicate the properties of a node
			if(ch == '"')
				bInQuotes = !bInQuotes;

			if(bInQuotes || !::isspace(ch))
				nodeInfo += ch;
		}
	}

	tree.numNodes(numInternalNodes + numLeafNodes);
	tree.numLeaves(numLeafNodes);

	return true;
}

void NewickIO::write(Tree& tree, std::ostream& out) const
{
	out << "(";

	Node* root = tree.root();
	
	if(root->children().size() == 0)
	{
		out << "'" << root->name().c_str() << "'";

		double dist = root->distanceToParent();
		if(dist != Node::NO_DISTANCE)
			out << " " << dist;
	}
	else
	{
		writeNodes(tree, out, root);
	}
	out << ")";
	
	if(!root->name().empty())
		out << "'" << root->name().c_str() << "'";
	
	out << ";" << std::endl;
}

void NewickIO::writeNodes(Tree&tree, std::ostream& out, Node* parent) const
{
	writeElements(tree, out, parent, parent->child(0));
	for(uint i = 1; i < parent->numberOfChildren(); i++)
	{
		out << ",";
		writeElements(tree, out, parent, parent->child(i));
	}
}

void NewickIO::writeElements(Tree& tree, std::ostream& out, Node* parent, Node* child) const
{	
	if(child->numberOfChildren() != 0)
	{
	out << "(";
	writeNodes(tree, out, child); 
	out << ")";
	}

	if(!child->name().empty())
		out << "'" << child->name().c_str() << "'";
	
	if(child->distanceToParent() != Node::NO_DISTANCE)
		out << ":" << child->distanceToParent();
}
