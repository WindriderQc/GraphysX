using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace ArduinoGUI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string newPath = string.Concat(Environment.GetEnvironmentVariable("PATH"), ";", Properties.Settings.Default.AdditionalPaths);  // ******
            Environment.SetEnvironmentVariable("PATH", newPath, EnvironmentVariableTarget.Process);  //  *****   requis pour linker le path de la lib et dll native setté dans les param
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new ArduinoGUI());
        }
    }
}
