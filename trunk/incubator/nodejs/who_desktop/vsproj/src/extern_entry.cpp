#include <v8.h>
using namespace v8;

#include <node.h>
#ifdef _M_X64
#pragma comment(lib, "lib/x64/node.lib")
#else
#pragma comment(lib, "lib/x86/node.lib")
#endif

extern "C" void NODE_EXTERN init (Handle<Object> target)
{
   HandleScope scope;
   target->Set(String::New("hello"), String::New("world"));
}