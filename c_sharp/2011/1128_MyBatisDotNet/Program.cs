using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// http://www.mybatis.org/dotnet.html
// Mapper.cs
using IBatisNet.DataMapper;

namespace MyBatisDotNet
{
    class Program
    {
        static void Main(string[] args)
        {
            ISqlMapper mapper = MyMapper.Instance();
            IList<String> list = mapper.QueryForList<String>("GetLoginInfo", "");
            System.Console.WriteLine("size of list :{0}", list.Count);

            // http://www.dotnetperls.com/foreach
            foreach (String r in list)
            {
                System.Console.WriteLine(" {0}", r);
            }
        }
    }
}
