///////////////////////////////////////////////////////////////////////////////

#pragma warning(disable : 4251)	// Prevent warning message "node::ObjectWrap::handle_"
								// It's not important, because we just provide functions
#include <node.h>
#include <v8.h>
using namespace v8;	// internal included <v8.h>

#ifdef _M_X64
#pragma comment(lib, "lib/x64/node.lib")
#else
#pragma comment(lib, "lib/x86/node.lib")
#endif

#include "whoConnect.h"

///////////////////////////////////////////////////////////////////////////////
// Constants

const char* ERR_PARAM_LEN = "message function must be with two argument";
const char* ERR_PARAM_ONE = "1st Argument must be an unsigned integer";
const char* ERR_PARAM_TWO = "2st Argument must be an string";
const int	TWO_PARAMETERS = 2;

///////////////////////////////////////////////////////////////////////////////

Handle<Value> EnumHandler(const Arguments& args)
{
	HandleScope scope;

	Json::Value out;
	Json::FastWriter w;
	wts::ShowCurrentConnectedUser(out);

	return scope.Close(String::New(w.write(out).c_str()));
}

Handle<Value> MsgHandler(const Arguments& args)
{
	HandleScope scope;

	// check the param
	// refs. http://nodejs.org/api/addons.html
	if(args.Length() != TWO_PARAMETERS) {
		ThrowException( Exception::TypeError(String::New(ERR_PARAM_LEN)) );
		return scope.Close(Undefined());
	}
	if(!args[0]->IsUint32()) {
		ThrowException( Exception::TypeError(String::New(ERR_PARAM_ONE)) );
		return scope.Close(Undefined());
	}
	if(!args[1]->IsString()) {
		ThrowException( Exception::TypeError(String::New(ERR_PARAM_TWO)) );
		return scope.Close(Undefined());
	}

	// get the param
	// refs. http://stackoverflow.com/questions/7476145/converting-from-v8arguments-to-c-types
	uint32_t		sid = args[0]->Uint32Value();
	String::Value	msg( args[1]->ToString() );

	// execute C++ user function
	wts::SendMessageToSesstionID(sid, reinterpret_cast<wchar_t *>(*msg));

	return scope.Close(String::New("OK"));
}

///////////////////////////////////////////////////////////////////////////////

void init(Handle<Object> target)
{
	NODE_SET_METHOD(target, "enum", EnumHandler);
	NODE_SET_METHOD(target, "message", MsgHandler);
}

NODE_MODULE(wts, init)