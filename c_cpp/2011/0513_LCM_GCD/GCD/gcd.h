#pragma once

#include <utility>

//calculate the gcd of two numbers
// GCD(greatest common divisor)
template<class _Ty>
_Ty gcd(_Ty a, _Ty b)
{
	if ( a < b ) std::swap(a,b);

	while ( b > 0 ) {
		_Ty c = b;
		b = a % b;
		a = c;
	}

	return a;
}