#include "gridhandler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <list>
#include <fstream>
#include <cstdio>


void gridcheck(GridMatrix_pointer grid, int init_row = 0, int init_col = 0)
{	
	while (true)
	{
		if ((*grid)(init_row, init_col).type == 0) break;
		init_row++;
	}

	position p;
	p.row = init_row;
	p.col = init_col;
	std::list<position> positions;
	positions.push_back(p);
	for (auto l: positions)
	{
		int row = l.row;
		int col = l.col;
		if (row < (grid->rows()-1) && (*grid)(row+1, col).mark == 0 &&
		 (*grid)(row+1, col).type != -1)
		{
			position pnew;
			(*grid)(row+1, col).mark = 1;
			(*grid)(row+1, col).check = true;
			pnew.row = row+1;
			pnew.col = col;
			positions.push_back(pnew);
		}
		if (row > 0 && (*grid)(row-1, col).mark == 0 && 
			(*grid)(row-1, col).type != -1)
		{
			position pnew;
			(*grid)(row-1, col).mark = 1;
			(*grid)(row-1, col).check = true;
			pnew.row = row-1;
			pnew.col = col;
			positions.push_back(pnew);
		}
		if (col < (grid->cols()-1) && (*grid)(row, col+1).mark == 0 &&
		 (*grid)(row, col+1).type != -1)
		{
			position pnew;
			(*grid)(row, col+1).mark = 1;
			(*grid)(row, col+1).check = true;
			pnew.row = row;
			pnew.col = col+1;
			positions.push_back(pnew);
		}
		if (col > 0 && (*grid)(row, col-1).mark == 0 && 
			(*grid)(row, col-1).type != -1)
		{
			position pnew;
			(*grid)(row, col-1).mark = 1;
			(*grid)(row, col-1).check = true;
			pnew.row = row;
			pnew.col = col-1;
			positions.push_back(pnew);
		}
	}
}

GridMatrix_pointer gridGenerator(int gridsize, int wallsize, bool save)
{
	GridMatrix_pointer grid;
	int cols = gridsize;
	int rows = gridsize;
	bool ok;
	srand(time(0));
	do
	{
		ok = true;
		grid = std::make_shared<GridMatrix>(gridsize,gridsize);
		std::cout << "griglia fatta " + std::to_string(grid->rows()) + std::to_string(grid->cols()) << '\n';
		for (int w = 0; w < wallsize; w++)
		{
			(*grid)(rand() % rows, rand() % cols).type = -1;
		}
		std::cout << "muri fatta " << '\n';
		(*grid)(0,cols-1).type = 1;
		(*grid)(rows-1,0).type = 1;
		std::cout << "goal fatta " << '\n';
		gridcheck(grid);
		std::cout << "fine grid check " << '\n';
		for (int c = 0; (c < cols) && ok; c++)
		{
			for (int r = 0; (r < rows) && ok; r++)
			{
				if (!(*grid)(r,c).check && ((*grid)(r,c).type!=-1)) ok = false;
			}
		}
		std::cout << "fine check " << '\n';
		if (ok) break;
		std::cout << "griglia fatta male... rifacciamo " << '\n';
    } while (true);
	std::string fn = "grid_matrix_list_size" + std::to_string(gridsize) + ".bin";
	if (save) saveGrid(grid);

	return grid;
}

void printGrid(GridMatrix_pointer grid)
{
	int cols = grid->cols();
	int rows = grid->rows();
	for (int row = 0; row < rows; row++)
	{
		if (row==0)
		{
			std::cout << std::string("-", cols) << '\n';  
		}
		std::cout << "|";
		for (int col = 0; col < cols; col++)
		{
			if ((*grid)(row,col).type == -1) std::cout << '#' ;
			else if ((*grid)(row,col).type == 1) std::cout << 'G' ;
			else if ((*grid)(row,col).type == 0) std::cout << ' ' ;
		}
		std::cout << "|";
		std::cout << '\n';
		if (row==rows-1)
		{
			std::cout << std::string("-", cols) << '\n';  
		}
	}
}

void printGrid(std::shared_ptr<MatrixXi> grid, int agent_row, int agent_col)
{
	int cols = grid->cols();
	int rows = grid->rows();
	for (int row = 0; row < rows; row++)
	{
		if (row == 0)
		{
			std::cout << '\n';  
		}
		std::cout << "|";
		for (int col = 0; col < cols; col++)
		{
			if ( agent_row == row && agent_col == col) std::cout << '@';
			else if ((*grid)(row,col) == -1) std::cout << '#' ;
			else if ((*grid)(row,col) == 1) std::cout << 'G' ;
			else if ((*grid)(row,col) == 0) std::cout << ' ' ;
		}
		std::cout << "|";
		std::cout << '\n';
		if (row==rows-1)
		{
			std::cout << '\n';  
		}
	}
}

int listSize(int size)
{
	std::string fn = "grid_matrix_list_size" + std::to_string(size) + ".bin";
	FILE *fs = fopen(fn.c_str(), "ab");
	if(!fs) std::perror("File opening failed");
	if(fseek(fs, 0, SEEK_END)!=0) std::perror("File seek failed");
	return ftell(fs)/(sizeof(int) * size * size);
}

void saveGrid(GridMatrix_pointer grid)
{
	int size = grid->cols();
	std::string fn = "grid_matrix_list_size" + std::to_string(size) + ".bin";
	int output[size];
	FILE *fs = fopen(fn.c_str(), "ab");
	if(!fs) std::perror("File opening failed");
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			output[j] = (*grid)(i,j).type;
		}
		std::fwrite(output, sizeof(int), size, fs);
		if(std::fflush(fs) != 0)
		{
			std::cout << "Error in flushing file" << '\n';
			return;
		}
	}
	if(std::fclose(fs) != 0) std::cout << "Error in closing file" << '\n';
}

std::shared_ptr<MatrixXi> openGrid(int size, int id)
{
	std::shared_ptr<MatrixXi> grid = std::make_shared<MatrixXi>(size,size);
	std::string fn = "grid_matrix_list_size" + std::to_string(size) + ".bin";
	FILE *fs = fopen(fn.c_str(), "rb");
	if(!fs)
	{
		std::cout << "File opening failed: ";
		std::perror(fn.c_str());
	}
	if(fseek(fs, sizeof(int) * size * size * id, SEEK_SET)!=0) std::perror("File seek failed");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; ++j)
		{
			fread(&(*grid)(i,j), sizeof(int), 1, fs);
		}
	}

	if(std::fclose(fs) != 0) std::cout << "Error in closing file" << '\n';
	return grid;
}