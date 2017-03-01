#include "agent.h"
#include <vector>

int main(int argc, char* argv[])
{
	std::cout << "1 ";
	int num_threads;
	std::cout << "1 ";
	pthread_mutex_t m;
	pthread_attr_t attr;
	std::shared_ptr<Eigen::MatrixXd> global_q1;
	std::shared_ptr<Eigen::MatrixXd> global_q2;
	std::cout << "1 ";
	int cache, size, i;
	void * return_value;
	std::cout << "1 ";
	srand(time(0));
	if (argc < 2) { std::cout << "choose env: "; std::cin >> argv[1];}
	Environment env (14, atof(argv[1]));
	global_q1 = std::make_shared<Eigen::MatrixXd>(env.getStatesList()->rows(), env.getNumActions());
	global_q1->setZero();
	std::cout << "1 ";
	global_q2 = std::make_shared<Eigen::MatrixXd>(env.getStatesList()->rows(), env.getNumActions());
	global_q2->setZero();
	num_threads = atof(argv[2]);
	size = env.getStatesList()->rows() / num_threads;
	pthread_mutex_init(&m,NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	std::cout << "1 ";
	switch (num_threads)
	{
		case 2: cache = 8;
	}
	std::vector<Agent> agents_list;
	std::cout << "1 ";
	for (i = 0; i < num_threads-1; i++)
	{
		Agent a(env, i*size, i*size+size, cache, m);
		agents_list[i] = (a);
	}
	std::cout << "1 ";
	if (num_threads == 1 || num_threads == 0)
	{
		Agent a(env);
		agents_list[i] = (a);
	} else 
	{
		Agent a(env, i*size, env.getStatesList()->rows() - 1, cache, m);
		agents_list[i] = (a);
	}
	pthread_t threads[num_threads];
	std::cout << "1 ";
	for (i = 0; i < num_threads; i++) {pthread_create(&threads[i], &attr, Agent::learn, (void *) &agents_list[i]);}
	for (i = 0; i < num_threads; i++) {pthread_join(threads[i], &return_value);}
	for (i = 0; i < num_threads; i++)
	{
		std::cout << "agent num " + std::to_string(i) << '\n';
		std::cout << (*agents_list[i].getQ()) << '\n';
		std::cout << '\n';
		std::cout << '\n';
	}
	std::cout << "global_q" << '\n';
	std::cout << (*agents_list[0].getGlobalQ()) << '\n';
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