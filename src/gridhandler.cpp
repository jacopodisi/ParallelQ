#include "gridhandler.h"
#include "functions.h"


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
	do
	{
		ok = true;
		int row, col;
		grid = std::make_shared<GridMatrix>(gridsize,gridsize);
		(*grid)(0,cols-1).type = 1;
		(*grid)(rows-1,0).type = 1;
		for (int w = 0; w < wallsize; w++)
		{
			row = rand() % rows;
			col = rand() % cols;
			if ((*grid)(row, col).type == 0) (*grid)(row, col).type = -1;
			else w--;
		}
		gridcheck(grid);
		for (int c = 0; (c < cols) && ok; c++)
		{
			for (int r = 0; (r < rows) && ok; r++)
			{
				if (!(*grid)(r,c).check && ((*grid)(r,c).type!=-1)) ok = false;
			}
		}
		if (ok) break;
    } while (true);
    int id = 0;
	if (save)
	{
		while(true)
	    {
	    	std::string fn = "grid/grid_matrix_size" + std::to_string(gridsize) + "id" + std::to_string(id) + ".bin";
	    	if(!Functions::fileExists(fn)) break;
	    	id++;
	    }
		saveGrid(grid, id);
	}

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
			std::cout << '\n';  
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
			std::cout << '\n';  
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

void saveGrid(GridMatrix_pointer grid, int id)
{
	std::string dir = "grid";
	std::string fn = "grid_matrix_size" + std::to_string(grid->cols()) + "id" + std::to_string(id);
	std::shared_ptr<Eigen::MatrixXd> mat = std::make_shared<Eigen::MatrixXd>(grid->rows(), grid->cols());
	for(int i = 0; i < mat->rows(); i++)
	{
		for (int j = 0; j < mat->cols(); j++)
		{
			(*mat)(i,j) = (double) (*grid)(i,j).type;
		}
	}
	Functions::saveMat(mat, fn, dir);
}

std::shared_ptr<MatrixXi> openGrid(int size, int id)
{
	std::shared_ptr<MatrixXi> grid = std::make_shared<MatrixXi>();
	std::shared_ptr<MatrixXd> grid_double = std::make_shared<MatrixXd>();
	std::string dir = "grid";
	std::string file = "grid_matrix_size" + std::to_string(size) + "id" + std::to_string(id);
	if (!Functions::readMat(grid_double, file, dir)) exit(0);
	(*grid) = (*grid_double).cast<int>();
	return grid;
}