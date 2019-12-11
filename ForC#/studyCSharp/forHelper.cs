using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;

namespace studyCSharp
{
    public delegate void FolderFound(string folder);

    class forHelper
    {

        static public void Study()
        {
            EnumerateFolders(@"C:\Users\oye\AppData\Local\SkyDRM", (i) => {
                ProtectFolder(i, false);
            });
        }

        static void EnumerateFolders(string folder, FolderFound action)
        {
            foreach (var i in Directory.EnumerateDirectories(folder))
            {
                Console.WriteLine("Found "+i);
                action(i);
                EnumerateFolders(i, action);
            }
            

        }

        static bool ProtectFolder(string folder, bool isSet)
        {
            bool rt = false;
            try
            {

                NTAccount curUser = (NTAccount)WindowsIdentity.GetCurrent().User.Translate(typeof(NTAccount));

                FileSystemAccessRule DenyListDir = new FileSystemAccessRule(curUser,
                    FileSystemRights.ListDirectory, AccessControlType.Deny);


                DirectorySecurity dirSecureity = System.IO.Directory.GetAccessControl(folder);



                if (isSet)
                {
                    dirSecureity.ResetAccessRule(DenyListDir);
                }
                else
                {
                    dirSecureity.RemoveAccessRule(DenyListDir);
                }

                System.IO.Directory.SetAccessControl(folder, dirSecureity);

                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
            return rt;
        }
    }
}
