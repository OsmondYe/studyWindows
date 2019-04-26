using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace studyCSharp
{
    class forRegex
    {
        static public void Study()
        {
            // like log-2019-01-24-07-04-28.txt
            string name = @"log-2019-01-24-07-04-28.txt.nxl";
            // pattern-match "-2019-01-24-07-04-28" replaced with latest lcoal timestamp
            string pattern = @"-\d{4}-\d{2}-\d{2}-\d{2}-\d{2}-\d{2}";
            // new stime string
            string newTimeStamp = DateTime.Now.ToLocalTime().ToString("-yyyy-MM-dd-HH-mm-ss");
            Regex r = new Regex(pattern);
            string newName = name;
            if (r.IsMatch(name)){
                newName = r.Replace(name, newTimeStamp);
            }           

            Console.WriteLine(newName);


        }
    }
}
