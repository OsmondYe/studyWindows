using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace showPDF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }


        private void OnBtn_SelectFile(object sender, RoutedEventArgs e)
        {
            // Initialize an OpenFileDialog
            OpenFileDialog openFileDialog = new OpenFileDialog();

            // Set filter and RestoreDirectory
            openFileDialog.RestoreDirectory = true;
            openFileDialog.Filter = "All documents(*.*)|*.*";

            bool? result = openFileDialog.ShowDialog();
            if (result == true)
            {
                if (openFileDialog.FileName.Length > 0)
                {
                    tbSelectFile.Text = openFileDialog.FileName;
                }
            }
        }

        private void OnBtn_View(object sender, RoutedEventArgs e)
        {
            string wordDocument = tbSelectFile.Text;
            if (string.IsNullOrEmpty(wordDocument) || !File.Exists(wordDocument))
            {
                MessageBox.Show("The file is invalid. Please select an existing file again.");
            }
            else
            {
                dv.Navigate(wordDocument);
            }
        }

    }

    

}
