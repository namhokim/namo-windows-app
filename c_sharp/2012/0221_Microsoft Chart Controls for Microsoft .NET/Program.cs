using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Graph
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}

// http://www.hoons.kr/Board.aspx?Name=Pds&Mode=2&BoardIdx=16602&Key=&Value=
// http://www.taeyo.net/Columns/View.aspx?SEQ=346&PSEQ=11&IDX=0