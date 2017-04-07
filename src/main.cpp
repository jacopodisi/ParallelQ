#include "agent.h"
#include <vector>
#include <getopt.h>
#include <sstream>
#include "functions.h"

int main(int argc, char* argv[])
{
	srand(time(0));
	bool print_out = false;
	bool save_time = false;
	bool save_stat = false;
	bool save_ep_val_func = false;
	int num_threads, cache, size, i, id;
	cache = 0;
	std::chrono::steady_clock::time_point start, end;
	void * return_value;
	if (argc < 2) { std::cout << "choose env or -1 to create a new grid: "; std::cin >> argv[1];}
	id = atof(argv[1]);
	if (id<0)
	{
		printGrid(gridGenerator(14, 60, true));
		return 1;
	}
	Environment env (14, id);
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
		{"no_shared_mem",	no_argument, 	    0, 's'	},
		{"print_out",		no_argument, 	    0, 'p'	},
		{"save_time",		no_argument, 	    0, 't'	},
		{"save_valep",		no_argument, 	    0, 'f'	},
		{"save_stat",		no_argument, 	    0, 'v'	}, //save valuefunction per episode
		{0,					0,					0,	0	},
	};
	int index;
	int iarg = 0;
	//turn off getopt error message
	opterr = 1; 
	while(iarg != -1)
	{
		iarg = getopt_long(argc, argv, "fsptve:n:d:a:m:", longopts, &index);
		switch (iarg)
		{
			case 'e':
				if(optarg)
					opt.eps = atof(optarg);
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
				opt.shared_mem = false;
				break;
			case 'p':
				print_out = true;
				break;
			case 't':
				save_time = true;
				break;
			case 'v':
				save_stat = true;
				break;
			case 'f':
				save_ep_val_func = true;
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
	std::vector<Agent> agents_list(num_threads, Agent(env, opt, save_stat));
	for (i = 0; i < num_threads-1; i++)
	{
		agents_list[i] = *(new Agent(env, i*size + 1, i*size+size, cache, opt, save_stat));
	}
	if (num_threads != 1)
	{
		agents_list[i] = *(new Agent(env, i*size + 1, env.getStatesList()->rows() - 1, cache, opt, save_stat));
	}
	pthread_attr_t attr;
	pthread_t threads[num_threads];
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	start = std::chrono::steady_clock::now();
	for (i = 0; i < num_threads; i++) {pthread_create(&threads[i], &attr, Agent::learn, (void *) &agents_list[i]);}
	for (i = 0; i < num_threads; i++) {pthread_join(threads[i], &return_value);}
	end = std::chrono::steady_clock::now();
	auto interval = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	if (print_out)
	{
		for (i = 0; i < num_threads; i++)
		{
			std::cout << "q function of agent" + std::to_string(i) << '\n';
			std::cout << (*agents_list[i].getQ()) << '\n'
					  << '\n'
					  << '\n';
		}
		std::cout << "global_q" << '\n'
				  << (*agents_list[0].getGlobalQ()) << "\n"
				  << '\n'
				  << '\n';
		std::cout << "duration: "
	              << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
	              << "."
	              << (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count())%1000000
	              << "s\n";
	}
    if (save_time)
    {
    	std::ofstream outfile;
	    std::string fn = "times/usid" + std::to_string(id) + "np" + std::to_string(num_threads) + ".txt";
		outfile.open(fn, std::ios_base::app);
		outfile << interval << '\n';
    }
    if (save_stat)
    {
    	agents_list[0].computeSaveStatistics("nt" + std::to_string(num_threads));
    }
    if (save_ep_val_func)
    {
    	agents_list[0].saveEpVF("nt" + std::to_string(num_threads));
    }
}