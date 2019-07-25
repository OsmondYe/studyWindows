using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using Excel = Microsoft.Office.Interop.Excel;
using Office = Microsoft.Office.Core;
using Microsoft.Office.Tools.Excel;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ExcelAddIn1
{
    public partial class ThisAddIn
    {
        Ribbon ribbon = new Ribbon();
        string strOverlay = "Linux系统编程手册(上下册)";

        protected override Microsoft.Office.Core.IRibbonExtensibility CreateRibbonExtensibilityObject()
        {
            Debug.WriteLine(System.Reflection.MethodBase.GetCurrentMethod().Name);
            return ribbon;
        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
            Debug.WriteLine(System.Reflection.MethodBase.GetCurrentMethod().Name);

        }

        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {
            Debug.WriteLine(System.Reflection.MethodBase.GetCurrentMethod().Name);
            Application.AutoRecover.Enabled = false;

            Application.WorkbookOpen += Application_WorkbookOpen;
            Application.WindowActivate += Application_WindowActivate;
            Application.WindowResize += Application_WindowResize;
            Application.WindowDeactivate += Application_WindowDeactivate;

            Application.ProtectedViewWindowActivate += Application_ProtectedViewWindowActivate;
            Application.ProtectedViewWindowResize += Application_ProtectedViewWindowResize;

            var activeWorkbook = Application.ActiveWorkbook;
            if (activeWorkbook != null)
            {
                Application_WorkbookOpen(activeWorkbook);
            }

        }


        private void Application_ProtectedViewWindowResize(Excel.ProtectedViewWindow Pvw)
        {
            Debug.WriteLine("Application_ProtectedViewWindowResize");

        }

        private void Application_ProtectedViewWindowActivate(Excel.ProtectedViewWindow Pvw)
        {
            Debug.WriteLine("Application_ProtectedViewWindowActivate");

        }

        private void Application_WindowDeactivate(Excel.Workbook Wb, Excel.Window Wn)
        {
            
            Debug.WriteLine("Application_WindowDeactivate");
            // in doc close, clos all overlays
            //DeleteWaterMark((IntPtr)Wn.Hwnd);
        }

        private void Application_WindowResize(Excel.Workbook Wb, Excel.Window Wn)
        {
            Debug.WriteLine("Application_WindowResize");
            //UpdateWaterMark((IntPtr)Wn.Hwnd);
        }

        private void Application_WindowActivate(Excel.Workbook Wb, Excel.Window Wn)
        {
            Debug.WriteLine("Application_WindowActivate");
            InitWaterMark(Wn.Hwnd, strOverlay);
        }

        

        private void Application_WorkbookOpen(Excel.Workbook Wb)
        {
            Debug.WriteLine(System.Reflection.MethodBase.GetCurrentMethod().Name);
            Wb.BeforeSave += Wb_BeforeSave;
            Wb.BeforeClose += Wb_BeforeClose;

            Wb.WindowActivate += Wb_WindowActivate;
            Wb.WindowResize += Wb_WindowResize;

            
            Excel::Windows ws = Wb.Windows;
            // colleciton in COM the index is from 1
            var hWnd = ws.Item[1].Hwnd;
            InitWaterMark(hWnd, strOverlay);

        }

       

        private void Wb_WindowResize(Excel.Window Wn)
        {
            Debug.WriteLine("Wb_WindowResize");
            //if (bHasWarterMark)
            //{
            //    UpdateWaterMark((IntPtr)Wn.Hwnd);
            //}
        }


        private void Wb_WindowActivate(Excel.Window Wn)
        {
            Debug.WriteLine("Wb_WindowActivate");
            InitWaterMark(Wn.Hwnd,strOverlay);
        }

        private void Wb_BeforeClose(ref bool Cancel)
        {
            //MessageBox.Show("Wb_BeforeClose");
            Debug.WriteLine("Wb_BeforeClose");
            Excel::Windows ws = Application.ActiveWorkbook.Windows;

            for(int i = 1; i < ws.Count; i++)
            {
                DeleteWaterMark(ws[i].Hwnd);
            }
        }

        private void Wb_BeforeSave(bool SaveAsUI, ref bool Cancel)
        {
            Debug.WriteLine(System.Reflection.MethodBase.GetCurrentMethod().Name);
            try
            {
                var f = new Form1();
                f.ShowDialog();

                Office.DocumentProperties cps = Application.ActiveWorkbook.CustomDocumentProperties;


                var iter = cps.GetEnumerator();

                while (iter.MoveNext())
                {                    
                    Office.DocumentProperty p= (Office.DocumentProperty)iter.Current;
                    
                    if (p.Type == Office.MsoDocProperties.msoPropertyTypeString)
                    {
                        string v = p.Value;
                        MessageBox.Show(p.Name+" "+ p.Value);
                    }
                    

                }
                iter.Reset();

                cps.Add("iter", false, Office.MsoDocProperties.msoPropertyTypeString, "yes");

            }catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }
            
        }

       

        #region VSTO generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InternalStartup()
        {
            this.Startup += new System.EventHandler(ThisAddIn_Startup);
            this.Shutdown += new System.EventHandler(ThisAddIn_Shutdown);
        }

        #endregion



        #region CppDLL for watermark
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "InitWaterMark")]
        static private extern void InitWaterMark(int hwnd,
            [MarshalAs(UnmanagedType.LPWStr)] string str);

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "UpdateWaterMark")]
        static private extern void UpdateWaterMark(int hwnd);

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "DeleteWaterMark")]
        static private extern void DeleteWaterMark(int hwnd);

        #endregion
    }
}
