#include "dynamicprog.h"

typedef  Eigen::Matrix<Q_value, Eigen::Dynamic, Eigen::Dynamic> QList;

class Agent
{
public:
	Agent(Environment env);
	int epsilonGreedyPolicy(int state, double epsilon);
	void learn(double epsilon = -1, int num_episodes = 32000, double discount_factor = 0.95, double alpha = 1, int MSE = 10000);
	std::shared_ptr<Eigen::MatrixXd> getQ();
	void saveQ(std::string fn);
	std::shared_ptr<Eigen::MatrixXd> readQ(std::string fn);
	~Agent(){};

private:
	int num_actions;
	Environment env;
	std::shared_ptr<PosVector> states_list;
	std::shared_ptr<Eigen::MatrixXd> q_function;
	std::shared_ptr<Eigen::MatrixXd> q_cache;
};