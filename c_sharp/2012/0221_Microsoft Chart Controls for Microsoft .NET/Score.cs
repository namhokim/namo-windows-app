using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Graph
{
    public class Score
    {
        public string Name { get; set;}
        public int Korean { get; set; }
        public int Math { get; set; }
        public int English { get; set; }

        public Score(string name, int korean, int math, int english)
        {
            Name = name;
            Korean = korean;
            Math = math;
            English = english;
        }
    }
}
