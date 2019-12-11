using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace studyCSharp
{
    class forRegisty
    {
        static public void Study()
        {
            // Opearte on Current User
            RegistryKey key = Registry.CurrentUser;
            // Open or create Reg\CurrentUser\software\Nextlabs\SkyDRM\LocalApp
            RegistryKey Nextlabs= key.CreateSubKey(@"software\Nextlabs");
            RegistryKey SkyDRM = Nextlabs.CreateSubKey(@"SkyDRM");
            /*
             *  Nextlabs:
             *      SkyDRM
             *          LocalApp
             *              Directory:  working folder         
             *              Executable:  exe path
             *              User
             *                  Name: user name
             *                  Email: user email
             *                  Code: user pass code
             *                  Login Time: user login time
             *                  
             *                  
             */
            RegistryKey LocalApp = SkyDRM.CreateSubKey(@"LocalApp");
            string dir = System.Environment.CurrentDirectory;
            string exe = Process.GetCurrentProcess().MainModule.FileName;
            LocalApp.SetValue("Directory", dir, RegistryValueKind.String);
            LocalApp.SetValue("Executable", exe, RegistryValueKind.String);

            RegistryKey User = LocalApp.CreateSubKey(@"User");

            string winLoginUser = System.Environment.UserName;

            User.SetValue("Name", winLoginUser, RegistryValueKind.String);
            User.SetValue("Email", winLoginUser, RegistryValueKind.String);
            User.SetValue("Code", winLoginUser, RegistryValueKind.String);
            User.SetValue("Login Time", winLoginUser, RegistryValueKind.String);


            bool rt = IsValueExist(User, "name");


            User.Close();
            LocalApp.Close();
            SkyDRM.Close();
            Nextlabs.Close();
            key.Close();
        }


        private static bool IsValueExist(RegistryKey key, string item)
        {
            try
            {
                string[] subs = key.GetValueNames();

                return (from i in subs select i.ToLower()).Contains(item.ToLower());
            }
            catch
            {
                return false;
            }
        }

    }
}
