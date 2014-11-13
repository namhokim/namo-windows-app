using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace RenameToDropbox
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        #region "buttonTargetDir_Click"
        private void buttonTargetDir_Click(object sender, EventArgs e)
        {
            string rootFolder = this.textBoxTargetDir.Text;

            using(var folder = new FolderBrowserDialog())
            {
                folder.Description = "Select the target directory";
                folder.ShowNewFolderButton = false;
                if (File.Exists(rootFolder))
                {
                    folder.SelectedPath = rootFolder;
                }

                if (folder.ShowDialog() == DialogResult.OK)
                {
                    this.textBoxTargetDir.Text = folder.SelectedPath;
                }
            }
        }
        #endregion

        #region "buttonRename_Click"
        private void buttonRename_Click(object sender, EventArgs e)
        {
            textBoxOutput.Clear();

            string rootFolder = this.textBoxTargetDir.Text;
            if (rootFolder != string.Empty) 
            {
                ProcessRename(rootFolder, GetProcessor(SelectedType));
            }
        }

        private IRenameProcessor GetProcessor(ProcessingType type)
        {
            IRenameProcessor instance;
            if (type == ProcessingType.Undefined)
            {
                throw new ArgumentException("선택한 타입이 잘못되었습니다.");
            }
            else
            {
                switch (type)
                {
                    default:
                    case ProcessingType.Oruxmap:
                        instance = new RenameOruxmap();
                        break;
                    case ProcessingType.Photo:
                        var photo = new RenamePhoto();
                        photo.TextBoxOutput = this.textBoxOutput;
                        instance = photo;
                        break;
                }
            }
            return instance;
        }

        private void ProcessRename(string path, IRenameProcessor processor)
        {
            foreach (string file in Directory.GetFiles(path))
            {
                processor.Process(file);
            }
        }

        private void ProcessEachFile(string filename)
        {
            FileInfo fi = new FileInfo(filename);
            if (Rename.IsTargetExtension(fi.Extension))
            {
                string onlyFilename = fi.ExtractOnlyFilename();
                if (Rename.IsTargetPattern(onlyFilename))
                {
                    string newName = Rename.GetNewName(onlyFilename);
                    File.Move(fi.FullName, fi.FullName.Replace(onlyFilename, newName));
                    textBoxOutput.AppendText(fi.Name + " => " + newName);
                    textBoxOutput.AppendText(Environment.NewLine);
                }
            }
        }
        #endregion

        #region "Processing Type"
        enum ProcessingType
        {
            Undefined, Oruxmap, Photo
        }

        private ProcessingType SelectedType
        {
            get
            {
                if (rbTypeOruxmap.Checked)
                {
                    return ProcessingType.Oruxmap;
                }
                else if (rbTypePhoto.Checked)
                {
                    return ProcessingType.Photo;
                }
                else
                {
                    return ProcessingType.Undefined;
                }
            }
        }
        #endregion
    }
}
