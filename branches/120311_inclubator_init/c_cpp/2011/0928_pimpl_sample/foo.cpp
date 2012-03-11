#ifndef USE_PIMPL

#include "FooInternalData.h"
#include "Header.h"
#include "foo.h"

CFoo::CFoo()
{
}

CFoo::~CFoo()
{
}

bool CFoo::ProcessFile(const char* csFile)
{
    //do something
    return true;
}

#else

#include "FooInternalData.h"
#include "Header.h"
#include "foo.h"

//here defines PIMPl class, because it is use only in this file
class CFoo_pimpl
{
public:
    CFoo_pimpl()
    {
    }

    ~CFoo_pimpl()
    {
    }  
    bool ProcessFile(const char* csFile)
    {
        //do something
        return true;
    }
};

CFoo::CFoo()
:m_pImpl(new CFoo_pimpl())
{
}

CFoo::~CFoo()
{
    //do not have to delete, std::auto_ptr is very nice 
}

bool CFoo::ProcessFile(const char* csFile)
{
    //just call your PIMPL function ;-)
    return m_pImpl->ProcessFile(csFile);
}

#endif