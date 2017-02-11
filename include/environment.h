#include <Eigen/Dense>

using namespace Eigen;

struct gridstruct {
		int type = 0;
		bool check = false;
		int mark = 0;
};

struct pos {
		int row;
		int col;
};

typedef Eigen::Matrix<gridstruct, Eigen::Dynamic, Eigen::Dynamic> GridMatrix;
typedef std::shared_ptr<GridMatrix> GridMatrix_pointer;

void gridcheck(GridMatrix_pointer grid, int row, int col);

// +1 for goal
// -1 for wall
// 0 nothing
GridMatrix_pointer gridGenerator(int gridsize, int wallsize);

void printGrid(GridMatrix_pointer grid);