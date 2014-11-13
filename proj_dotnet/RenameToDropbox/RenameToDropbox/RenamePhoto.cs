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
            FileInfo fi = new FileInfo(filename);
            if (Rename.IsTargetExtension(fi.Extension))
            {
                string onlyFilename = fi.ExtractOnlyFilename();
                if (Rename.IsTargetPattern(onlyFilename))
                {
                    string newName = Rename.GetNewName(onlyFilename);
                    File.Move(fi.FullName, fi.FullName.Replace(onlyFilename, newName));
                    if (TextBoxOutput != null)
                    {
                        TextBoxOutput.AppendText(fi.Name + " => " + newName);
                        TextBoxOutput.AppendText(Environment.NewLine);
                    }
                }
            }
        }
    }
}
