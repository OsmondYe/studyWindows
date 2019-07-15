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

namespace ExcelAddIn1
{
    public partial class ThisAddIn
    {
        Excel.Workbook cur;

        protected override Microsoft.Office.Core.IRibbonExtensibility CreateRibbonExtensibilityObject()
        {
            Debug.WriteLine("CreateRibbonExtensibilityObject");
            return new Ribbon();
        }

        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {
            Application.WorkbookOpen += Application_WorkbookOpen;
            //MessageBox.Show("ThisAddIn_Startup");
            Debug.WriteLine("ThisAddIn_Startup");
        }
        
        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {

        }


        private void Application_WorkbookOpen(Excel.Workbook Wb)
        {
            this.cur = Wb;
            Wb.BeforeSave += Wb_BeforeSave;
            Wb.BeforeClose += Wb_BeforeClose;
        }

        private void Wb_BeforeClose(ref bool Cancel)
        {
            //MessageBox.Show("Wb_BeforeClose");
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
    }
}
