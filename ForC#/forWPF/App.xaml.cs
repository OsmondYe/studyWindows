using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace forWPF
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private void Application_Exit(object sender, ExitEventArgs e)
        {
            //MessageBox.Show("on application exit");
            Console.Write("On application exit");
        }

        private void Application_Startup(object sender, StartupEventArgs e)
        {

        }

        private void Application_Activated(object sender, EventArgs e)
        {

        }
    }
}
