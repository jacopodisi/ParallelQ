#ifndef FUNCTIONS
#define FUNCTIONS

#include "types.h"

namespace Functions {

	/*
	check if a file exists
	*/
	bool fileExists(const std::string& filename);
	void save(std::shared_ptr<Eigen::MatrixXd> mat, std::string filename, std::string directoryname);

} // Functions


#endif //FUNCTIONS