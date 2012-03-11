using System;

namespace CompleteEvent
{
    class Program
    {
        static void Main(string[] args)
        {
            Printer P = new Printer();
            Receiver R = new Receiver();

            R.OnComplete += P.Print; // OnComplete 이벤트후에 호출하고 싶은 메소드 등록
            R.Receive();
            // P.Print();
        }
    }

    delegate void Notice();

    class Printer
    {
        public void Print() // 수신완료라는 사건에 대한 이벤트 핸들러
        {
            Console.WriteLine("수신된 데이터를 인쇄합니다");
        }
    }

    class Receiver // 수신완료 시 호출할 메소드를 저장하기 위해 OnComplete 이벤트 소유
    {
        public event Notice OnComplete;

        public void Receive() // 네트워크에서 데이터를 수신하는 동작
        {
            for (int i = 0; i < 100; i++)
            {
                Console.WriteLine(i + "%수신중");
                System.Threading.Thread.Sleep(5000);
            }

            if (OnComplete != null)
                OnComplete();
        }
    }
}