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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace forWPF
{
    /// <summary>
    /// Interaction logic for toSharp.xaml
    /// </summary>
    public partial class toSharp : Window
    {
        public toSharp()
        {
            InitializeComponent();
        }

        private void Rectangle_MouseDown(object sender, MouseButtonEventArgs e)
        {
            myRect.Fill = Brushes.Black;
        }

      

        private void canvasDrawingArea_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Shape shapeToRender = new Ellipse() { Fill = Brushes.Black, Height = 35, Width = 35 };

            Canvas.SetLeft(shapeToRender, e.GetPosition(canvasDrawingArea).X - 35 / 2);
            Canvas.SetTop(shapeToRender, e.GetPosition(canvasDrawingArea).Y - 35 / 2);
            canvasDrawingArea.Children.Add(shapeToRender);
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string path = Environment.CurrentDirectory;
            //BitmapImage im = new BitmapImage(new Uri(string.Format(@"{0}\Image\003.gif", path)));
            BitmapImage im = new BitmapImage(new Uri(@"\Image\003.gif",UriKind.Relative));
            imgHolder.Source = im;
        }

        private bool isSpin = false;

        
        private void aBtn_MouseEnter(object sender, MouseEventArgs e)
        {
            // Demo for Animation
            if (!isSpin)
            {
                isSpin = true;
                //
                DoubleAnimation da = new DoubleAnimation();
                //
                da.Completed += (s, ee) => { isSpin = false; };
                da.From = 0;da.To = 360;
                // 
                //RotateTransform rt = new RotateTransform();
                //aBtn.RenderTransform = rt;
                //rt.BeginAnimation(RotateTransform.AngleProperty, da);

                ScaleTransform st = new ScaleTransform();
                aBtn.RenderTransform = st;
                aBtn.BeginAnimation(Button.HeightProperty, da);

            }
        }

        private void btnClickMe_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("you click me");

        }

        private void Ellipse_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.Title = "new";
            e.Handled = true;
        }

        private void Ellipse_MouseDown_1(object sender, MouseButtonEventArgs e)
        {
            this.Title = "old";
            e.Handled = true;
        }
        string dataToShow = "";
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            dataToShow = "";
            BuildLogicalTree(0,this);
            MessageBox.Show(dataToShow);
        }
        void BuildLogicalTree(int depth, object obj)
        {
            dataToShow += new string(' ', depth) + obj.GetType().Name + "\n";
            if(!(obj is DependencyObject))
            {
                return;
            }
            foreach (object child in LogicalTreeHelper.GetChildren(obj as DependencyObject))
                BuildLogicalTree(depth + 5, child);
        }
    }
}
