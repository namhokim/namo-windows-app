using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace RenameToDropbox
{
    public class Rename
    {
        #region "static field"
        static readonly HashSet<String> extensions = new HashSet<String> { ".jpg", ".mp4", ".gif" };
        static readonly Regex MatchRule1 = new Regex(
            @"(?<YYYY>\d{4}?)(?<MM>\d{2}?)(?<DD>\d{2}?)_"
            + @"(?<hh>\d{2}?)(?<mm>\d{2}?)(?<ss>\d{2}?)\((?<seq>(\d+)?)\)");
        static readonly Regex MatchRule2 = new Regex(
            @"(?<YYYY>\d{4}?)(?<MM>\d{2}?)(?<DD>\d{2}?)_"
            + @"(?<hh>\d{2}?)(?<mm>\d{2}?)(?<ss>\d{2}?)");
        static readonly Regex MatchRuleSCAM = new Regex(
            @"(?<MM>\d{2}?)-(?<DD>\d{2}?)@"
            + @"(?<hh>\d{2}?)-(?<mm>\d{2}?)-(?<ss>\d{2}?)-(?<fff>(\d+)?)");
        #endregion

        public static bool IsTargetExtension(string extension)
        {
            return extensions.Contains(extension);
        }

        public static bool IsTargetPattern(string onlyFilename)
        {
            Match match1 = MatchRule1.Match(onlyFilename);
            Match match2 = MatchRule2.Match(onlyFilename);
            Match matchSCAM = MatchRuleSCAM.Match(onlyFilename);
            return (match1.Success || match2.Success || matchSCAM.Success);
        }

        public static string GetNewName(string onlyFilename)
        {
            Match match1 = MatchRule1.Match(onlyFilename);
            Match match2 = MatchRule2.Match(onlyFilename);
            Match matchSCAM = MatchRuleSCAM.Match(onlyFilename);
            string returnValue = string.Empty;

            if (match1.Success)
            {
                Match match = match1;
                returnValue = string.Format("{0}-{1}-{2} {3}.{4}.{5} {6}",
                    match.Groups["YYYY"].Value, match.Groups["MM"].Value, match.Groups["DD"].Value,
                    match.Groups["hh"].Value, match.Groups["mm"].Value, match.Groups["ss"].Value,
                    match.Groups["seq"].Value);
            }
            else if (match2.Success)
            {
                Match match = match2;
                returnValue = string.Format("{0}-{1}-{2} {3}.{4}.{5}",
                    match.Groups["YYYY"].Value, match.Groups["MM"].Value, match.Groups["DD"].Value,
                    match.Groups["hh"].Value, match.Groups["mm"].Value, match.Groups["ss"].Value);
                return returnValue;
            }
            else if (matchSCAM.Success)
            {
                Match match = matchSCAM;
                returnValue = string.Format("{0}-{1}-{2} {3}.{4}.{5}.{6:000}",
                    DateTime.Now.Year, match.Groups["MM"].Value, match.Groups["DD"].Value,
                    match.Groups["hh"].Value, match.Groups["mm"].Value, match.Groups["ss"].Value,
                    Int32.Parse(match.Groups["fff"].Value));
                return returnValue;
            }
            else
            {
                throw new ArgumentException("이름이 잘못되었습니다.");
            }

            return returnValue;
        }

    }
}
