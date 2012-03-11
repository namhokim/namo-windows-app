using System;
using System.Threading;

namespace AsynMethodCall
{
    /// <summary>
    /// http://support.microsoft.com/kb/315582/ko
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            // 예제 1: 동기적으로 A 메서드 호출
            AsyncDemo ad = new AsyncDemo();
            //ad.DemoSyncCall();

            // 예제 2: 비동기적으로 호출 - EndInvoke() 호출 패턴을 사용
            ad.DemoEndInvoke();

            // 예제 3~5는 EndInvoke()를 호출할 동기화에 대한 것임(예제2에서 추가)
            // 예제 3: 비동기적으로 호출 -  AsyncWaitHandle을 이용하여 완료시까지 기다림
            ad.DemoWaitHandle();

            // 예제 4: 비동기적으로 호출 - 폴링 호출 패턴을 사용하여 메서드 호출
            //ad.DemoPolling();

            // 예제 5: 한 콜백 비동기 메서드 완료시 실행
            //ad.DemoCallback();
        }
    }

    public class AsyncDemo
    {
        string LongRunningMethod(int iCallTime, out int iExecThread)
        {
            Thread.Sleep(iCallTime);
            iExecThread = Thread.CurrentThread.ManagedThreadId;
            return "MyCallTime was " + iCallTime.ToString();
        }

        // The following delegate
        delegate string MethodDelegate(int iCallTime, out int iExecThread);

        // 예제1
        public void DemoSyncCall()
        {
            string s;
            int iExecThread;

            // Create an instance of a delegate that wraps LongRunningMethod.
            MethodDelegate dlgt = new MethodDelegate(this.LongRunningMethod);

            // Call LongRunningMethod using the delegate.
            s = dlgt(3000, out iExecThread);

            Console.WriteLine("The delegate call returned the string: \"{0}\", and the thread ID {1}",
                s, iExecThread.ToString());
        }

        // 예제2
        public void DemoEndInvoke()
        {
            MethodDelegate dlgt = new MethodDelegate(this.LongRunningMethod);
            int iExecThread;

            // Initiate the asynchronous call.
            IAsyncResult ar = dlgt.BeginInvoke(3000, out iExecThread, null, null);

            // Do some useful work here. This would be work you want to have
            // run at the same time as the asynchronous call.

            // Retrieve the results of the asynchronous call.
            string s = dlgt.EndInvoke(out iExecThread, ar);

            Console.WriteLine("The delegate call returned the string: \"{0}\", and the number {1}",
                s, iExecThread.ToString());
        }

        // 예제3
        public void DemoWaitHandle()
        {
            MethodDelegate dlgt = new MethodDelegate(this.LongRunningMethod);
            int iExecThread;

            // Initiate the asynchronous call.
            IAsyncResult ar = dlgt.BeginInvoke(3000, out iExecThread, null, null);

            // Do some useful work here. This would be work you want to have
            // run at the same time as the asynchronous call.

            // Wait for the WaitHandle to become signaled. (일반적인 스레드 동기화 기술)
            ar.AsyncWaitHandle.WaitOne();   // 시그널 될때까지 여기서 블록

            // Get the results of the asynchronous call.
            string s = dlgt.EndInvoke(out iExecThread, ar);

            Console.WriteLine("The delegate call returned the string:   \"{0}\", and the number {1}",
                s, iExecThread.ToString());
        }

        // 예제4
        public void DemoPolling()
        {
            MethodDelegate dlgt = new MethodDelegate(this.LongRunningMethod);
            int iExecThread;

            // Initiate the asynchronous call.
            IAsyncResult ar = dlgt.BeginInvoke(3000, out iExecThread, null, null);

            // Poll IAsyncResult.IsCompleted
            while (ar.IsCompleted == false)
            {
                Thread.Sleep(10);  // pretend to so some useful work
            }
            string s = dlgt.EndInvoke(out iExecThread, ar);

            Console.WriteLine("The delegate call returned the string:   \"{0}\", and the number {1}",
                s, iExecThread.ToString());
        }

        // 예제5
        public void DemoCallback()
        {
            MethodDelegate dlgt = new MethodDelegate(this.LongRunningMethod);
            int iExecThread;

            // Create the callback delegate.
            AsyncCallback cb = new AsyncCallback(MyAsyncCallback);

            // Initiate the Asynchronous call passing in the callback delegate
            // and the delegate object used to initiate the call.
            IAsyncResult ar = dlgt.BeginInvoke(3000, out iExecThread, cb, dlgt);
        }

        public void MyAsyncCallback(IAsyncResult ar)
        {
            int iExecThread;

            // Because you passed your original delegate in the asyncState parameter
            // of the Begin call, you can get it back here to complete the call.
            MethodDelegate dlgt = (MethodDelegate)ar.AsyncState;

            // Complete the call.
            string s = dlgt.EndInvoke(out iExecThread, ar);

            Console.WriteLine("The delegate call returned the string:   \"{0}\", and the number {1}",
                s, iExecThread.ToString());
}

    }
}
