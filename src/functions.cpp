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

void Functions::save(std::shared_ptr<Eigen::MatrixXd> mat, std::string filename, std::string directoryname)
{	
	std::string dirname = "./" + directoryname;
	mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	std::string fn = dirname + "/" + filename + ".bin";
	//std::string fn = "epepqfunc/grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId()) + filename + ".bin";
	std::string choice;
	while (fileExists(fn)) 
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
	FILE *fs = fopen(fn.c_str(), "wb");
	if(!fs) std::perror("File opening failed");
	uint rows = mat->rows();
	uint cols = mat->cols();
	std::fwrite(&rows, sizeof(uint), 1, fs);
	std::fwrite(&cols, sizeof(uint), 1, fs);
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
		return;
	}
	if(std::fclose(fs) != 0) std::cout << "Error in closing file" << '\n';
}