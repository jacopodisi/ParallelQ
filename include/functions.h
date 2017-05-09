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

#ifndef FUNCTIONS
#define FUNCTIONS

#include "types.h"

namespace Functions {

	/*
	check if a file exists
	*/
	bool fileExists(const std::string& filename);

	/*
	save the matrix as rows+cols+data with rows and cols as uint, data as double
	in a .bin file specified.
	@param the matrix to save, the name of the file, the direcotry to which
	       save the file
	*/
	int saveMat(std::shared_ptr<Eigen::MatrixXd> mat, std::string filename, std::string directoryname, bool incremental = false);

	/*
	open the matrix saved as rows+cols+data with rows and cols as uint, data as double
	from a .bin file specified.
	@param the matrix where to save, the name of the file and the direcotry where the
	       file is saved
	*/
	int readMat(std::shared_ptr<Eigen::MatrixXd>& mat, std::string filename, std::string directoryname);

} // Functions


#endif //FUNCTIONS