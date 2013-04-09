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

using namespace std;

class StringUtils
{
public:
	StringUtils() {}
	~StringUtils() {}

	static bool isWhiteSpaceCharacter(char c)
	{
		return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r') || (c == '\f');  
	}

	static string removeWhiteSpaces(const string & s)
	{
		string st (s);
		for (uint i = 0; i < st.size(); i++)
		{
			if(isWhiteSpaceCharacter(st[i]))
			{
				st.erase(st.begin() + i);
				i--;
			}
		}

		return st;
	}

	static string toString(int i)
	{
		ostringstream oss;
		oss << i;
		return oss.str();
	}

	static string toString(double d, int precision)
	{
		ostringstream oss;
		oss << setprecision(precision) << d;
		return oss.str();
	}

};