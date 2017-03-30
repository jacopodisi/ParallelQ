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
	int saveMat(std::shared_ptr<Eigen::MatrixXd> mat, std::string filename, std::string directoryname);

	/*
	open the matrix saved as rows+cols+data with rows and cols as uint, data as double
	from a .bin file specified.
	@param the matrix where to save, the name of the file and the direcotry where the
	       file is saved
	*/
	int readMat(std::shared_ptr<Eigen::MatrixXd>& mat, std::string filename, std::string directoryname);

} // Functions


#endif //FUNCTIONS