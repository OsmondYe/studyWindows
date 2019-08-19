using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Excel =  Microsoft.Office.Interop.Excel;
using Office = Microsoft.Office.Core;

// This is the code for your desktop app.
// Press Ctrl+F5 (or go to Debug > Start Without Debugging) to run your app.

namespace WinFormForOffice
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();

            InitWaterMark(this.Handle);
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            // Click on the link below to continue learning how to build a desktop app using WinForms!
            System.Diagnostics.Process.Start("http://aka.ms/dotnet-get-started-desktop");

        }

        
        private void button1_Click(object sender, EventArgs e)
        {
            //t_1();
            //t_2();
            t_appAggregation();
            //t_usingApp();

        }

        private void t_usingApp()
        {
            Excel.Application app = new Excel.Application();
            app.Visible = true;

            Excel.Workbook wb = app.Workbooks.Add();           
            
        }

        private void t_1()
        {
            // Notice
            // using System.Runtime.InteropServices;
            // using Excel =  Microsoft.Office.Interop.Excel;
            Excel.Application ExcelApp = (Excel.Application)Marshal.GetActiveObject("Excel.Application");
            MessageBox.Show(ExcelApp.Version);
        }

        private void t_2()
        {
            Excel.Application app = new Excel.Application();
            app.Visible = true;
            app.Caption = "new caption";

            app.Workbooks.Add();
        }

        private void t_appAggregation()
        {
            Excel.Application app = new Excel.Application();

            string result = "";
            listBox1.Items.Add("all dialogs:" + app.Dialogs.Count);
            //
            //  all plugins 
            //
            listBox1.Items.Add("[all-addins]:"+app.AddIns.Count);
            foreach(Excel.AddIn addin in app.AddIns)
            {
                result = addin.Name + "\t" +
                    addin.Installed + "\t" + 
                    addin.FullName + "\t" +
                listBox1.Items.Add(result);
            }

            listBox1.Items.Add("[com-addins]:" + app.COMAddIns);
            foreach (Office.COMAddIn addin in app.COMAddIns)
            {
                result = addin.Description + "\t" +
                    addin.Connect + "\t" +
                    addin.Guid + "\t" +
                listBox1.Items.Add(result);
            }


            //
            // all commandbars
            //
            listBox1.Items.Add("[all cmdbars]:"+app.CommandBars.Count);
            foreach (Office.CommandBar item in app.CommandBars)
            {
                result = item.Name + "\t" + item.Type;
                listBox1.Items.Add(result);
            }

            app.Quit();
        }



        #region CppDLL for watermark
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "InitWaterMark")]
        static private extern void InitWaterMark(IntPtr hwnd);

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "UpdateWaterMark")]
        static private extern void UpdateWaterMark(IntPtr hwnd);


        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "DeleteWaterMark")]
        static private extern void DeleteWaterMark();

        #endregion
    }
}
