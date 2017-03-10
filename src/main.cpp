#include "agent.h"
#include <vector>

int main(int argc, char* argv[])
{
	pthread_attr_t attr;
	int num_threads, cache, size;
	int i = 0;
	void * return_value;
	srand(time(0));
	if (argc < 2) { std::cout << "choose env: "; std::cin >> argv[1];}
	Environment env (14, atof(argv[1]));
	if (argc < 3) { std::cout << "choose num_threads: "; std::cin >> argv[2];}
	num_threads = atof(argv[2]);
	while(num_threads!=1 && num_threads!=2 && num_threads!=4 && num_threads!=8)
	{
	    std::cout << "wrong num_threads, choose another (1,2,4 or 8): ";
	    std::cin >> num_threads;
	}
	size = env.getStatesList()->rows() / num_threads;
	switch (num_threads)
	{
		case 1: cache = 0; break;
		case 2: cache = 8; break;
		//todo cache other case
	}
	//creation of different agents
	std::vector<Agent> agents_list(num_threads, Agent(env));
	for (;i < num_threads-1; i++)
	{
		agents_list[i] = *(new Agent(env, i*size, i*size+size, cache));
	}
	if (num_threads != 1)
	{
		agents_list[i] = *(new Agent(env, i*size, env.getStatesList()->rows() - 1, cache));
	}
	pthread_t threads[num_threads];
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (i = 0; i < num_threads; i++) {pthread_create(&threads[i], &attr, Agent::learn, (void *) &agents_list[i]);}
	for (i = 0; i < num_threads; i++) {pthread_join(threads[i], &return_value);}
	for (i = 0; i < num_threads; i++)
	{
		std::cout << "agent" + std::to_string(i) << '\n';
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
