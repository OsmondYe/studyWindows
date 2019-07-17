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
        Excel.Workbook cur;
        Ribbon ribbon = new Ribbon();

        protected override Microsoft.Office.Core.IRibbonExtensibility CreateRibbonExtensibilityObject()
        {
            Debug.WriteLine("CreateRibbonExtensibilityObject");
            return ribbon;
        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
            if (bHasWarterMark)
            {
                DeleteWaterMark();
            }
            Debug.WriteLine("ThisAddIn_Shutdown");

        }

        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {
            Application.WorkbookOpen += Application_WorkbookOpen;
            Application.WindowActivate += Application_WindowActivate;
            Application.WindowResize += Application_WindowResize;
            Application.WindowDeactivate += Application_WindowDeactivate;

            Application.ProtectedViewWindowActivate += Application_ProtectedViewWindowActivate;
            Application.ProtectedViewWindowResize += Application_ProtectedViewWindowResize;


            Application.WorkbookAddinInstall += Application_WorkbookAddinInstall;
            Application.WorkbookAddinUninstall += Application_WorkbookAddinUninstall;
            
            Debug.WriteLine("Nexltabs: ThisAddIn_Startup");
            //MessageBox.Show("ThisAddIn_Startup");
        }

        private void Application_WorkbookAddinUninstall(Excel.Workbook Wb)
        {
            Debug.WriteLine("Nexltabs: Application_WorkbookAddinUninstall");
        }

        private void Application_WorkbookAddinInstall(Excel.Workbook Wb)
        {
            Debug.WriteLine("Nexltabs: Application_WorkbookAddinInstall");

            Wb.WindowActivate += Wb_WindowActivate;
            Wb.WindowResize += Wb_WindowResize;
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
        }

        private void Application_WindowResize(Excel.Workbook Wb, Excel.Window Wn)
        {
            Debug.WriteLine("Application_WindowResize");
            if (bHasWarterMark)
            {
                UpdateWaterMark((IntPtr)Wn.Hwnd);
            }
        }

        bool bHasWarterMark = false;
        private void Application_WindowActivate(Excel.Workbook Wb, Excel.Window Wn)
        {
            Debug.WriteLine("Application_WindowActivate");
            if (!bHasWarterMark)
            {
                bHasWarterMark = true;
                InitWaterMark((IntPtr)Wn.Hwnd);
            }

        }

        

        private void Application_WorkbookOpen(Excel.Workbook Wb)
        {           

            this.cur = Wb;  
            Wb.BeforeSave += Wb_BeforeSave;
            Wb.BeforeClose += Wb_BeforeClose;

            Wb.WindowActivate += Wb_WindowActivate;
            Wb.WindowResize += Wb_WindowResize;
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
            //if (!bHasWarterMark)
            //{
            //    bHasWarterMark = true;
            //    InitWaterMark((IntPtr)Wn.Hwnd);
            //}
        }

        private void Wb_BeforeClose(ref bool Cancel)
        {
            //MessageBox.Show("Wb_BeforeClose");
            Debug.WriteLine("Wb_BeforeClose");
        }

        private void Wb_BeforeSave(bool SaveAsUI, ref bool Cancel)
        {
            try
            {
                Office.DocumentProperties cps = cur.CustomDocumentProperties;


                var f = new Form1();
                f.ShowDialog();

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
