//-------------------------------------------------
// SetSpaceProperty.cs (c) 2006 by Charles Petzold
//-------------------------------------------------
using System;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.SetSpaceProperty
{
    public class SetSpaceProperty : SpaceWindow
    {
        //[STAThread]
        //public static void Main()
        //{
        //    Application app = new Application();
        //    app.Run(new SetSpaceProperty());
        //}
        public SetSpaceProperty()
        {
            Title = "Set Space Property";
            SizeToContent = SizeToContent.WidthAndHeight;
            ResizeMode = ResizeMode.CanMinimize;
            int[] iSpaces = { 0, 1, 2 };

            Grid grid = new Grid();
            Content = grid;

            for (int i = 0; i < 2; i++)
            {
                RowDefinition row = new RowDefinition();
                row.Height = GridLength.Auto;
                grid.RowDefinitions.Add(row);
            }
            for (int i = 0; i < iSpaces.Length; i++)
            {
                ColumnDefinition col = new ColumnDefinition();
                col.Width = GridLength.Auto;
                grid.ColumnDefinitions.Add(col);
            }
            for (int i = 0; i < iSpaces.Length; i++)
            {
                SpaceButton btn = new SpaceButton();
                btn.Text = "Set window Space to " + iSpaces[i];
                btn.Tag = iSpaces[i];
                btn.HorizontalAlignment = HorizontalAlignment.Center;
                btn.VerticalAlignment = VerticalAlignment.Center;
                btn.Click += WindowPropertyOnClick;
                grid.Children.Add(btn);
                Grid.SetRow(btn, 0);
                Grid.SetColumn(btn, i);

                btn = new SpaceButton();
                btn.Text = "Set button Space to " + iSpaces[i];
                btn.Tag = iSpaces[i];
                btn.HorizontalAlignment = HorizontalAlignment.Center;
                btn.VerticalAlignment = VerticalAlignment.Center;
                btn.Click += ButtonPropertyOnClick;
                grid.Children.Add(btn);
                Grid.SetRow(btn, 1);
                Grid.SetColumn(btn, i);
            }
        }
        void WindowPropertyOnClick(object sender, RoutedEventArgs args)
        {
            SpaceButton btn = args.Source as SpaceButton;
            Space = (int)btn.Tag;
        }
        void ButtonPropertyOnClick(object sender, RoutedEventArgs args)
        {
            SpaceButton btn = args.Source as SpaceButton;
            btn.Space = (int)btn.Tag;
        }
    }

    public class SpaceWindow : Window
    {
        // DependencyProperty and property.
        public static readonly DependencyProperty SpaceProperty;

        public int Space
        {
            set
            {
                SetValue(SpaceProperty, value);
            }
            get
            {
                return (int)GetValue(SpaceProperty);
            }
        }

        // Static constructor.
        static SpaceWindow()
        {
            // Define metadata.
            FrameworkPropertyMetadata metadata = new FrameworkPropertyMetadata();
            metadata.Inherits = true;

            // Add owner to SpaceProperty and override metadata.
            SpaceProperty =
                SpaceButton.SpaceProperty.AddOwner(typeof(SpaceWindow));
            SpaceProperty.OverrideMetadata(typeof(SpaceWindow), metadata);
        }
    }

    public class SpaceButton : Button
    {
        // A traditional .NET private field and public property.
        string txt;

        public string Text
        {
            set
            {
                txt = value;
                Content = SpaceOutText(txt);
            }
            get
            {
                return txt;
            }
        }

        // A DependencyProperty and public property.
        public static readonly DependencyProperty SpaceProperty;

        public int Space
        {
            set
            {
                SetValue(SpaceProperty, value);
            }
            get
            {
                return (int)GetValue(SpaceProperty);
            }
        }

        // Static constructor.
        static SpaceButton()
        {
            // Define the metadata.
            FrameworkPropertyMetadata metadata = new FrameworkPropertyMetadata();
            metadata.DefaultValue = 1;
            metadata.AffectsMeasure = true;
            metadata.Inherits = true;
            metadata.PropertyChangedCallback += OnSpacePropertyChanged;

            // Register the DependencyProperty.
            SpaceProperty =
                DependencyProperty.Register("Space", typeof(int),
                                            typeof(SpaceButton), metadata,
                                            ValidateSpaceValue);
        }

        // Callback method for value validation.
        static bool ValidateSpaceValue(object obj)
        {
            int i = (int)obj;
            return i >= 0;
        }

        // Callback method for property changed.
        static void OnSpacePropertyChanged(DependencyObject obj,
                                    DependencyPropertyChangedEventArgs args)
        {
            SpaceButton btn = obj as SpaceButton;
            btn.Content = btn.SpaceOutText(btn.txt);
        }

        // Method to insert spaces in the text.
        string SpaceOutText(string str)
        {
            if (str == null)
                return null;

            StringBuilder build = new StringBuilder();

            foreach (char ch in str)
                build.Append(ch + new string(' ', Space));

            return build.ToString();
        }
    }
}
