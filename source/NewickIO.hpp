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

#include "Tree.hpp"
#include "Node.hpp"

class NewickIO
{
public:		
	NewickIO() {}

	bool read(Tree& tree, const std::string& filename);
	bool parseNewickString(Tree& tree, const std::string& newickStr);
	void write(Tree& tree, const std::string & filename, bool overwrite = true) const;

private:
	void parseNodeInfo(Node* node, std::string& nodeInfo);

	void write(Tree& tree, std::ostream & out) const;
	void writeElements(Tree& tree, std::ostream& out, Node* parent, Node* child) const;
	void writeNodes(Tree& tree, std::ostream& out, Node* parent) const;
};