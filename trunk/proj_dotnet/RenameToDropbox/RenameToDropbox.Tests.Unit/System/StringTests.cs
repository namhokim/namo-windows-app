
namespace System
{
    using NUnit.Framework;

    [TestFixture]
    class StringTests
    {
        [Test]
        public void TestFormat()
        {
            Assert.AreEqual("0.001", string.Format("0.{0:000}", 1));
            Assert.AreEqual("0.002", string.Format("0.{0:000}", 2));
            Assert.AreEqual("0.003", string.Format("0.{0:000}", 3));
            Assert.AreEqual("0.010", string.Format("0.{0:000}", 10));
            Assert.AreEqual("0.100", string.Format("0.{0:000}", 100));
        }
    }
}
