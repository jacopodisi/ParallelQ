#include "agent.h"
#include <vector>
#include <getopt.h>
#include <sstream>

int main(int argc, char* argv[])
{
	int num_threads, cache, size, i;
	void * return_value;
	srand(time(0));
	if (argc < 2) { std::cout << "choose env: "; std::cin >> argv[1];}
	Environment env (14, atof(argv[1]));
	if (argv[2] == NULL) { std::cout << "choose num_threads: "; std::cin >> argv[2];}
	num_threads = atof(argv[2]);
	while(num_threads!=1 && num_threads!=2 && num_threads!=4 && num_threads!=8)
	{
	    std::cout << "wrong num_threads, choose another (1,2,4 or 8): ";
	    std::cin >> num_threads;
	}
	//HANDLE OPTIONS
	agent_options opt;
	const struct option longopts[] =
	{
		{"eps",				required_argument, 	0, 'e'	},
		{"num_episodes",	required_argument, 	0, 'n'	},
		{"discount",		required_argument, 	0, 'd'	},
		{"alpha",			required_argument, 	0, 'a'	},
		{"mse",				required_argument, 	0, 'm'	},
		{"shared_mem",		required_argument, 	0, 's'	},
		{0,					0,					0,	0	},
	};
	int index;
	int iarg = 0;
	//turn off getopt error message
	opterr = 1; 
	while(iarg != -1)
	{
		iarg = getopt_long(argc, argv, "s:e:n:d:a:m:", longopts, &index);
		switch (iarg)
		{
			case 'e':
				if(optarg)
					opt.eps = atof(optarg);
					std::cout << "Your argument(s): " << optarg << std::endl;
				break;
			case 'n':
				if(optarg)
					opt.num_ep = atof(optarg);
				break;
			case 'd':
				if(optarg)
					opt.discount = atof(optarg);
				break;
			case 'a':
				if(optarg)
					opt.alpha = atof(optarg);
				break;
			case 'm':
				if(optarg)
					opt.mse = atof(optarg);
				break;
			case 's':
				if(optarg)
				{
					std::string str(optarg);
					if (str[0] == 'F' || str[0] == 'f')
						opt.shared_mem = false;
				}
				break;
		}
	}
	//END OPTIONS
	size = env.getStatesList()->rows() / num_threads;
	switch (num_threads)
	{
		case 1: cache = 0; break;
		case 2: cache = 8; break;
		case 4: cache = 4; break;
		case 8: cache = 2; break;
	}
	//instantiation of different agents
	std::vector<Agent> agents_list(num_threads, Agent(env, opt));
	for (i = 0; i < num_threads-1; i++)
	{
		agents_list[i] = *(new Agent(env, i*size + 1, i*size+size, cache, opt));
	}
	if (num_threads != 1)
	{
		agents_list[i] = *(new Agent(env, i*size + 1, env.getStatesList()->rows() - 1, cache, opt));
	}
	pthread_attr_t attr;
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
