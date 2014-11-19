using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RenameToDropbox
{
    interface IRenameProcessor
    {
        System.Windows.Forms.TextBox TextBoxOutput { get; set; }

        void Process(string filename);
    }
}
