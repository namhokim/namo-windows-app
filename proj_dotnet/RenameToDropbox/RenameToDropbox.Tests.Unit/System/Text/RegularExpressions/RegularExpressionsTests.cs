using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace System.Text.RegularExpressions
{
    using NUnit.Framework;

    [TestFixture]
    class RegularExpressionsTests
    {
        [Test]
        public void TestIsMatch()
        {
            string target = "__20141108_1219";
            var postfix = new Regex(@"__\d{8}_\d{4}");
            Assert.True(postfix.IsMatch(target));
        }
    }
}
