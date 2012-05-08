#include <v8.h>
using namespace v8;

#include <node.h>
#ifdef _M_X64
#pragma comment(lib, "lib/x64/node.lib")
#else
#pragma comment(lib, "lib/x86/node.lib")
#endif

Handle<Value> EnumHandler(const Arguments& args) {
   HandleScope scope;
   return scope.Close(String::New("enum handler"));
}

Handle<Value> MsgHandler(const Arguments& args) {
   HandleScope scope;
   return scope.Close(String::New("message handler"));
}

void init(Handle<Object> target) {
	NODE_SET_METHOD(target, "enum", EnumHandler);
	NODE_SET_METHOD(target, "message", MsgHandler);
}

NODE_MODULE(wts, init)