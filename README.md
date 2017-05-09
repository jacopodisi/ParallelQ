# ParallelQ

## Description
PrallalelQ implement a parallel version of the Reinforcement Learning algortihm Q-Learning using pthread and a shared memory concept.  
The environment choose to test the algorithm is a grid, 14x14 with 60 wall and 2 goal. Each transition give a reward of 0, except when the agent reach one of the two goal. Here he will receive a reward of 100.  
The already created grid, with their optimal value function is saved in the folder grid.  
The file grid14List.rtf contains a list of the available grids.

## How to build it
From the terminal use:  
$ make

## Command Line Arguments ##
Example:  ./myprog.out <grid_id> <num_threads> --eps <eps> --num\_episodes <num\_episodes> --print\_out

    - <grid_id>
      specify the id of the grid. If -1 the algorithm just generate a new grid, no more arguments are considered
    - <num_threads>
      how many thread to use: options are 1, 2, 4, 8
    - --eps <eps>
      the value of epsilon (0,1). Value -1 enable the eps = (1-episode/num_episodes)^2.  
      default 0.1
    - --num_episodes <num_episodes>
      default 32000
    - --discount <discount>
      default 0.95
    - --alpha <alpha>
      default 1
    - --mse <mse>
      maximum step per episode.
      default 10000
    - --name_option <name_option>
      define the last part of the name of the file that will be created.
    - --no_shared_mem
      remove the shared memory between the tasks.
    - --print_out
      at the end of the execution print the q_function computed by each agent, the global q_function and the time taken to run the algorithm.
    - --save_time
      save the execution time in a file called times/usid<id>nt<nt><name_option>.txt where <id> corresponds to the grid id, <nt> to the number of threads and <name_option> the optional name specified in the arguments.
    - --save_epval
      save the global value function computed at each episode in a file valfuncepisodes/grid_size14id<id>nt<nt><name_option>.bin
    - --save_stat
      save some statistics in a file called statistics/grid_size14id<id>nt<nt>.bin
    - --incremental
      if the file that is been creating already exists, the program append to the file an incremental name option (eg. "_0", "_1", ...)








