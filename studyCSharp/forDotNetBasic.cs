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
            Console.WriteLine("For DotNet UseDelegate:");
            forString.Study();
            Anonymous.Study();
            Initializer.Study();

        }
    }



    class forString
    {
        static public void Study()
        {
            Console.WriteLine("forString:");

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
    

    class Anonymous
    {
        public static void Study()
        {
            Console.WriteLine("For Anonymous");

            // define a anomyous class object
            // var   名字=new {字段赋值}；
            var obj = new { Guid.Empty, myTitle = "this is a title", myOtherParam = new int[] { 1, 2, 3, 4, 5, 6, 7 } };
            Console.WriteLine(obj.Empty);
            Console.WriteLine(obj.myTitle);
            Console.WriteLine(obj.myOtherParam);

            var tom = new { Name = "Tom", Age = 16 };
            var Ham = new { Name = "Hom", Age = 16 };
            var Jom = new { Name = "Jom", Age = 16 };


        }
    }

    class Initializer
    {
        class MyObj
        {
            private string name;
            public Guid id { get; set; }
            public int age { get; set; }

            public MyObj(string name)
            {
                this.name = name;
            }

        }
        public static void Study()
        {
            Console.WriteLine("for Initializer:");
            // 创建一个对象,直接给他赋值
            var obj = new MyObj("Tom") { id = Guid.NewGuid(), age = 16 };

            // Collections
            var obj2 = new List<int>() { 1, 2, 3, 4, 5, 6, 7, 8, 8, 10 };

        }
    }

}
