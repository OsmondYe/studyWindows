using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace studyCSharp
{
    class forLambda
    {
        public static void Study()
        {
            List<int> list = new List<int>(new int[] { 20, 1, 4, 8, 9, 44 });

            // traditonal
            List<int> evens = list.FindAll(new Predicate<int>(isEven));

            // anonymous, must use keyword delegate
            evens = list.FindAll(
                delegate (int i) 
                {
                    return (i % 2) != 0;
                }
                );

            // lambda
            evens = list.FindAll(i => (i % 2) == 0);

        }
        static bool isEven(int i)
        {
            return (i % 2) == 0;
        }
    }
}
