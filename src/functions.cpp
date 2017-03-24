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

int Functions::saveMat(std::shared_ptr<Eigen::MatrixXd> mat, std::string filename, std::string directoryname)
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
	if(!fs)
	{
		std::perror("File opening failed");
		return 0;
	}
	int rows = mat->rows();
	int cols = mat->cols();
	std::fwrite(&rows, sizeof(int), 1, fs);
	std::fwrite(&cols, sizeof(int), 1, fs);
	std::cout << std::to_string(rows) + " " + std::to_string(rows) + "\n";
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

int Functions::readMat(std::shared_ptr<Eigen::MatrixXd> mat, std::string filename, std::string directoryname)
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
	rows = 14;
	cols = 14;
	std::cout << std::to_string(rows) + " " + std::to_string(rows) + "\n";
	std::shared_ptr<Eigen::MatrixXd> matrix = std::make_shared<Eigen::MatrixXd>(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; ++j)
		{
			std::fread(&(*matrix)(i,j), sizeof(double), 1, fs);
		}
	}
	if(std::fclose(fs) != 0)
	{
		std::cout << "Error in closing file" << '\n';
		return 0;
	}
	mat = matrix;
	return 1;
}