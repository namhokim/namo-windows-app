using System.IO;

namespace RenameToDropbox
{
    public static class FileInfoExtention
    {
        public static string ExtractOnlyFilename(this FileInfo fileinfo)
        {
            return fileinfo.Name.Replace(fileinfo.Extension, string.Empty);
        }
    }
}
