using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.IO;
using System.Diagnostics;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Windows.Documents;
using System.Xml;
using System.Windows.Markup;
using OyeNS;

namespace studyWPF
{
    // Window : ContentControl : Control : FrameElement : UIElement : Visual : DependencyObject : DispatcherObject
    class MyMainWind : Window
    {
        //[STAThread]
        //static void Main()
        //{
        //    Application app = new Application();
        //    app.ShutdownMode = ShutdownMode.OnMainWindowClose;
        //    app.Run(new MyMainWind());
        //}

        MyMainWind()
        {

            //
            // From Window
            //
            Title = "hello world";
            //SizeToContent = SizeToContent.WidthAndHeight;

            //
            //  From ContentControl
            //
            //Content = "hello china,this is from hangzhou";
           // Content = Math.PI;
            //
            //  From Control
            //
            BorderBrush = Brushes.SeaGreen;
            BorderThickness = new Thickness(10,10,10,10);
            Foreground = Brushes.SaddleBrown;
            Background = Brushes.Yellow;
            FontFamily = new FontFamily("Times New Roman");
            FontSize = 16;
            FontStyle = FontStyles.Italic;
            FontWeight = FontWeights.Heavy;


            //
            // From UIElement
            //
            Opacity = 0.8;



            //ShowImage();
            //showTextBlock();
            //ShowButton();
            //ShowImageButton();
            //ShowCommandButton();
            //ShowToggleButton();
            //ShowToggleButtonWithDataBinding();

            //
            //OnNavigate();


            //
            //UsingStackPannel();
            //UsingTuneTheRadio();
            //UsingFileExplore();

            //
            // using Dock
            //
            //UsingDockPanel();
            //UsingGrid();

            //UsingColorScroll();

            //
            // for canvas
            //
            //PaintTheButton();
            //ForPlayJeuDeTacquin();

            //
            //  for Dependency Property
            //
            //forDP1();



            //
            //  XML
            //
            //forLoadEmbeddedXaml();
            //forLoadFromResource();
            ShowCompileXamlWindow();
        }

        void ShowCompileXamlWindow()
        {
            CompileXamlWindow w = new CompileXamlWindow();
            w.Show();
        }

        void forLoadFromResource()
        {
            Title = "forLoadFromResource";
            Uri uri = new Uri("pack://application:,,,/XMLFile1.xml");
            Stream stream = Application.GetResourceStream(uri).Stream;
            FrameworkElement el = XamlReader.Load(stream) as FrameworkElement;
            Content = el;
            // find button
            Button btn = el.FindName("MyButton") as Button;
            btn.Click += (ee, ss) =>
            {
                MessageBox.Show("you cliked me");
            };

        }

        void forLoadEmbeddedXaml()
        {
            Title = "Load Embedded Xaml";

            string strXaml =
                "<Button xmlns='http://schemas.microsoft.com/" +
                                      "winfx/2006/xaml/presentation'" +
                "        Foreground='LightSeaGreen' FontSize='24pt'>" +
                "    Click me!" +
                "</Button>";

            StringReader strreader = new StringReader(strXaml);
            XmlTextReader xmlreader = new XmlTextReader(strreader);
            object obj = XamlReader.Load(xmlreader);

            Content = obj;
        }
        private void forDP1()
        {
            Title = "Set FontSize Property";
            SizeToContent = SizeToContent.WidthAndHeight;
            ResizeMode = ResizeMode.CanMinimize;
            FontSize = 32;
            double[] fntsizes = { 8, 16, 32 };

            // Create Grid panel.
            Grid grid = new Grid();
            Content = grid;

            // Define row and columns.
            for (int i = 0; i < 2; i++)
            {
                RowDefinition row = new RowDefinition();
                row.Height = GridLength.Auto;
                grid.RowDefinitions.Add(row);
            }
            for (int i = 0; i < fntsizes.Length; i++)
            {
                ColumnDefinition col = new ColumnDefinition();
                col.Width = GridLength.Auto;
                grid.ColumnDefinitions.Add(col);
            }

            // Create six buttons.
            for (int i = 0; i < fntsizes.Length; i++)
            {
                Button btn = new Button();
                btn.Content = new TextBlock(
                    new Run("Set window FontSize to " + fntsizes[i]));
                btn.Tag = fntsizes[i];
                btn.HorizontalAlignment = HorizontalAlignment.Center;
                btn.VerticalAlignment = VerticalAlignment.Center;
                btn.Click += WindowFontSizeOnClick;
                grid.Children.Add(btn);
                Grid.SetRow(btn, 0);
                Grid.SetColumn(btn, i);

                btn = new Button();
                btn.Content = new TextBlock(
                    new Run("Set button FontSize to " + fntsizes[i]));
                btn.Tag = fntsizes[i];
                btn.HorizontalAlignment = HorizontalAlignment.Center;
                btn.VerticalAlignment = VerticalAlignment.Center;
                btn.Click += ButtonFontSizeOnClick;
                grid.Children.Add(btn);
                Grid.SetRow(btn, 1);
                Grid.SetColumn(btn, i);
            }
        }

        void WindowFontSizeOnClick(object sender, RoutedEventArgs args)
        {
            Button btn = args.Source as Button;
            FontSize = (double)btn.Tag;
        }
        void ButtonFontSizeOnClick(object sender, RoutedEventArgs args)
        {
            Button btn = args.Source as Button;
            btn.FontSize = (double)btn.Tag;
        }

        private void ForPlayJeuDeTacquin()
        {
            PlayJeuDeTacquin w = new PlayJeuDeTacquin();
            w.Show();
        }

        void PaintTheButton()
        {
            Title = "Paint the Button";

            // Create the Button as content of the window.
            Button btn = new Button();
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.VerticalAlignment = VerticalAlignment.Center;
            Content = btn;

            // Create the Canvas as content of the button.
            Canvas canv = new Canvas();
            canv.Width = 144;
            canv.Height = 144;
            btn.Content = canv;

            // Create Rectangle as child of canvas.
            Rectangle rect = new Rectangle();
            rect.Width = canv.Width;
            rect.Height = canv.Height;
            rect.RadiusX = 24;
            rect.RadiusY = 24;
            rect.Fill = Brushes.Blue;
            canv.Children.Add(rect);
            Canvas.SetLeft(rect, 0);
            Canvas.SetRight(rect, 0);

            // Create Polygon as child of canvas.
            Polygon poly = new Polygon();
            poly.Fill = Brushes.Yellow;
            poly.Points = new PointCollection();

            for (int i = 0; i < 5; i++)
            {
                double angle = i * 4 * Math.PI / 5;
                Point pt = new Point(48 * Math.Sin(angle),
                                    -48 * Math.Cos(angle));
                poly.Points.Add(pt);
            }
            canv.Children.Add(poly);
            Canvas.SetLeft(poly, canv.Width / 2);
            Canvas.SetTop(poly, canv.Height / 2);
        }

        private void UsingColorScroll()
        {
            ColorScroll w = new ColorScroll();
            w.Show();
        }

        private void UsingGrid()
        {
            var g = new Grid();
            Content = g;
            g.ShowGridLines = true;
            //
            RowDefinition rd = new RowDefinition();
            rd.Height = GridLength.Auto;
            g.RowDefinitions.Add(rd);
            //
            rd = new RowDefinition();
            rd.Height = new GridLength(33, GridUnitType.Star);
            g.RowDefinitions.Add(rd);
            //
            rd = new RowDefinition();
            rd.Height = new GridLength(150);
            g.RowDefinitions.Add(rd);
            //
            rd = new RowDefinition();
            rd.Height = new GridLength(67, GridUnitType.Star);
            g.RowDefinitions.Add(rd);

            
            for (int i = 0; i < 4; i++)
            {
                var btn = new Button();
                btn.Content = "button" + (i + 1);
                g.Children.Add(btn);
                Grid.SetRow(btn, i);
            }

        }

        private void UsingDockPanel()
        {
            var d = new DockPanel();
            d.Margin = new Thickness(10);
            Content = d;

            for (int i = 0; i < 10; i++)
            {
                var btn = new Button(); btn.Content = "button"+(i+1);
                //btn.Width = btn.Height = 100;
                d.LastChildFill = false;
                d.Children.Add(btn);
                //btn.SetValue(DockPanel.DockProperty, (Dock)(i % 4));

                DockPanel.SetDock(btn, (Dock)(i % 4));
            }

            //DockPanel.SetDock(btn, Dock.Left);
        }

        private void UsingFileExplore()
        {
            var s = new ScrollViewer();
            Content = s;

            var p = new WrapPanel();
            s.Content = p;

            p.Children.Add(new FileSystemInfoButton());
        }

        private void UsingTuneTheRadio()
        {
            GroupBox gb = new GroupBox();
            gb.Header = "window style";
            gb.Margin = new Thickness(10);
            gb.Padding = gb.Margin;
            this.Content = gb;

            StackPanel sp = new StackPanel();
            sp.Orientation = Orientation.Horizontal;
            gb.Content = sp;


            sp.Children.Add(NewRbtn("No border", WindowStyle.None));
            sp.Children.Add(NewRbtn("single border", WindowStyle.SingleBorderWindow));
            sp.Children.Add(NewRbtn("3d border", WindowStyle.ThreeDBorderWindow));
            sp.Children.Add(NewRbtn("tool", WindowStyle.ToolWindow));


            AddHandler(RadioButton.CheckedEvent, new RoutedEventHandler(
                (ss,ee)=>
                {
                    var rb = ee.Source as RadioButton;
                    this.WindowStyle = (WindowStyle)rb.Tag;
                }
                ));

        }

        RadioButton  NewRbtn(string t, WindowStyle ws)
        {
            var rb = new RadioButton();
            rb.Content = t;
            rb.Tag = ws;
            rb.Margin = new Thickness(5);
            rb.IsChecked = (ws == this.WindowStyle);
            return rb;

        }

        private void UsingStackPannel()
        {
            //ScrollViewer sv = new ScrollViewer();
            //sv.VerticalScrollBarVisibility = ScrollBarVisibility.Visible;
            //sv.HorizontalScrollBarVisibility = ScrollBarVisibility.Visible;

            Viewbox vb = new Viewbox();
            Content = vb;

            StackPanel sp = new StackPanel();
            sp.Margin = new Thickness(50);
            sp.Orientation = Orientation.Horizontal;

            vb.Child = sp;

            for(int i = 0; i < 10; i++)
            {
                StackPanel sp2 = new StackPanel();
                sp.Children.Add(sp2);
                for (int j = 0; j < 10; j++)
                {
                    Button btn = new Button();
                    btn.Content = "btn_" + i +" "+j ;
                    btn.Margin = new Thickness(5);
                    sp2.Children.Add(btn);
                }
            }
        }

        private void OnNavigate()
        {
            var frm = new Frame();
            Content = frm;
            Loaded += (ss, ee) =>
            {
                var dlg = new UriDialog();
                dlg.Owner = this;
                dlg.Text = "http://";
                dlg.ShowDialog();
                frm.Source = new Uri(dlg.Text);
            };
        }

        private void ShowToggleButtonWithDataBinding()
        {
            ToggleButton btn = new ToggleButton();
            btn.Content = "make _topmost";

            btn.DataContext = this;
            // 如果 btn是checked,就call btn.DataContext.Topmost=True;
            btn.SetBinding(ToggleButton.IsCheckedProperty,"Topmost");
            // or 
            //Binding bind = new Binding("Topmost");
            //bind.Source = this;
            //btn.SetBinding(ToggleButton.IsCheckedProperty, bind);

            ToolTip tip = new ToolTip();
            tip.Content = "Toggle the button on to make the window topmost on the destop";
            btn.ToolTip = tip;

            this.Content = btn;
        }

        private void ShowToggleButton()
        {
            ToggleButton btn = new ToggleButton();
            btn.Content = "toggle resizeable";
            btn.Click += (ss, ee) =>
            {
                ToggleButton b = ss as ToggleButton;
                ResizeMode = (bool)b.IsChecked ? ResizeMode.CanResize : ResizeMode.NoResize;
            };

            Content = btn;
        }

        private void ShowCommandButton()
        {
            // btn assoc CMD_Paste, and Paste bingding Excuthe_Method
            // so btn is bind to the Execute

            Button btn = new Button();
            btn.Command = ApplicationCommands.Paste;
            btn.Content = ApplicationCommands.Paste.Text;

            Content = btn;

            CommandBindings.Add(new CommandBinding(ApplicationCommands.Paste, PasteExecute, PasteCanExecute));
        }

        void PasteExecute(Object sender, ExecutedRoutedEventArgs args)
        {
            Title = Clipboard.GetText();
        }

        void PasteCanExecute(Object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = Clipboard.ContainsText();
        }


        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            Title = "Command the butn";
        }

        private void ShowImageButton()
        {
            Uri uri = new Uri(@"pack://application:,,,/myImage/pic1.jpg");
            BitmapImage bi = new BitmapImage(uri);
            Image im = new Image();
            im.Source = bi;
            im.Stretch = Stretch.Uniform;
            
            Button btn = new Button();

            btn.Content = im;

            Content = btn;

        }

        private void ShowButton()
        {
            Button btn = new Button();
            btn.Content = "_dlick me";
            btn.Click += Btn_Click;
            btn.Margin = new Thickness(10); // btn in window  , Margin is in Framelemnt class
            btn.Padding = new Thickness(40); // btn's content in btn  Padding is in Control class 
            btn.HorizontalContentAlignment = HorizontalAlignment.Right;
            btn.VerticalContentAlignment = VerticalAlignment.Top;
            Content = btn;
        }

        private void Btn_Click(object sender, RoutedEventArgs e)
        {
            Content = "you clicked me";
        }

        void ShowImage()
        {
            Uri uri = new Uri(@"https://www.baidu.com/img/baidu_jgylogo3.gif");
            BitmapImage bi = new BitmapImage(uri);
            Image im = new Image();
            im.Source = bi;
            Content = im;
        }

        void showTextBlock()
        {
            TextBlock tb = new TextBlock();
            tb.Inlines.Add("hello world");
            tb.Inlines.Add("hello Asia");
            tb.Inlines.Add("hello China");

            Content = tb;
        }

    }


    class UriDialog: Window
    {
        TextBox textBox;

        public UriDialog()
        {
            Title = "enter a uri";
            SizeToContent = SizeToContent.WidthAndHeight;
            WindowStyle = WindowStyle.ToolWindow;

            textBox = new TextBox();
            textBox.Margin = new Thickness(48);
            Content = textBox;
            textBox.Focus();

            

        }


        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            if (e.Key == Key.Enter)
            {
                Close();
            }
        }


        public string Text
        {
            set
            {
                textBox.Text = value;
                textBox.SelectionStart = textBox.Text.Length;
            }
            get
            {
                return textBox.Text;
            }
        }

    }

    public class FileSystemInfoButton : Button
    {
        FileSystemInfo info;
        public FileSystemInfoButton()
            : this(new DirectoryInfo(
                Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)))
        {
        }
        public FileSystemInfoButton(FileSystemInfo info)
        {
            this.info = info;
            Content = info.Name;
            if (info is DirectoryInfo)
                FontWeight = FontWeights.Bold;
            Margin = new Thickness(10);
        }
        public FileSystemInfoButton(FileSystemInfo info, string str)
            : this(info)
        {
            Content = str;
        }
        protected override void OnClick()
        {
            if (info is FileInfo)
                Process.Start(info.FullName);
            else if (info is DirectoryInfo)
            {
                DirectoryInfo dir = info as DirectoryInfo;
                Application.Current.MainWindow.Title = dir.FullName;
                Panel pnl = Parent as Panel;
                pnl.Children.Clear();
                if (dir.Parent != null)
                    pnl.Children.Add(new FileSystemInfoButton(dir.Parent, ".."));
                foreach (FileSystemInfo inf in dir.GetFileSystemInfos())
                {
                    pnl.Children.Add(new FileSystemInfoButton(inf));
                }
            }
            base.OnClick();
        }
    }


    class ColorScroll : Window
    {
        ScrollBar[] scrolls = new ScrollBar[3];
        TextBlock[] txtValue = new TextBlock[3];
        Panel pnlColor;

        public ColorScroll()
        {
            Title = "Color Scroll";
            Width = 500;
            Height = 300;

            // GridMain contains a vertical splitter.
            Grid gridMain = new Grid();
            Content = gridMain;

            // GridMain column definitions.
            ColumnDefinition coldef = new ColumnDefinition();
            coldef.Width = new GridLength(200, GridUnitType.Pixel);
            gridMain.ColumnDefinitions.Add(coldef);

            coldef = new ColumnDefinition();
            coldef.Width = GridLength.Auto;
            gridMain.ColumnDefinitions.Add(coldef);

            coldef = new ColumnDefinition();
            coldef.Width = new GridLength(100, GridUnitType.Star);
            gridMain.ColumnDefinitions.Add(coldef);

            // Vertical splitter.
            GridSplitter split = new GridSplitter();
            split.HorizontalAlignment = HorizontalAlignment.Center;
            split.VerticalAlignment = VerticalAlignment.Stretch;
            split.Width = 6;
            gridMain.Children.Add(split);
            Grid.SetRow(split, 0);
            Grid.SetColumn(split, 1);

            // Panel on right side of splitter to display color
            pnlColor = new StackPanel();
            pnlColor.Background = new SolidColorBrush(SystemColors.WindowColor);
            gridMain.Children.Add(pnlColor);
            Grid.SetRow(pnlColor, 0);
            Grid.SetColumn(pnlColor, 2);

            // Secondary grid at left of splitter
            Grid grid = new Grid();
            gridMain.Children.Add(grid);
            Grid.SetRow(grid, 0);
            Grid.SetColumn(grid, 0);

            // Three rows for label, scroll, and label.
            RowDefinition rowdef = new RowDefinition();
            rowdef.Height = GridLength.Auto;
            grid.RowDefinitions.Add(rowdef);

            rowdef = new RowDefinition();
            rowdef.Height = new GridLength(100, GridUnitType.Star);
            grid.RowDefinitions.Add(rowdef);

            rowdef = new RowDefinition();
            rowdef.Height = GridLength.Auto;
            grid.RowDefinitions.Add(rowdef);

            // Three columns for Red, Green, and Blue.
            for (int i = 0; i < 3; i++)
            {
                coldef = new ColumnDefinition();
                coldef.Width = new GridLength(33, GridUnitType.Star);
                grid.ColumnDefinitions.Add(coldef);
            }

            for (int i = 0; i < 3; i++)
            {
                Label lbl = new Label();
                lbl.Content = new string[] { "Red", "Green", "Blue" }[i];
                lbl.HorizontalAlignment = HorizontalAlignment.Center;
                grid.Children.Add(lbl);
                Grid.SetRow(lbl, 0);
                Grid.SetColumn(lbl, i);

                scrolls[i] = new ScrollBar();
                scrolls[i].Focusable = true;
                scrolls[i].Orientation = Orientation.Vertical;
                scrolls[i].Minimum = 0;
                scrolls[i].Maximum = 255;
                scrolls[i].SmallChange = 1;
                scrolls[i].LargeChange = 16;
                scrolls[i].ValueChanged += ScrollOnValueChanged;
                grid.Children.Add(scrolls[i]);
                Grid.SetRow(scrolls[i], 1);
                Grid.SetColumn(scrolls[i], i);

                txtValue[i] = new TextBlock();
                txtValue[i].TextAlignment = TextAlignment.Center;
                txtValue[i].HorizontalAlignment = HorizontalAlignment.Center;
                txtValue[i].Margin = new Thickness(5);
                grid.Children.Add(txtValue[i]);
                Grid.SetRow(txtValue[i], 2);
                Grid.SetColumn(txtValue[i], i);
            }

            // Initialize scroll bars.
            Color clr = (pnlColor.Background as SolidColorBrush).Color;
            scrolls[0].Value = clr.R;
            scrolls[1].Value = clr.G;
            scrolls[2].Value = clr.B;

            // Set initial focus.
            scrolls[0].Focus();
        }
        void ScrollOnValueChanged(object sender, RoutedEventArgs args)
        {
            ScrollBar scroll = sender as ScrollBar;
            Panel pnl = scroll.Parent as Panel;
            TextBlock txt = pnl.Children[1 +
                                    pnl.Children.IndexOf(scroll)] as TextBlock;

            txt.Text = String.Format("{0}\n0x{0:X2}", (int)scroll.Value);
            pnlColor.Background =
                new SolidColorBrush(
                    Color.FromRgb((byte)scrolls[0].Value,
                                  (byte)scrolls[1].Value, (byte)scrolls[2].Value));
        }
    }


    public class PlayJeuDeTacquin : Window
    {
        const int NumberRows = 4;
        const int NumberCols = 4;

        UniformGrid unigrid;
        int xEmpty, yEmpty, iCounter;
        Key[] keys = { Key.Left, Key.Right, Key.Up, Key.Down };
        Random rand;
        UIElement elEmptySpare = new Empty();

        
        public PlayJeuDeTacquin()
        {
            Title = "Jeu de Tacquin";
            SizeToContent = SizeToContent.WidthAndHeight;
            ResizeMode = ResizeMode.CanMinimize;
            Background = SystemColors.ControlBrush;

            // Create StackPanel as content of window.
            StackPanel stack = new StackPanel();
            Content = stack;

            // Create Button at top of window.
            Button btn = new Button();
            btn.Content = "_Scramble";
            btn.Margin = new Thickness(10);
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.Click += ScrambleOnClick;
            stack.Children.Add(btn);

            // Create Border for aesthetic purposes.
            Border bord = new Border();
            bord.BorderBrush = SystemColors.ControlDarkDarkBrush;
            bord.BorderThickness = new Thickness(1);
            stack.Children.Add(bord);

            // Create Unigrid as Child of Border.
            unigrid = new UniformGrid();
            unigrid.Rows = NumberRows;
            unigrid.Columns = NumberCols;
            bord.Child = unigrid;

            // Create Tile objects to fill all but one cell.
            for (int i = 0; i < NumberRows * NumberCols - 1; i++)
            {
                Tile tile = new Tile();
                tile.Text = (i + 1).ToString();
                tile.MouseLeftButtonDown += TileOnMouseLeftButtonDown;
                unigrid.Children.Add(tile);
            }
            // Create Empty object to fill the last cell.
            unigrid.Children.Add(new Empty());
            xEmpty = NumberCols - 1;
            yEmpty = NumberRows - 1;
        }
        void TileOnMouseLeftButtonDown(object sender, MouseButtonEventArgs args)
        {
            Tile tile = sender as Tile;

            int iMove = unigrid.Children.IndexOf(tile);
            int xMove = iMove % NumberCols;
            int yMove = iMove / NumberCols;

            if (xMove == xEmpty)
                while (yMove != yEmpty)
                    MoveTile(xMove, yEmpty + (yMove - yEmpty) /
                                        Math.Abs(yMove - yEmpty));
            if (yMove == yEmpty)
                while (xMove != xEmpty)
                    MoveTile(xEmpty + (xMove - xEmpty) /
                                        Math.Abs(xMove - xEmpty), yMove);
        }
        protected override void OnKeyDown(KeyEventArgs args)
        {
            base.OnKeyDown(args);

            switch (args.Key)
            {
                case Key.Right: MoveTile(xEmpty - 1, yEmpty); break;
                case Key.Left: MoveTile(xEmpty + 1, yEmpty); break;
                case Key.Down: MoveTile(xEmpty, yEmpty - 1); break;
                case Key.Up: MoveTile(xEmpty, yEmpty + 1); break;
            }
        }
        void ScrambleOnClick(object sender, RoutedEventArgs args)
        {
            rand = new Random();
            iCounter = 16 * NumberCols * NumberRows;

            DispatcherTimer tmr = new DispatcherTimer();
            tmr.Interval = TimeSpan.FromMilliseconds(10);
            tmr.Tick += TimerOnTick;
            tmr.Start();
        }
        void TimerOnTick(object sender, EventArgs args)
        {
            for (int i = 0; i < 5; i++)
            {
                MoveTile(xEmpty, yEmpty + rand.Next(3) - 1);
                MoveTile(xEmpty + rand.Next(3) - 1, yEmpty);
            }
            if (0 == iCounter--)
                (sender as DispatcherTimer).Stop();
        }
        void MoveTile(int xTile, int yTile)
        {
            if ((xTile == xEmpty && yTile == yEmpty) ||
                xTile < 0 || xTile >= NumberCols ||
                yTile < 0 || yTile >= NumberRows)
                return;

            int iTile = NumberCols * yTile + xTile;
            int iEmpty = NumberCols * yEmpty + xEmpty;

            UIElement elTile = unigrid.Children[iTile];
            UIElement elEmpty = unigrid.Children[iEmpty];

            unigrid.Children.RemoveAt(iTile);
            unigrid.Children.Insert(iTile, elEmptySpare);
            unigrid.Children.RemoveAt(iEmpty);
            unigrid.Children.Insert(iEmpty, elTile);

            xEmpty = xTile;
            yEmpty = yTile;
            elEmptySpare = elEmpty;
        }
    }


    class Empty : System.Windows.FrameworkElement
    {
    }

    public class Tile : Canvas
    {
        const int SIZE = 64;    // 2/3 inch
        const int BORD = 6;     // 1/16 inch
        TextBlock txtblk;

        public Tile()
        {
            Width = SIZE;
            Height = SIZE;

            // Upper-left shadowed border.
            Polygon poly = new Polygon();
            poly.Points = new PointCollection(new Point[]
                {
                    new Point(0, 0), new Point(SIZE, 0),
                    new Point(SIZE-BORD, BORD),
                    new Point(BORD, BORD),
                    new Point(BORD, SIZE-BORD), new Point(0, SIZE)
                });
            poly.Fill = SystemColors.ControlLightLightBrush;
            Children.Add(poly);

            // Lower-right shadowed border.
            poly = new Polygon();
            poly.Points = new PointCollection(new Point[]
                {
                    new Point(SIZE, SIZE), new Point(SIZE, 0),
                    new Point(SIZE-BORD, BORD),
                    new Point(SIZE-BORD, SIZE-BORD),
                    new Point(BORD, SIZE-BORD), new Point(0, SIZE)
                });
            poly.Fill = SystemColors.ControlDarkBrush;
            Children.Add(poly);

            // Host for centered text.
            Border bord = new Border();
            bord.Width = SIZE - 2 * BORD;
            bord.Height = SIZE - 2 * BORD;
            bord.Background = SystemColors.ControlBrush;
            Children.Add(bord);
            SetLeft(bord, BORD);
            SetTop(bord, BORD);

            // Display of text.
            txtblk = new TextBlock();
            txtblk.FontSize = 32;
            txtblk.Foreground = SystemColors.ControlTextBrush;
            txtblk.HorizontalAlignment = HorizontalAlignment.Center;
            txtblk.VerticalAlignment = VerticalAlignment.Center;
            bord.Child = txtblk;
        }
        // Public property to set text.
        public string Text
        {
            set { txtblk.Text = value; }
            get { return txtblk.Text; }
        }
    }

}
