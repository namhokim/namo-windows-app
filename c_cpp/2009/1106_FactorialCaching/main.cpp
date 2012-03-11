#include <stdio.h>
#include <vector>

std::vector<int> Factorial_Cache;

int fac(std::vector<int>::size_type i) {
	if(Factorial_Cache.empty()) {
		Factorial_Cache.push_back(1);	// 0! = 1
		Factorial_Cache.push_back(1);	// 1! = 1
	}
	if(Factorial_Cache.size() > i) return Factorial_Cache.at(i);
	else {
		int before_value = fac(i-1);
		Factorial_Cache.push_back(before_value * i);
		return Factorial_Cache.at(i);
	}
}

int main()
{
	printf("Result of 1!+3!+5!+7!+9! : %d\n", fac(20) + fac(3) + fac(5) + fac(7) + fac(9));
	return 0;
}