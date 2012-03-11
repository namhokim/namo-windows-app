#include "SingletonOperation.h"

namespace my
{
	bool g_value_initialized = false;
	int g_before_value = 0;

	int add(int a, int b)
	{
		if(!g_value_initialized) {
			g_before_value = a + b;
			g_value_initialized = true;
		}
		return g_before_value;
	}
}