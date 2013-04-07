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

// Generally only 3rd party header files should be included in this file. That is, header files that will
// almost never be changed. If a project header is added to this file, the entire project will need to be
// recompiled whenever this file changes. There are cases where this is reasonable (i.e., header files 
// specifying common typedef's or constants that must be included in almost every file anyways).

// C/C++ includes
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <float.h>	// defines DBL_MAX

// Project files (add with caution!)
#include "DataTypes.hpp"
#include "Common.hpp"


