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

#include "functions.h"

bool Functions::fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

int Functions::saveMat(std::shared_ptr<Eigen::MatrixXd> mat, std::string filename, std::string directoryname, bool incremental)
{	
	std::string dirname = "./" + directoryname;
	mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	std::string fn = dirname + "/" + filename + ".bin";
	std::string choice;
	int i = 0;
	while (fileExists(fn)) 
	{
		if(incremental)
		{
			fn = dirname + "/" + filename + "_" + std::to_string(i) + ".bin";
			i++;
		} else
		{
			std::cout << "file " + fn + " already exist. Do you want to overwrite it?(y/n): ";
			std::cin >> choice;
			std::cout  << '\n';
			while(choice != "y" && choice != "n")
			{
				std::cout << "Invalid option (y/n): ";
				std::cin >> choice;
			}
			if (choice == "y") break;
			else std::cout << "specify a new file name: " + dirname + "/";
			std::cin >> filename;
			fn = dirname + "/" + filename + ".bin";
		}
	}
	FILE *fs = fopen(fn.c_str(), "wb");
	if(!fs)
	{
		std::perror("File opening failed");
		return 0;
	}
	int rows = mat->rows();
	int cols = mat->cols();
	std::fwrite(&rows, sizeof(int), 1, fs);
	std::fwrite(&cols, sizeof(int), 1, fs);
	for (int i = 0; i < mat->rows(); ++i)
	{
		for (int j = 0; j < mat->cols(); j++)
		{
			std::fwrite(&(*mat)(i,j), sizeof(double), 1, fs);
		}
	}
	if(std::fflush(fs) != 0)
	{
		std::cout << "Error in flushing file" << '\n';
		return 0;
	}
	if(std::fclose(fs) != 0)
	{
		std::cout << "Error in closing file" << '\n';
		return 0;
	}
	return 1;
}

int Functions::readMat(std::shared_ptr<Eigen::MatrixXd>& mat, std::string filename, std::string directoryname)
{
	std::string dirname = "./" + directoryname;
	std::string fn = dirname + "/" + filename + ".bin";
	if (!Functions::fileExists(fn)) 
	{
		std::cout << "file does not exist: " + fn << '\n';
		return 0;
	}
	FILE *fs = fopen(fn.c_str(), "rb");
	if(!fs)
	{
		std::cout << "File opening failed: ";
		return 0;
	}
	int rows, cols;
	std::fread(&rows, sizeof(int), 1, fs);
	std::fread(&cols, sizeof(int), 1, fs);
	mat = std::make_shared<Eigen::MatrixXd>(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; ++j)
		{
			std::fread(&(*mat)(i,j), sizeof(double), 1, fs);
		}
	}
	if(std::fclose(fs) != 0)
	{
		std::cout << "Error in closing file" << '\n';
		return 0;
	}
	return 1;
}