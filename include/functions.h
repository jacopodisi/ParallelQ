#ifndef FUNCTIONS
#define FUNCTIONS

#include "types.h"

namespace Functions {

	/*
	check if a file exists
	*/
	bool fileExists(const std::string& filename);

	/*
	save the matrix rows+cols+data with rows and cols as uint, data as double
	in a .bin file specified.
	@param the matrix to save, the name of the file, the direcotry to which
	       save the file
	*/
	void save(std::shared_ptr<Eigen::MatrixXd> mat, std::string filename, std::string directoryname);

} // Functions


#endif //FUNCTIONS