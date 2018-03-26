using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace studyCSharp
{
    public static class StringExtension
    {
        public static void Foo(this string s)
        {
            Console.WriteLine("Foo invoded for {0}", s);
        }
    }
        
       

    class forClassExtension
    {
        public static void Study()
        {
            string str = "Hello World";

            // below two statements are same meaning;
            StringExtension.Foo(str);
            str.Foo();

        }
    }
}
