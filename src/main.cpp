#include "agent.h"
#include <vector>

int main(int argc, char* argv[])
{
	pthread_attr_t attr;
	int num_threads, cache, size, i;
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
	for (i = 0; i < num_threads-1; i++)
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
		std::cout << "q function of agent" + std::to_string(i) << '\n';
		std::cout << (*agents_list[i].getQ()) << '\n';
		std::cout << '\n';
		std::cout << '\n';
	}
	std::cout << "global_q" << '\n';
	std::cout << (*agents_list[0].getGlobalQ()) << '\n';
	std::cout << '\n';
	std::cout << '\n';
}
