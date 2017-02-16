#include <cstdint>

struct position {
	int row;
	int col;
};

struct observation {
	int next_state;
	double reward;
	bool done;
};

enum Actions : uint8_t
{
  up = 0, // 0
  down = 1, // 1
  left = 2, // 2
  right = 3 // 3
};

struct Q_value
{
	int value = 0;
	int cache = 0;
};