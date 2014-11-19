using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace RenameToDropbox
{
    class RenamePhoto : IRenameProcessor
    {
        public System.Windows.Forms.TextBox TextBoxOutput { get; set; }

        public void Process(string filename)
        {
            string fullFilename, onlyFilename, dirName;
            if (IsTarget(filename, out fullFilename, out onlyFilename, out dirName))
            {
                string newFullName = GetRenameFile(fullFilename, onlyFilename);

                RenameIt(fullFilename, newFullName, dirName);
            }
        }

        private bool IsTarget(string filename, out string fullFilename, out string onlyFilename, out string dirName)
        {
            FileInfo fi = new FileInfo(filename);

            if (Rename.IsTargetExtension(fi.Extension))
            {
                fullFilename = fi.FullName;
                onlyFilename = fi.ExtractOnlyFilename();
                dirName = fi.DirectoryName;
                return Rename.IsTargetPattern(onlyFilename);
            }
            else
            {
                fullFilename = string.Empty;
                onlyFilename = string.Empty;
                dirName = string.Empty;
                return false;
            }
        }

        private string GetRenameFile(string fullFilename, string onlyFilename)
        {
            string newName = Rename.GetNewName(onlyFilename);
            string newFullName = fullFilename.Replace(onlyFilename, newName);
            int postfix = 0;
            while (File.Exists(newFullName))
            {
                postfix++;
                newFullName = fullFilename.Replace(onlyFilename, newName + "-" + postfix);
            }
            return newFullName;
        }

        private void RenameIt(string filename, string renameFile, string dirName)
        {
            File.Move(filename, renameFile);

            if (TextBoxOutput != null)
            {
                string removePrefix = dirName + Path.DirectorySeparatorChar;
                TextBoxOutput.AppendText(filename.Replace(removePrefix, string.Empty) + " => " + renameFile.Replace(removePrefix, string.Empty));
                TextBoxOutput.AppendText(Environment.NewLine);
            }
        }

    }
}
