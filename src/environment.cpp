#include "environment.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <list>

void gridcheck(GridMatrix_pointer grid, int init_row = 0, int init_col = 0)
{	
	while (true)
	{
		if ((*grid)(init_row, init_col).type == 0) break;
		init_row++;
	}

	pos p;
	p.row = init_row;
	p.col = init_col;
	std::list<pos> positions;
	positions.push_back(p);
	for (auto l: positions)
	{
		int row = l.row;
		int col = l.col;
		if (row < (grid->rows()-1) && (*grid)(row+1, col).mark == 0 &&
		 (*grid)(row+1, col).type != -1)
		{
			pos pnew;
			(*grid)(row+1, col).mark = 1;
			(*grid)(row+1, col).check = true;
			pnew.row = row+1;
			pnew.col = col;
			positions.push_back(pnew);
		}
		if (row > 0 && (*grid)(row-1, col).mark == 0 && 
			(*grid)(row-1, col).type != -1)
		{
			pos pnew;
			(*grid)(row-1, col).mark = 1;
			(*grid)(row-1, col).check = true;
			pnew.row = row-1;
			pnew.col = col;
			positions.push_back(pnew);
		}
		if (col < (grid->cols()-1) && (*grid)(row, col+1).mark == 0 &&
		 (*grid)(row, col+1).type != -1)
		{
			pos pnew;
			(*grid)(row, col+1).mark = 1;
			(*grid)(row, col+1).check = true;
			pnew.row = row;
			pnew.col = col+1;
			positions.push_back(pnew);
		}
		if (col > 0 && (*grid)(row, col-1).mark == 0 && 
			(*grid)(row, col-1).type != -1)
		{
			pos pnew;
			(*grid)(row, col-1).mark = 1;
			(*grid)(row, col-1).check = true;
			pnew.row = row;
			pnew.col = col-1;
			positions.push_back(pnew);
		}
	}
}

GridMatrix_pointer gridGenerator(int gridsize, int wallsize)
{
	GridMatrix_pointer grid;
	int cols = gridsize;
	int rows = gridsize;
	bool ok;
	do
	{
		ok = true;
		grid = std::make_shared<GridMatrix>(gridsize,gridsize);
		std::cout << "griglia fatta " + std::to_string(grid->rows()) + std::to_string(grid->cols()) << '\n';
		srand(time(0));
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
		printGrid(grid);
    	std::this_thread::sleep_for(std::chrono::seconds(1));	
	} while (true);

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
			else if ((*grid)(row,col).type == 0 && !(*grid)(row,col).check) std::cout << '@' ;
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