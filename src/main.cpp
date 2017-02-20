#include "agent.h"

int main(int argc, char* argv[])
{
	srand(time(0));
	if (argc < 2) {std::cin >> argv[1];}
	Environment env (14, atof(argv[1]));
	env.printGridEnv();
	std::shared_ptr<Eigen::VectorXd> vin = env.readValueFunction();
	std::cout << '\n';
	std::cout << '\n';
	std::cout << (*vin) << '\n';
	Agent agent(env);
	agent.learn(-1, 100);
	std::cout << '\n';
	std::cout << '\n';
	std::cout << (*agent.getQ()) << '\n';
	std::string qfile = "default";
	agent.saveQ(qfile);
	std::shared_ptr<Eigen::MatrixXd> qin = agent.readQ(qfile);
	std::cout << '\n';
	std::cout << '\n';
	std::cout << (*qin) << '\n';
}