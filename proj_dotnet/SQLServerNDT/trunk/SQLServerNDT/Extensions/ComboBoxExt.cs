using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SQLServerNDT.Extensions
{
    public static class ComboBoxExt
    {
        public static void AddItemsCollection(
            this System.Windows.Forms.ComboBox comboBox, System.Collections.ICollection collection)
        {
            foreach (var v in collection)
            {
                comboBox.Items.Add(v);
            }
        }
    }
}
