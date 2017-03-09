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
	global_q1 = std::make_shared<Eigen::MatrixXd>(env.getStatesList()->rows(), env.getNumActions());
	global_q1->setZero();
	global_q2 = std::make_shared<Eigen::MatrixXd>(env.getStatesList()->rows(), env.getNumActions());
	global_q2->setZero();
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
	std::cout << "agent" << '\n';
	std::cout << (*agent1.getGlobalQ()) << '\n';
	std::cout << '\n';
	std::cout << '\n';

	/*
	std::string qfile = "default";
	
	std::cout << "global" << '\n';
	std::cout << (*ret) << '\n';
	std::cout << '\n';
	std::cout << '\n';
	
	//declarations
	
	std::list<Parameters> params;
	int space_size;
	std::shared_ptr<Eigen::MatrixXd> global_q;
	Parameters par;
	void * return_value;

	srand(time(0));
	while ((argc < 2) || atof(argv[1]) < 0 || atof(argv[1]) > 4) { std::cout << "choose env (between 0 and 4): "; std::cin >> argv[1];}
	Environment env (14, atof(argv[1]));

	pthread_mutex_init(&m, NULL);
	pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	num_threads = atof(argv[2]);
	while((num_threads<0 || num_threads>8) && (num_threads%2!=0))
	{ 
		std::cout << "choose num threads (even and between 0 and 4): ";
		std::cin >> num_threads;
	}
	space_size = env.getPositionsList()->size() / num_threads;
	global_q = std::make_shared<Eigen::MatrixXd>(env.getStatesList()->rows(), env.getNumActions());

	pthread_t threads[num_threads];
	int cache = 0;
	if (num_threads == 2) cache = 8;
	else if (num_threads == 4) cache = 4;
	else if (num_threads == 8) cache = 2;

	for(int i = 0; i < num_threads; i++)
	{
		par.env = env;
		par.init = i*space_size;
		par.end = par.init + space_size;
		par.cache_size = cache;
		par.global_q = global_q;
		par.mutex = m;
		if (i == (num_threads-1)) par.end = env.getStatesList()->rows();
		params.push_back(par);
	}

	std::list<Parameters>::iterator it;
	int index = 0;
	for(it = params.begin(); it != params.end(); ++it, index++)
	{
		pthread_create(&threads[index], &attr, learn, (void *) &it);
	}
	for (int i = 0; i < num_threads; ++i)
	{
		pthread_join(threads[i], &return_value);
	}

	std::cout << (*global_q) << '\n';
	*/
}
