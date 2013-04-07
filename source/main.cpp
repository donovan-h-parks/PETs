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

#include "PETs.hpp"
#include "Tree.hpp"
#include "NewickIO.hpp"

int main(int argc, char* argv[])
{
	PETs pets;

	// read input file specifying forest of trees for each gene
	std::ifstream fin(argv[1]);
	if(!fin.is_open())
	{
		std::cout << "[Error] Failed to open input file: " << argv[1] << std::endl;
	}

	std::string geneName;
	std::vector<Tree> trees;
	while(!fin.eof())
	{
		std::string line;
		getline(fin, line);

		if(line[0] == '%')
		{
			if(!geneName.empty())
				pets.addGene(geneName, trees);

			geneName = line.substr(1);
		}
		else
		{
			Tree tree;
			NewickIO newickIO;
			newickIO.parseNewickString(tree, line);

			trees.push_back(tree);
		}
	}
	pets.addGene(geneName, trees);

	fin.close();

	// build split systems from each forest of gene trees
	pets.buildSplitSystems();

	return 0;
}

