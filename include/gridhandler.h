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

#ifndef GRIDHANDLER
#define GRIDHANDLER

#include "types.h"
#include <thread>
#include <chrono>
#include <list>
#include <fstream>

using namespace Eigen;

/*
structures used in the creation of the grid
*/
struct gridstruct {
		int type = 0;
		bool check = false;
		int mark = 0;
};
typedef Eigen::Matrix<gridstruct, Eigen::Dynamic, Eigen::Dynamic> GridMatrix;
typedef std::shared_ptr<GridMatrix> GridMatrix_pointer;

/*
function to check that the created grid is correct:
every couple of cells that are not a wall are reachable. 
*/
void gridcheck(GridMatrix_pointer grid, int row, int col);

// +1 for goal
// -1 for wall
// 0 nothing
GridMatrix_pointer gridGenerator(int gridsize, int wallsize, bool save = false);

/*
function used to print out the grid passed by pointer
and the size of the list of grids given the size of the grid
*/
void printGrid(GridMatrix_pointer grid);
void printGrid(std::shared_ptr<MatrixXi> grid, int agent_row = -1, int agent_col = -1);
int listSize(int size);

/*
function used to save the grid in a file referenced to the
size of the grid.
*/
void saveGrid(GridMatrix_pointer grid, int id);

/*
function used to open a grid given the id of the grid and its size
returning a pointer to the given grid
*/
std::shared_ptr<MatrixXi> openGrid(int size, int id);

#endif //GRIDHANDLER