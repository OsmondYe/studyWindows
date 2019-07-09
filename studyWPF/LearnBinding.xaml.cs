using System;
using System.Collections.Generic;
using System.Globalization;
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

namespace studyWPF
{


    /// <summary>
    /// Interaction logic for LearnBinding.xaml
    /// </summary>
    public partial class LearnBinding : Window
    {
        List<Student> l = new List<Student>();
        public LearnBinding()
        {
            InitializeComponent();

            l.Add(new Student { Id = 1, Age = 10, Name = "tom" });
            l.Add(new Student { Id = 2, Age = 13, Name = "kevin" });
            l.Add(new Student { Id = 3, Age = 16, Name = "julia" });
            l.Add(new Student { Id = 4, Age = 19, Name = "ana" });
            l.Add(new Student { Id = 5, Age = 20, Name = "bard" });


            this.lbStu.ItemsSource = l;
            this.lbStu.DisplayMemberPath = "Name";

            this.tbId.SetBinding(TextBox.TextProperty,
                new Binding() { Path = new PropertyPath("SelectedItem.Id"), Source = lbStu });


            // validation
            Binding b = new Binding("Value") { Source = this.s_num };
            b.UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged;
            var vvv = new RangeValidation();
            vvv.ValidatesOnTargetUpdated = true;            
            b.NotifyOnValidationError = true;
            b.ValidationRules.Add(vvv);
            b.Converter = new SlideIntDoubleConverter();
            
            this.tb_num.SetBinding(TextBox.TextProperty, b);
            // this.tb_num.AddHandler(Validation.ErrorEvent, new RoutedEventHandler(this.OnValidationError));

            this.aaaa.AddHandler(Button.ClickEvent, new RoutedEventHandler(this.ButtonClick));
        }


        void OnValidationError(object sender, RoutedEventArgs args)
        {
            this.tb_num.ToolTip = "Validation Failed";
        }

        private void ButtonClick(object sender, RoutedEventArgs args)
        {
            MessageBox.Show((args.OriginalSource as FrameworkElement).Name);
        }
    }

    class SlideIntDoubleConverter : IValueConverter
    {
        // source to targert
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            double c = (double)value;
            return c;
        }
        // targert to source
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value;
        }
    }

    class RangeValidation : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            double d = 0;
            if(double.TryParse(value.ToString(),out d))
            {
                if(d>=0 &&d<=100)
                return new ValidationResult(true, null);
            }
            return new ValidationResult(false, "Validation failed");
        }
    }

    class Student 
    {
        private string name;
        private int age;
        private int id;

        public string Name
        {
            get => name;
            set
            {
                name = value;
                
            }
        }

        public int Age
        {
            get => age;
            set
            {
                age = value;
                
            }
        }

        public int Id { get => id; set => id = value; }
    }
}
