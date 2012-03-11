// refs. Encapsulating a Thread in a C++ Object
// http://jrdodds.blogs.com/blog/2006/06/encapsulating_a.html

#define WIN32_LEAN_AND_MEAN
#include <process.h>
#include <windows.h>
#include <iostream>

class thread
{
public:
	thread();
	virtual ~thread();

	const HANDLE& GetHandle() const { return m_hThread; }
	const bool wait() const;

private:
	// copy operations are private to prevent copying
	thread(const thread&);
	thread& operator=(const thread&);

	static unsigned __stdcall threadProc(void*);
	unsigned run();

	HANDLE m_hThread;
	unsigned m_tid;
};

thread::thread()
{
	m_hThread = reinterpret_cast<HANDLE>(
		::_beginthreadex(
		0,	// security
		0,	// stack size
		threadProc,	// thread routine
		static_cast<void*>(this),	// thread arg
		0,	// initial state flag
		&m_tid	//	thread ID
		)
		);
	if (m_hThread == 0) 
	{
		throw std::exception("failed to create thread");
	}
}

thread::~thread() 
{
	try    
	{
		::CloseHandle(GetHandle());
	}
	catch(...)
	{
		// suppress any exception; dtors should never throw
	}
}

const bool thread::wait() const
{
	bool bWaitSuccess = false;
	// a thread waiting on itself will cause a deadlock
	if (::GetCurrentThreadId() != m_tid)
	{
		DWORD nResult = ::WaitForSingleObject(GetHandle(), INFINITE);
		// nResult will be WAIT_OBJECT_0 if the thread has terminated;
		// other possible results: WAIT_FAILED, WAIT_TIMEOUT,
		// or WAIT_ABANDONED
		bWaitSuccess = (nResult == WAIT_OBJECT_0);
	}
	return bWaitSuccess;
}

unsigned __stdcall thread::threadProc(void* a_param)
{
	thread* pthread = static_cast<thread*>(a_param);
	return pthread->run();
}

unsigned thread::run()
{
	std::cout << "Hello from thread" << std::endl;
	return 0;
}

int main(int argc, char* const argv[])
{
	try
	{
		std::cout << "Hello" << std::endl;

		thread thrd;
		thrd.wait();

		std::cout << "Done" << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "unknown exception" << std::endl;
	}

	return 0;
}