using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;

namespace OyeNS
{

    class Student : INotifyPropertyChanged
    {
        private  string name;
        private int age;

        public event PropertyChangedEventHandler PropertyChanged;
        public string Name
        {
            get => name;
            set
            {
                name = value;
                if (this.PropertyChanged != null)
                {
                    this.PropertyChanged.Invoke(this, new PropertyChangedEventArgs("Name"));
                }
            }
        }

        public int Age
        {
            get => age;
            set
            {
                age = value;
                if (this.PropertyChanged != null)
                {
                    this.PropertyChanged.Invoke(this, new PropertyChangedEventArgs("Age"));
                }
            }
        }
    }

    partial class CompileXamlWindow : Window
    {
        Student stu = new Student();
        public CompileXamlWindow()
        {
            InitializeComponent();

            // add lsbxo items
            foreach (var prop in typeof(Brushes).GetProperties())
            {
                lstbox.Items.Add(prop.Name);
            }

            // set binding
            Binding b = new Binding();
            b.Source = stu;
            b.Path = new PropertyPath("Name");

            BindingOperations.SetBinding(this.stuName, TextBox.TextProperty, b);


            //stuAge.SetBinding(TextBlock.TextProperty, new Binding("Age") { Source = stu });
            stuAge.SetBinding(TextBlock.TextProperty, 
                new Binding() { Path = new PropertyPath("Age"),Source=stu });

        }

        //delegate void RoutedEventHandler(object sender, RoutedEventArgs e);
        void ButtonOnClick(object sender, RoutedEventArgs e)
        {
            stu.Name += "name";
            stu.Age += 10;
            //MessageBox.Show("ButtonOnClick");
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
