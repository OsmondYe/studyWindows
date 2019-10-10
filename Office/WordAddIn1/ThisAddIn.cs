using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using Word = Microsoft.Office.Interop.Word;
using Office = Microsoft.Office.Core;
using Microsoft.Office.Tools.Word;
using System.Windows.Forms;
using System.Diagnostics;

namespace WordAddIn1
{
    public partial class ThisAddIn
    {
        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {
            Application.DocumentBeforeSave += Application_DocumentBeforeSave;
            Application.DocumentOpen += Application_DocumentOpen;
            Application.DocumentBeforeClose += Application_DocumentBeforeClose;
            Application.DocumentChange += Application_DocumentChange;
            Application.WindowActivate += Application_WindowActivate;

        }

        private void Application_WindowActivate(Word.Document Doc, Word.Window Wn)
        {
            Debug.WriteLine("Application_WindowActivate Doc:"+Doc.Name);
        }

        private void Application_DocumentChange()
        {
            Debug.WriteLine("Application_DocumentChange");

        }

        private void Application_DocumentBeforeClose(Word.Document Doc, ref bool Cancel)
        {
            Debug.WriteLine("Application_DocumentBeforeClose");
        }

        private void Application_DocumentOpen(Word.Document Doc)
        {
            Debug.WriteLine("Application_DocumentOpen");   
        }




        private void Application_DocumentBeforeSave(Word.Document Doc, ref bool SaveAsUI, ref bool Cancel)
        {
            Debug.WriteLine("Application_DocumentBeforeSave");
            Debug.WriteLine("doc:" + Doc.Name);
            if (SaveAsUI)
            {
                Cancel = true;
                Debug.WriteLine("this is for save as");
                // we do it by our self
                var saveAsDlg = Application.FileDialog[Office.MsoFileDialogType.msoFileDialogSaveAs];
                saveAsDlg.Title = "Nextlabs Safe Save As";
                saveAsDlg.InitialFileName = Doc.Name;
                saveAsDlg.ButtonName = "Protected Save AS";

                if (-1 == saveAsDlg.Show())
                {
                    System.Diagnostics.Debug.WriteLine("d.showed"+ saveAsDlg.Item);
                    var newFilePath = saveAsDlg.SelectedItems.Item(1);
                    //saveAsDlg.Execute();

                    Doc.SaveAs2(newFilePath);


                    //MessageBox.Show("new file created:" + newFilePath, "safe save as ok");
                    //MessageBox.Show("demo for show ClassifyDlg");
                    // copy the new plain file to a nxl new one
                    //System.IO.File.Copy(newFilePath, newFilePath + ".nxl",true);  // may throw when can not overwrite
                    //var op = Word.WdSaveOptions.wdDoNotSaveChanges;
                    //Doc.Close(op);
                }
            }
        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
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
