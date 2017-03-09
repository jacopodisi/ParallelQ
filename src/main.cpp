#include "agent.h"

int main(int argc, char* argv[])
{
	int num_threads;
	pthread_mutex_t m;
	pthread_attr_t attr;
	std::shared_ptr<Eigen::MatrixXd> global_q1;
	std::shared_ptr<Eigen::MatrixXd> global_q2;

	pthread_t t1;
	pthread_t t2;
	void * return_value1;
	void * return_value2;
	
	srand(time(0));
	if (argc < 2) { std::cout << "choose env: "; std::cin >> argv[1];}
	Environment env (14, atof(argv[1]));
	num_threads = 2;

	pthread_mutex_init(&m,NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	Agent agent1(env, 0, 66, 8, m);
	Agent agent2(env, 67, 132, 8, m);

	pthread_create(&t1, &attr, Agent::learn, (void *) &agent1);
	pthread_create(&t2, &attr, Agent::learn, (void *) &agent2);

	pthread_join(t1, &return_value1);
	pthread_join(t2, &return_value2);
	//Agent::learn((void *) &agent1);
	//Eigen::MatrixXd * ret;
	//ret = (Eigen::MatrixXd *) return_value2;
	//pthread_join(t2, &return_value2);
	std::cout << "first agent" << '\n';
	std::cout << (*agent1.getQ()) << '\n';
	std::cout << '\n';
	std::cout << '\n';
	std::cout << "second agent" << '\n';
	std::cout << (*agent2.getQ()) << '\n';
	std::cout << '\n';
	std::cout << '\n';
	std::cout << "global agent" << '\n';
	std::cout << (*agent1.getGlobalQ()) << '\n';
	std::cout << '\n';
    std::cout << '\n';
}
