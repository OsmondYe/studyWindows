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
            // fixed unmuttable contents, just replace new obj with old one
            // public IEnumerable<char>,  can be used by LINQ

            // concat
            string msg1 = "Hello";
            msg1 += ", There";          // result: "Hello, There"
            string msg2 = msg1 + "!";   // result: "Hello, There!"
            // use index
            char c4 = msg2[4];          // result  o

            msg2 = "ABCEFEGHIGKLMNOPQRSTUVWXYZ";

            var set = from i in msg2
                      where i > 'A' && i < 'Z'
                      select i;
            foreach (char i in set)
            {
                msg2 += i;
            }


            StringBuilder sb = new StringBuilder("this is a test for that test");

            sb.Append("a test that can be used for windows and linux, but not or MAC OS");

            sb.Replace('a', 'z');


        }
    }

}
