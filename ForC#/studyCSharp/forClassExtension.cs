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
        
     static class Helper
    {
        static public bool GreatThan10(string str)
        {
            return str.Length > 10;
        }

        static public IEnumerable<T> Mywhere<T>(this IEnumerable<T> s,string condition)
        {
            return s;
        }

        static public IEnumerable<T> MySelect<T>(this IEnumerable<T> s, string conditon)
        {
            return s;
        }

    }
       

    class forClassExtension
    {
        public static void Study()
        {
            string str = "Hello World";

            Helper.GreatThan10(str);



            //Helper.Mywhere(str);
            //str.Mywhere();

            List<int> ll = new List<int>(10);
             ll.Mywhere("i>100").MySelect("i");
            // from i in ll Mywhere i>100 MySelect i;


            // below two statements are same meaning;
            StringExtension.Foo(str);
            str.Foo();

        }
    }
}
