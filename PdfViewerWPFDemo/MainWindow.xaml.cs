using PdfViewerNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Forms.Integration;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace PdfViewerWPFDemo
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            var viewer = windowsHost.Child as PdfViewer;            
            if (viewer != null)
            {
                viewer.BackColor = System.Drawing.SystemColors.Control;
                viewer.BorderColor = System.Drawing.Color.FromArgb(203, 203, 203);                         
            }
        }

        private void mainWindow_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            var viewer = windowsHost.Child as PdfViewer;
            viewer.SetWPFParentSize((int)this.Width, (int)this.Height);
        }

        private void mainWindow_LocationChanged(object sender, EventArgs e)
        {
            var viewer = windowsHost.Child as PdfViewer;
            viewer.SetWPFParentLocation((int)this.Left, (int)this.Top);
        }

        private void mainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            var viewer = windowsHost.Child as PdfViewer;
            viewer.SetWPFParentSize((int)this.Width, (int)this.Height);
            viewer.SetWPFParentLocation((int)this.Left, (int)this.Top);
        }
    }    
}
