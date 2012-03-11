#pragma once

#include "../GCD/gcd.h"

//calculate the lcm of two numbers
// LCM(least common multiple)

// use follow's algorithm
// lcm(a,b) = ab / gcd(a,b)
template<class _Ty>
_Ty lcm(_Ty a, _Ty b)
{
	_Ty gcd_a_b = gcd(a, b);

	return ((a*b)/gcd_a_b);
}