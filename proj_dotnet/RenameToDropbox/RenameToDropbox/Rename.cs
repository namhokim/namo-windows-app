using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace RenameToDropbox
{
    public class Rename
    {
        static readonly HashSet<String> extensions = new HashSet<String> { ".jpg", ".mp4" };
        static readonly string MatchRule1 =
            @"(?<YYYY>\d{4}?)(?<MM>\d{2}?)(?<DD>\d{2}?)_"
            + @"(?<hh>\d{2}?)(?<mm>\d{2}?)(?<ss>\d{2}?)\((?<seq>\d+?)\)";
        static readonly string MatchRule2 =
            @"(?<YYYY>\d{4}?)(?<MM>\d{2}?)(?<DD>\d{2}?)_"
            + @"(?<hh>\d{2}?)(?<mm>\d{2}?)(?<ss>\d{2}?)";

        public static bool IsTargetExtension(string extension)
        {
            return extensions.Contains(extension);
        }

        public static bool IsTargetPattern(string filename)
        {
            Match match1 = Regex.Match(filename, MatchRule1);
            Match match2 = Regex.Match(filename, MatchRule2);
            return (match1.Success || match2.Success);
        }

        public static string GetNewName(string onlyFilename)
        {
            Match match1 = Regex.Match(onlyFilename, MatchRule1);
            Match match2 = Regex.Match(onlyFilename, MatchRule2);
            if (match1.Success)
            {
                string newValue = string.Format("{0}-{1}-{2} {3}.{4}.{5} {6}",
                    match1.Groups["YYYY"].Value, match1.Groups["MM"].Value, match1.Groups["DD"].Value,
                    match1.Groups["hh"].Value, match1.Groups["mm"].Value, match1.Groups["ss"].Value,
                    match1.Groups["seq"].Value);
                return newValue;
            }
            else if (match2.Success)
            {
                string newValue = string.Format("{0}-{1}-{2} {3}.{4}.{5}",
                    match2.Groups["YYYY"].Value, match2.Groups["MM"].Value, match2.Groups["DD"].Value,
                    match2.Groups["hh"].Value, match2.Groups["mm"].Value, match2.Groups["ss"].Value);
                return newValue;
            }
            else
            {
                throw new ArgumentException("이름이 잘못되었습니다.");
            }
        }

    }
}
