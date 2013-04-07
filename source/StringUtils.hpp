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

class StringUtils
{
public:
	StringUtils() {}
	~StringUtils() {}

	/**
	 * @brief Write map to file. 
	 * @param outputFile File to write map to.
	 * @param map Map to write to file.
	 */
	static void WriteMap(const std::string& filename, const QHash<std::string, uint>& map)
	{
		QFile file(filename);
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			Log::Inst().Warning("Unable to create file: " + filename);
			return;
		}

		QTextStream textStream(&file);
		textStream.setRealNumberPrecision(6);

		QHashIterator<std::string, uint> iter(map);
		while(iter.hasNext())
		{
			iter.next();
			textStream << iter.key() << ": " << iter.value() << endl;
		}

		file.close();
	}

	/**
	 * @brief Write matrix to file. 
	 * @param outputFile File to write matrix to.
	 * @param matrix Matrix to write to file.
	 * @param entryNames Name of rows/columns.
	 */
	static void WriteMatrix(const std::string& filename, const Matrix& matrix, const std::stringList& entryNames)
	{
		QFile file(filename);
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			Log::Inst().Warning("Unable to create file: " + filename);
			return;
		}

		QTextStream textStream(&file);
		textStream.setRealNumberPrecision(6);

		// write out distance matrix between all communities (taking care to ignore the "outgroup" community)
		textStream << matrix.size() << endl;

		for(int i = 0; i < matrix.size(); ++i)
		{
			std::string entryName = entryNames.at(i);
			entryName = entryName.leftJustified(10, ' ', true);
			textStream << entryName << " ";
			for(int j = 0; j < matrix.at(i).size(); ++j)
			{
				textStream << matrix.at(i).at(j) << '\t';
			}

			textStream << endl;
		}

		file.close();
	}
};
