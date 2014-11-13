using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RenameToDropbox
{
    using NUnit.Framework;

    [TestFixture]
    class RenameOruxmapTests
    {
        [Test]
        public void TestIsPattern()
        {
            var instance = new RenameOruxmap();
            Assert.False(instance.IsPattern("2014-11-07 1920 강릉(산)"), "__YYYYMMDD_HHMM 형태가 없는 것");
            Assert.True(instance.IsPattern("2014-11-08 1219 차량이동__20141108_1219"), "__YYYYMMDD_HHMM 형태");
        }

        [Test]
        public void TestGetRenameFile()
        {
            TestGetRenameFileWith("2014-11-08 1219 차량이동__20141108_1219.gpx", "2014-11-08 1219 차량이동.gpx");
            TestGetRenameFileWith("2014-11-08 1219 차량이동__20141108_1219.kmz", "2014-11-08 1219 차량이동.kmz");
            TestGetRenameFileWith("2015-11-08 1219 차량이동__20151108_1219.kmz", "2015-11-08 1219 차량이동.kmz");
            TestGetRenameFileWith("2015-11-08 1219.차량이동__20151108_1219.kmz", "2015-11-08 1219.차량이동.kmz");
            TestGetRenameFileWith("2015-11-08.1219.차량이동__20151108_1219.kmz", "2015-11-08.1219.차량이동.kmz");
        }

        private void TestGetRenameFileWith(string input, string expected)
        {
            var instance = new RenameOruxmap();
            string result = instance.GetRenameFile(input);
            Assert.AreEqual(expected, result);
        }

    }
}
