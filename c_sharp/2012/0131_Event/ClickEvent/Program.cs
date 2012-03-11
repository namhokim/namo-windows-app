using System;

namespace ClickEvent
{
    public delegate void deleClick();

    class Button // Click이벤트 선언
    {
        public event deleClick Click;

        public void Draw()
        {
            Console.WriteLine("나는 버튼");
        }

        public void OnClick() // Click에 저장되어있는 메소드 호출
        {
            if (Click != null)
                Click();
        }
    }

    class Program
    {
        static void GameStart()
        {
            Console.WriteLine("게임시작");

            for (int i = 0; i < 5; i++)
            {
                Console.WriteLine('.');
                System.Threading.Thread.Sleep(50);
            }
            Console.WriteLine("\n 게임 끝");
        }

        static void Main(string[] args) // 이벤트에 GameStart라는 핸들러 할당
        {
            Button BtnStart = new Button();
            BtnStart.Draw();
            BtnStart.Click += GameStart;
            Console.WriteLine("S:게임시작. E:끝");
            for (; ; )
            {
                if (Console.KeyAvailable)
                {
                    ConsoleKeyInfo cki;

                    cki = Console.ReadKey();

                    if (cki.Key == ConsoleKey.S)
                    {
                        BtnStart.OnClick();
                    }

                    if (cki.Key == ConsoleKey.E)
                    {
                        break;
                    }
                }
            }
        }
    }
}