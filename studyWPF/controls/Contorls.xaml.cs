using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace studyWPF.controls
{
    /// <summary>
    /// Interaction logic for Contorls.xaml
    /// </summary>
    public partial class Contorls : Window
    {
        //
        private RoutedCommand CloseCmd = new RoutedCommand("Close", typeof(Contorls));
        public Contorls()
        {
            InitializeComponent();
            // cmd source is abtn

            this.abtn.Command = CloseCmd;
            CloseCmd.InputGestures.Add(new KeyGesture(Key.Escape));
            // cmd target
            this.abtn.CommandTarget = this;
            // binding
            CommandBinding cb = new CommandBinding();
            cb.Command = CloseCmd;
            cb.Executed += (ss, ee) =>
            {
                this.Close();
                ee.Handled = true;
            };
            this.CommandBindings.Add(cb);
        }
    }
}
