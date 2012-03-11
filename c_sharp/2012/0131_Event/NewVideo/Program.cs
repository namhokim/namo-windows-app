using System;

namespace NewVideo
{
    // 멀티캐스트 델리게이트
    // 1. 여러 개의 메소드를 한꺼번에 호출하므로 리턴 타입은 void
    // 2. 하나의 사건을 여러 개의 객체에 동시에 전달하고 싶을 때 사용
    delegate void dRendMe(string Movie);

    class Mania
    {
        private int id;

        public Mania(int aid)
        {
            id = aid;
        }

        public void RendMe(string Movie)
        {
            Console.WriteLine("{0}번 고객이 {1}을 빌려간다", id, Movie);
        }
    }

    class aaaa
    {
        static void Main(string[] args)
        {
            Mania[] arMania = new Mania[10];

            for (int i = 0; i < 10; i++)
            {
                arMania[i] = new Mania(i);
            }

            dRendMe RentCall = null;

            RentCall += arMania[1].RendMe;
            RentCall += arMania[3].RendMe;
            RentCall += arMania[5].RendMe;
            RentCall += arMania[6].RendMe;

            RentCall("ddd");

        }
    }

}