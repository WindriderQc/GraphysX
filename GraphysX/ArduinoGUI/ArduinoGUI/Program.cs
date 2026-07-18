using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace GraphysX
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



            //	system("PAUSE");   // requiert d'appuyer sur un touche dans la console pour terminer l'application. permet de voir les dernier msg sur la console
            //	cin.ignore(); // // requiert d'appuyer sur Enter dans la console pour terminer l'application. permet de voir les dernier msg sur la console
        }
    }
}
