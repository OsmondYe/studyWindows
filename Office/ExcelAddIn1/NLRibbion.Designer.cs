namespace ExcelAddIn1
{
    partial class NLRibbion : Microsoft.Office.Tools.Ribbon.RibbonBase
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        public NLRibbion()
            : base(Globals.Factory.GetRibbonFactory())
        {
            InitializeComponent();
        }

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.nlTab = this.Factory.CreateRibbonTab();
            this.group1 = this.Factory.CreateRibbonGroup();
            this.box1 = this.Factory.CreateRibbonBox();
            this.button3 = this.Factory.CreateRibbonButton();
            this.toggleButton1 = this.Factory.CreateRibbonToggleButton();
            this.separator1 = this.Factory.CreateRibbonSeparator();
            this.button1 = this.Factory.CreateRibbonButton();
            this.button2 = this.Factory.CreateRibbonButton();
            this.buttonGroup1 = this.Factory.CreateRibbonButtonGroup();
            this.button4 = this.Factory.CreateRibbonButton();
            this.button5 = this.Factory.CreateRibbonButton();
            this.button6 = this.Factory.CreateRibbonButton();
            this.button7 = this.Factory.CreateRibbonButton();
            this.button8 = this.Factory.CreateRibbonButton();
            this.tab1 = this.Factory.CreateRibbonTab();
            this.nlTab.SuspendLayout();
            this.group1.SuspendLayout();
            this.box1.SuspendLayout();
            this.buttonGroup1.SuspendLayout();
            this.tab1.SuspendLayout();
            this.SuspendLayout();
            // 
            // nlTab
            // 
            this.nlTab.Groups.Add(this.group1);
            this.nlTab.Label = "Nextlabs";
            this.nlTab.Name = "nlTab";
            // 
            // group1
            // 
            this.group1.Items.Add(this.box1);
            this.group1.Items.Add(this.toggleButton1);
            this.group1.Items.Add(this.separator1);
            this.group1.Items.Add(this.button1);
            this.group1.Items.Add(this.button2);
            this.group1.Items.Add(this.buttonGroup1);
            this.group1.Label = "group1";
            this.group1.Name = "group1";
            // 
            // box1
            // 
            this.box1.Items.Add(this.button3);
            this.box1.Name = "box1";
            // 
            // button3
            // 
            this.button3.Label = "button3";
            this.button3.Name = "button3";
            // 
            // toggleButton1
            // 
            this.toggleButton1.Label = "toggleButton1";
            this.toggleButton1.Name = "toggleButton1";
            // 
            // separator1
            // 
            this.separator1.Name = "separator1";
            // 
            // button1
            // 
            this.button1.ControlSize = Microsoft.Office.Core.RibbonControlSize.RibbonControlSizeLarge;
            this.button1.Label = "button1";
            this.button1.Name = "button1";
            this.button1.ShowImage = true;
            // 
            // button2
            // 
            this.button2.Label = "button2";
            this.button2.Name = "button2";
            // 
            // buttonGroup1
            // 
            this.buttonGroup1.Items.Add(this.button4);
            this.buttonGroup1.Name = "buttonGroup1";
            // 
            // button4
            // 
            this.button4.Label = "button4";
            this.button4.Name = "button4";
            // 
            // button5
            // 
            this.button5.Label = "button5";
            this.button5.Name = "button5";
            this.button5.ShowImage = true;
            // 
            // button6
            // 
            this.button6.Label = "button6";
            this.button6.Name = "button6";
            this.button6.ShowImage = true;
            // 
            // button7
            // 
            this.button7.Label = "button7";
            this.button7.Name = "button7";
            this.button7.ShowImage = true;
            // 
            // button8
            // 
            this.button8.Label = "button8";
            this.button8.Name = "button8";
            this.button8.ShowImage = true;
            // 
            // tab1
            // 
            this.tab1.Label = "tab1";
            this.tab1.Name = "tab1";
            // 
            // NLRibbion
            // 
            this.Name = "NLRibbion";
            // 
            // NLRibbion.OfficeMenu
            // 
            this.OfficeMenu.Items.Add(this.button5);
            this.OfficeMenu.Items.Add(this.button6);
            this.OfficeMenu.Items.Add(this.button7);
            this.OfficeMenu.Items.Add(this.button8);
            this.RibbonType = "Microsoft.Excel.Workbook";
            this.StartFromScratch = true;
            this.Tabs.Add(this.nlTab);
            this.Tabs.Add(this.tab1);
            this.Load += new Microsoft.Office.Tools.Ribbon.RibbonUIEventHandler(this.Ribbon1_Load);
            this.nlTab.ResumeLayout(false);
            this.nlTab.PerformLayout();
            this.group1.ResumeLayout(false);
            this.group1.PerformLayout();
            this.box1.ResumeLayout(false);
            this.box1.PerformLayout();
            this.buttonGroup1.ResumeLayout(false);
            this.buttonGroup1.PerformLayout();
            this.tab1.ResumeLayout(false);
            this.tab1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        internal Microsoft.Office.Tools.Ribbon.RibbonTab nlTab;
        internal Microsoft.Office.Tools.Ribbon.RibbonGroup group1;
        internal Microsoft.Office.Tools.Ribbon.RibbonBox box1;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button3;
        internal Microsoft.Office.Tools.Ribbon.RibbonToggleButton toggleButton1;
        internal Microsoft.Office.Tools.Ribbon.RibbonSeparator separator1;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button1;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button2;
        internal Microsoft.Office.Tools.Ribbon.RibbonButtonGroup buttonGroup1;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button4;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button5;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button6;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button7;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button8;
        private Microsoft.Office.Tools.Ribbon.RibbonTab tab1;
    }

    partial class ThisRibbonCollection
    {
        internal NLRibbion Ribbon1
        {
            get { return this.GetRibbon<NLRibbion>(); }
        }
    }
}
