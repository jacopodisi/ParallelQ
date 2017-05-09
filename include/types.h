/***************************************************************************
 *   Copyright (C) 2017 by Jacopo Di Simone                                *
 *                                                                         *
 *   This file is part of ParallelQ.                                      *
 *                                                                         *
 *   ParallelQ is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   ParallelQ is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with ParallelQ; if not, see <http://www.gnu.org/licenses/>    *
 ***************************************************************************/

#ifndef TYPES
#define TYPES

#include <cstdint>
#include <iostream>
#include <sys/stat.h>
#include <Eigen/Dense>
#include <memory>

#define debugDynamic false
#define debugAgent false

struct position {
	int row;
	int col;
};

struct observation {
	int next_state;
	double reward;
	bool done;
};

enum Actions : uint8_t
{
  up = 0, // 0
  down = 1, // 1
  left = 2, // 2
  right = 3 // 3
};

struct Q_value
{
	int value = 0;
	int cache = 0;
};

#endif //TYPES