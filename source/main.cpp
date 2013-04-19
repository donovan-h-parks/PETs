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
#include "StringUtils.hpp"

#include "Tools.hpp"
#include "Kmedoid.hpp"
#include "PCoA.hpp"

int main(int argc, char* argv[])
{
	srand((uint)time(NULL));

	PETs pets;

	// read input file specifying forest of trees for each gene
	//std::string inputFile("../../unit-tests/arPETs9595.trees.subsampled.txt"); //argv[1]
	//std::string inputFile("../../unit-tests/compatibility-test.txt");
	std::string inputFile("../../data/ar/arPETs9595.trees.txt");

	std::ifstream fin(inputFile.c_str());
	if(!fin.is_open())
	{
		std::cout << "[Error] Failed to open input file: " << inputFile << std::endl;
	}

	std::string geneName;
	std::vector<Tree*> trees;
	while(!fin.eof())
	{
		std::string line;
		getline(fin, line);

		if(line.empty())
			continue;

		if(line[0] == '%')
		{
			if(!geneName.empty())
				pets.addGene(geneName, trees);

			geneName = StringUtils::removeWhiteSpaces(line.substr(1));
			trees.clear();
		}
		else
		{
			Tree* tree = new Tree();
			NewickIO newickIO;
			newickIO.parseNewickString(*tree, line);

			tree->name(geneName + "-" + StringUtils::toString(trees.size()));
			trees.push_back(tree);
		}
	}
	pets.addGene(geneName, trees);

	fin.close();

	//pets.compatibilityClusteringByPercentage(0.85, "../../data/ar/ar-0.85", "../../data/ar/ar-0.85.clustering.txt");

	for(uint i = 10; i < 40; ++i)
	{
		std::cout << i << std::endl;
		std::string iStr = StringUtils::toString(i);
		pets.compatibilityClusteringByFixedNumber(i, "../../data/ar/ar-" + iStr, "../../data/ar/ar-" + iStr + ".maxClique.txt");
	}

	//pets.conclustador("../../unit-tests/conclustador-results-by-inspection.txt");

	//std::ofstream fout("../../unit-tests/euclidean-result.txt");
	//pets.print(fout);
	//fout.close();

	return 0;
}

