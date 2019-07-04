using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace OyeNS
{
    partial class CompileXamlWindow : Window
    {
        public CompileXamlWindow()
        {
            InitializeComponent();

            // add lsbxo items
            foreach (var prop in typeof(Brushes).GetProperties())
            {
                lstbox.Items.Add(prop.Name);
            }
        }

        //delegate void RoutedEventHandler(object sender, RoutedEventArgs e);
        void ButtonOnClick(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("ButtonOnClick");
        }
        //delegate void SelectionChangedEventHandler(object sender, SelectionChangedEventArgs e);
        void ListBoxOnSeleciont(object sender, SelectionChangedEventArgs e)
        {
            //MessageBox.Show("ListBoxOnSeleciont");
            ListBox l = sender as ListBox;
            var str = l.SelectedItem as string;
            PropertyInfo prop = typeof(Brushes).GetProperty(str);
        }
    }

    public partial class CenteredButton : Button
    {
        public CenteredButton()
        {
            InitializeComponent();
        }
    }

}
