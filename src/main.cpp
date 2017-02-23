#include "agent.h"
#include "pthread.h"

int main(int argc, char* argv[])
{
	/*
	srand(time(0));
	if (argc < 2) {std::cin >> argv[1];}
	Environment env (14, atof(argv[1]));
	Agent agent(env);
	agent.learn();
	std::string qfile = "default";
	agent.saveQ(qfile);
	*/

	/*Number of threads*/
	unsigned int num_threads = atoi(argv[2]);
	
}