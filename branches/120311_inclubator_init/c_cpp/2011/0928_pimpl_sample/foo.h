#pragma once

#ifndef USE_PIMPL

class CFoo
{
public:
    CFoo();
    ~CFoo();
    bool ProcessFile(const char* csFile);
private:
    CFooInternalData    m_data;
    CHeader             m_header;
};

#else

#include <memory>

//here just declare the class PIMPL to compile. 
//As I use this class with a pointer, I can use this declaration 
class CFoo_pimpl; 

class CFoo
{
public:
    CFoo();
    ~CFoo();
    bool ProcessFile(const char* csFile);
private:
    std::auto_ptr<CFoo_pimpl>    m_pImpl;
};

#endif