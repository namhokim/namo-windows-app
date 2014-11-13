using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace RenameToDropbox
{
    public class RenameOruxmap : IRenameProcessor
    {
        public System.Windows.Forms.TextBox TextBoxOutput { get; set; }

        /// <summary>
        /// eg) __20141108_1219
        /// </summary>
        static readonly Regex Postfix = new Regex(@"__\d{8}_\d{4}");
        static readonly string PostfixSample = "__20141108_1219";

        public void Process(string filename)
        {
            string renameFile;
            if (IsTarget(filename, out renameFile))
            {
                RenameIt(filename, renameFile);
            }
        }

        private bool IsTarget(string filename, out string renameFile)
        {
            renameFile = string.Empty;
            FileInfo fi = new FileInfo(filename);
            if (IsExension(fi.Extension) && IsPattern(fi.Name))
            {
                renameFile = GetRenameFile(filename);
                return true;
            }
            else
            {
                return false;
            }
        }

        private bool IsExension(string extension)
        {
            return (extension == ".gpx" || extension == ".kmz");
        }

        public bool IsPattern(string name)
        {
            var part = name.Split(new char[] { '.'});
            string onlyName = part[0];
            if (onlyName.Length < PostfixSample.Length)
            {
                return false;
            }
            else
            {
                string extract = onlyName.Substring(onlyName.Length - PostfixSample.Length);
                return Postfix.IsMatch(extract);
            }
        }

        /// <summary>
        /// 바꿀 파일 이름을 가져온다. (주의. 절대 경로를 입력하도록 한다.)
        /// </summary>
        /// <param name="filename"></param>
        /// <returns></returns>
        public string GetRenameFile(string filename)
        {
            int separated = filename.LastIndexOf('.');   // 마지막 .을 기준으로 이름/확장자를 나눈다.
            string onlyName = filename.Substring(0, separated);
            string onlyExt = filename.Substring(separated);
            string extract = onlyName.Substring(0, onlyName.Length - PostfixSample.Length);
            return extract + onlyExt;
        }

        private void RenameIt(string filename, string renameFile)
        {
            File.Move(filename, renameFile);
        }

    }
}
