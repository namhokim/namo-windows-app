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
        #region "MainForm > InitializeComponent"
        public MainForm()
        {
            InitializeComponent();
        }
        #endregion

        #region "buttonTargetDir_Click"
        private void buttonTargetDir_Click(object sender, EventArgs e)
        {
            string rootFolder = this.textBoxTargetDir.Text;

            using(var folder = new FolderBrowserDialog())
            {
                folder.Description = "Select the target directory";
                folder.ShowNewFolderButton = false;
                if (Directory.Exists(rootFolder))
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
                        instance = photo;
                        break;
                }
                instance.TextBoxOutput = this.textBoxOutput;
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

        #region "Drag & Drop"
        private void MainForm_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Move;
        }

        private void MainForm_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
            foreach (string file in files)
            {
                if (IsDirectory(file))
                {
                    textBoxTargetDir.Text = file;
                    break;
                }
            }
        }

        private bool IsDirectory(string path)
        {
            FileAttributes attr = File.GetAttributes(path);
            return ((attr & FileAttributes.Directory) == FileAttributes.Directory);
        }
        #endregion
    }
}
