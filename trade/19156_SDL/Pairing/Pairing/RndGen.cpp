#include "RndGen.h"
#include <list>
#include <vector>
#include <map>
#include <Windows.h>	// GetCurrentProcessId

using namespace std;

const int RepeatNum = 2;

RndGen::RndGen(int max, int size)
{
	this->max = max; 
	this->size = size;
}

RndGen::~RndGen(void)
{
}

void RndGen::generate(unsigned int seed)
{
	// initialize
	clearQueue();

	// make a candidate
	list<int> cand;
	for (int i=1; i<=max; ++i) {
		cand.push_back(i);
	}

	// seed setting
	srand(seed);

	// make random number
	vector<int> v1;
	for (int i=0; i<(size/RepeatNum); ++i) {
		size_t candMax = cand.size();
		int what = rand()%candMax;
		list<int>::iterator pos = cand.begin();
		for (int l=0; l<what; ++l) {
			++pos;
		}
		v1.push_back(*pos);
		cand.erase(pos);
	}

	// make a double
	vector<int> v(v1);
	v.insert(v.end(), v1.begin(), v1.end());

	// random insert with multimap
	multimap<int, int> m;
	for (int i=0; i<size; ++i) {
		m.insert( pair<int, int>(rand()%max, v[i]) );
	}

	// insert into queue
	multimap<int, int>::iterator pos;
	for (pos=m.begin(); pos!=m.end(); ++pos) {
		que.push( pos->second );
	}

}

void RndGen::generate()
{
	generate(GetTickCount());
}

int RndGen::next()
{
	if(que.empty()) return 0;

	int r = que.front();
	que.pop();
	return r;
}

void RndGen::clearQueue()
{
	while(!que.empty()) que.pop();
}
