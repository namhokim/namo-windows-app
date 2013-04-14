#pragma once

#include <queue>

class RndGen
{
public:
	RndGen(int max, int size);
	~RndGen(void);

	void generate(unsigned int seed);
	void generate();
	int next();

private:
	int max, size;
	std::queue<int> que;

	void clearQueue();
};
