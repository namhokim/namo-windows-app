///////////////////////////////////////////////////////////////////////////////

#pragma warning(disable : 4251)	// Prevent warning message "node::ObjectWrap::handle_"
								// It's not important, because we just provide functions
// refs. http://nodejs.org/api/addons.html
#define BUILDING_NODE_EXTENSION
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

const char* ERR_PARAM_LEN_MESSAGE = "message function must be with two argument";
const char* ERR_PARAM_LEN_DISCONN = "disconnect function must be with two argument";
const char* ERR_PARAM_ONE = "1st Argument must be an unsigned integer";
const char* ERR_PARAM_TWO = "2st Argument must be an string";
const int	ONE_PARAMETERS = 1;
const int	TWO_PARAMETERS = 2;

///////////////////////////////////////////////////////////////////////////////
// Structure for Async
struct Baton {
    uv_work_t request;
    Persistent<Function> callback;
    int error_code;
    std::string error_message;

    // Custom data
    uint32_t result;
};

///////////////////////////////////////////////////////////////////////////////
// define for Async functions
void AsyncWork(uv_work_t* req);
void AsyncAfter(uv_work_t* req);


///////////////////////////////////////////////////////////////////////////////
Handle<Value> EnumHandler(const Arguments& args)
{
	HandleScope scope;

	Json::Value out;
	Json::FastWriter w;
	// execute C++ user function
	wts::ShowCurrentConnectedUser(out);

	return scope.Close(String::New(w.write(out).c_str()));
}

Handle<Value> MsgHandler(const Arguments& args)
{
	HandleScope scope;

	// check the param
	if(args.Length() != TWO_PARAMETERS) {
		ThrowException( Exception::TypeError(String::New(ERR_PARAM_LEN_MESSAGE)) );
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

Handle<Value> DisConnHandler(const Arguments& args)
{
	HandleScope scope;

	// check the param
	if(args.Length() != ONE_PARAMETERS) {
		ThrowException( Exception::TypeError(String::New(ERR_PARAM_LEN_DISCONN)) );
		return scope.Close(Undefined());
	}
	if(!args[0]->IsUint32()) {
		ThrowException( Exception::TypeError(String::New(ERR_PARAM_ONE)) );
		return scope.Close(Undefined());
	}

	// get the param
	uint32_t		sid = args[0]->Uint32Value();

	// execute C++ user function
	wts::DisconnectSession(sid);

	return scope.Close(String::New("OK"));
}

Handle<Value> CallbackHandler(const Arguments& args) {
    HandleScope scope;

    if (!args[0]->IsFunction()) {
        return ThrowException(Exception::TypeError(
            String::New("Callback function required")));
    }
    Local<Function> callback = Local<Function>::Cast(args[0]);

    Baton* baton = new Baton();
    baton->request.data = baton;
    baton->callback = Persistent<Function>::New(callback);

    uv_queue_work(uv_default_loop(), &baton->request,
        AsyncWork, AsyncAfter);

    return scope.Close(Undefined());
}

///////////////////////////////////////////////////////////////////////////////

void init(Handle<Object> target)
{
	NODE_SET_METHOD(target, "enumerate", EnumHandler);
	NODE_SET_METHOD(target, "message", MsgHandler);
	NODE_SET_METHOD(target, "disconnect", DisConnHandler);
	NODE_SET_METHOD(target, "monitor", CallbackHandler);
}

NODE_MODULE(wts, init)


///////////////////////////////////////////////////////////////////////////////
// Async handlers

void AsyncWork(uv_work_t* req) {
    // No HandleScope!

    Baton* baton = static_cast<Baton*>(req->data);

    // Do work in threadpool here.
    // Set baton->error_code/message on failures.
	baton->error_code = wts::WaitForEvent(&baton->result, baton->error_message);
}

void AsyncAfter(uv_work_t* req) {
    HandleScope scope;
    Baton* baton = static_cast<Baton*>(req->data);

	if (baton->error_code) {
		Local<Value> err = Exception::Error(
			String::New(baton->error_message.c_str()));
		Local<Value> argv[] = { err };

		TryCatch try_catch;
		baton->callback->Call(
			Context::GetCurrent()->Global(), 1, argv);

		if (try_catch.HasCaught()) {
			node::FatalException(try_catch);
		}
    } else {
        // Call baton->callback with results.
		const unsigned argc = 1;
		Local<Value> argv[argc] = { Local<Value>::New(Uint32::New(baton->result)) };

		baton->callback->Call(
			Context::GetCurrent()->Global(), argc, argv);
    }

    baton->callback.Dispose();
    delete baton;
}
