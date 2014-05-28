using System.IO;

namespace RenameToDropbox
{
    using NUnit.Framework;

    [TestFixture]
    class FileInfoExtentionTests
    {
        [Test]
        public void TestExtractOnlyFilename_올바른형태()
        {
            Assert.AreEqual("test", (new FileInfo("test.ext")).ExtractOnlyFilename());
            Assert.AreEqual("test2", (new FileInfo("test2.ext")).ExtractOnlyFilename());
        }
    }
}
