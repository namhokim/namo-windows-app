using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Graph
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            var scores = new List<Score>();

            scores.Add(new Score("김", 50, 100, 50));
            scores.Add(new Score("이", 40, 20, 70));
            scores.Add(new Score("박", 30, 30, 10));
            scores.Add(new Score("안", 20, 40, 20));
            scores.Add(new Score("문", 20, 10, 30));
            scores.Add(new Score("노", 10, 10, 30));

            chart_bar.DataSource = scores;
        }
    }
}
