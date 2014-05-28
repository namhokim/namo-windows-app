
namespace RenameToDropbox
{
    using NUnit.Framework;

    [TestFixture]
    class RenameTests
    {
        #region "IsTargetExtension 메서드 테스트"
        [Test]
        public void TestIsTargetExtension_대상확장자()
        {
            Assert.True(Rename.IsTargetExtension(".mp4"));
            Assert.True(Rename.IsTargetExtension(".jpg"));
        }

        [Test]
        public void TestIsTargetExtension_비대상확장자()
        {
            Assert.False(Rename.IsTargetExtension(".mp3"));
            Assert.False(Rename.IsTargetExtension(".mov"));
        }
        #endregion

        #region "TargetPattern 메서드 테스트"
        [Test]
        public void TestIsTargetPattern_변환할형태파일명()
        {
            Assert.True(Rename.IsTargetPattern("20140524_111014"));
        }

        [Test]
        public void TestIsTargetPattern_이미변환된형태이름()
        {
            Assert.False(Rename.IsTargetPattern("2014-05-24 07.37.30"));
        }
        #endregion

        #region "GetNewName 메서드 테스트"
        [Test]
        public void TestGetNewName_변환할형태파일명()
        {
            Assert.AreEqual("2014-05-24 11.10.14",
                Rename.GetNewName("20140524_111014"));

            Assert.AreEqual("2014-05-24 11.10.14 0",
                Rename.GetNewName("20140524_111014(0)"));
        }
        #endregion

    }
}
