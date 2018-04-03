using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace studyCSharp
{
    class forDotNetBasic
    {
        public static void Study()
        {
            Console.WriteLine("forString:");
            forString.Study();
            Console.WriteLine("end forString:");

        }
    }



    class forString
    {
        static public void Study()
        {
            // string is System.String

            // concat
            string msg1 = "Hello";
            msg1 += ", There";          // result: "Hello, There"
            string msg2 = msg1 + "!";   // result: "Hello, There!"
            // use index
            char c4 = msg2[4];          // result  o

        }
    }
    
}
