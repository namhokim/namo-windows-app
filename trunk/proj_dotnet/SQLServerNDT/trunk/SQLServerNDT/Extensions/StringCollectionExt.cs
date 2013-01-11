using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SQLServerNDT.Extensions
{
    public static class StringCollectionExt
    {
        // 요약:
        //     Adds a string to the end of the System.Collections.Specialized.StringCollection.
        //
        // 매개 변수:
        //   value:
        //     The string to add to the end of the System.Collections.Specialized.StringCollection.
        //     The value can be null.
        //
        // 반환 값:
        //     The zero-based index at which the new element is inserted.
        private static void AddWithLimit(
            this System.Collections.Specialized.StringCollection collection, String value, int limit)
        {
            const int SecondIndex = 1;

            collection.Add(value);
            while (limit > SecondIndex && collection.Count > limit)
            {
                collection.RemoveAt(SecondIndex);
            }
        }

        /// <summary>
        /// System.Collections.Specialized.StringCollection의 끝에 문자열을 추가합니다.
        /// <para>System.Windows.Forms.ComboBox의 MaxDropDownItems를 초과하는 두번째 원소는 제거됩니다.</para>
        /// </summary>
        /// <param name="comboBox">데이터는 Text 프로퍼티가 들어가고, 제한은 MaxDropDownItems 프로퍼티가 됩니다.</param>
        public static void AddWithLimit(
            this System.Collections.Specialized.StringCollection collection,
            System.Windows.Forms.ComboBox comboBox)
        {
            collection.AddWithLimit(comboBox.Text, comboBox.MaxDropDownItems);
        }

        public static void Reverse(
            this System.Collections.Specialized.StringCollection collection)
        {
            //return collection.GetEnumerator();
        }
        
    }
}
