#ifndef AGENT
#define AGENT

#include "dynamicprog.h"
#include <pthread.h>

typedef  Eigen::Matrix<Q_value, Eigen::Dynamic, Eigen::Dynamic> QList;

#define EPS 0.1
#define NUM_EP 320
#define DISCOUNT 0.95
#define ALPHA 1
#define MSE 10000

class Agent
{
public:
	Agent(Environment env);
	Agent(Environment env, int init, int end, int param_cache_size, pthread_mutex_t mutex);
	int epsilonGreedyPolicy(int state, double epsilon);
	static void * learn(void * agent);
	std::shared_ptr<Eigen::MatrixXd> getQ();
	std::shared_ptr<Eigen::MatrixXd> getGlobalQ();
	void saveQ(std::string fn);
	std::shared_ptr<Eigen::MatrixXd> readQ(std::string fn);
	~Agent(){};

private:
	int init_row;
	int end_row;
	int num_actions;
	int cache_size;
	Environment env;
	std::shared_ptr<PosVector> states_list;
	static std::shared_ptr<Eigen::MatrixXd> global_q;
	std::shared_ptr<Eigen::MatrixXd> q_function;
	std::shared_ptr<Eigen::VectorXd> q_cache;
	pthread_mutex_t mutex;
};

#endif
