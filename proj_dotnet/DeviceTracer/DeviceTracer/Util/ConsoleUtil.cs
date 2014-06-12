using System;
using System.Diagnostics;
using System.Security.Principal;    // for WindowsIdentity, WindowsPrincipal

namespace DeviceTracer.Util
{
    public static class ConsoleUtil
    {
        /// <summary>
        /// 관리자 권한으로 프로그램이 동작하는지 판단
        /// </summary>
        /// <returns>bool: true/false</returns>
        public static bool IsRunAsAdmin()
        {
            WindowsIdentity id = WindowsIdentity.GetCurrent();
            WindowsPrincipal principal = new WindowsPrincipal(id);
            return principal.IsInRole(WindowsBuiltInRole.Administrator);
        }

        public static void RunAsAdmin(string fileName, string arguments)
        {
            // Launch itself as administrator
            ProcessStartInfo proc = new ProcessStartInfo
            {
                UseShellExecute = true,
                WorkingDirectory = Environment.CurrentDirectory,
                FileName = fileName,
                Arguments = arguments,
                Verb = "runas",
            };

            Process.Start(proc);
        }

    }
}
