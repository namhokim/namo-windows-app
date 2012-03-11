#ifndef USE_PIMPL

#include "FooInternalData.h"
#include "Header.h"
#include "foo.h"

int main()
{
    CFoo foo;
    foo.ProcessFile("c:\\data.bin");
    return 0;
} 
#else
#include "foo.h"

int main() 
{
    CFoo foo;
    foo.ProcessFile("c:\\data.bin");
    return 0; 
} 
#endif