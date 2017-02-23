#include "dynamicprog.h"

typedef  Eigen::Matrix<Q_value, Eigen::Dynamic, Eigen::Dynamic> QList;

class Agent
{
public:
	Agent(Environment env);
	Agent(Environment env, int init_row, int end_row, int init_col, int end_col, int param_cache_size);
	int epsilonGreedyPolicy(int state, double epsilon);
	void learn(double epsilon = 0.1, int num_episodes = 32000, double discount_factor = 0.95, double alpha = 1, int MSE = 10000);
	std::shared_ptr<Eigen::MatrixXd> getQ();
	void saveQ(std::string fn);
	std::shared_ptr<Eigen::MatrixXd> readQ(std::string fn);
	~Agent(){};

private:
	int init_row;
	int end_row;
	int init_col;
	int end_col;
	int num_actions;
	int cache_size;
	Environment env;
	std::shared_ptr<PosVector> states_list;
	static std::shared_ptr<Eigen::MatrixXd> global_q;
	std::shared_ptr<Eigen::MatrixXd> q_function;
	std::shared_ptr<Eigen::VectorXd> q_cache;
};