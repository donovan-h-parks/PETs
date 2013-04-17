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

#include "Tools.hpp"
#include "StringUtils.hpp"

bool Tools::readMatrix(const std::string& file, Matrix& matrix, Strings& labels)
{
	matrix.clear();
	labels.clear();

	std::ifstream fin(file);
	if(!fin.is_open())
	{
		std::cout << "[Error] Failed to open matrix file: " << file << std::endl;
		return false;
	}

	while(!fin.eof())
	{
		std::string line;
		getline(fin, line);

		if(line.empty())
			continue;

		Strings tokens = StringUtils::split(line, '\t');
		labels.push_back(tokens.at(0));

		std::vector<double> row;
		for(uint i = 1; i < tokens.size(); ++i)
			row.push_back(atof(tokens.at(i).c_str()));

		matrix.push_back(row);
	}

	return true;
}

bool Tools::printMatrix(const std::string& filename, const Matrix& matrix, const Strings& labels)
{

	std::ofstream fout(filename.c_str());
	if(!fout.is_open())
	{
		std::cout << "[Error] Failed to output matrix file: " << filename << std::endl;
		return false;
	}

	for(uint r = 0; r < matrix.size(); ++r)
	{
		fout << labels.at(r);

		for(uint c = 0; c < matrix.size(); ++c)
			fout << '\t' << matrix.at(r).at(c);
		fout << std::endl;
	}

	fout.close();

	return true;
}

bool Tools::readMatrix(const std::string& file, AdjacencyMatrix& matrix, Strings& labels)
{
	matrix.clear();
	labels.clear();

	std::ifstream fin(file);
	if(!fin.is_open())
	{
		std::cout << "[Error] Failed to open matrix file: " << file << std::endl;
		return false;
	}

	while(!fin.eof())
	{
		std::string line;
		getline(fin, line);

		if(line.empty())
			continue;

		Strings tokens = StringUtils::split(line, '\t');
		labels.push_back(tokens.at(0));

		std::vector<uint> row;
		for(uint i = 1; i < tokens.size(); ++i)
			row.push_back(atoi(tokens.at(i).c_str()) > 0);

		matrix.push_back(row);
	}

	return true;
}

bool Tools::printMatrix(const std::string& filename, const AdjacencyMatrix& matrix, const Strings& labels)
{
	std::ofstream fout(filename.c_str());
	if(!fout.is_open())
	{
		std::cout << "[Error] Failed to output matrix file: " << filename << std::endl;
		return false;
	}

	for(uint r = 0; r < matrix.size(); ++r)
	{
		fout << labels.at(r);

		for(uint c = 0; c < matrix.size(); ++c)
			fout << '\t' << matrix.at(r).at(c);
		fout << std::endl;
	}

	fout.close();

	return true;
}