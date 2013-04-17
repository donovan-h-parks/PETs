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

class Tools
{
public:
	static bool printMatrix(const std::string& filename, const Matrix& matrix, const Strings& labels);
	static bool readMatrix(const std::string& file, Matrix& matrix, Strings& labels);

	static bool printMatrix(const std::string& filename, const AdjacencyMatrix& matrix, const Strings& labels);
	static bool readMatrix(const std::string& file, AdjacencyMatrix& matrix, Strings& labels);
};
