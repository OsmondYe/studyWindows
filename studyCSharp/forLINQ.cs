using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace studyCSharp
{
    class forLINQ
    {
        public static void Study()
        {
            Basic();
            Theory();
            QueryOverStrings();
            QueryOverInts();
        }

        static void Basic()
        {
            // from i in set                :   Subject, 
            // where                        :   Condition
            // select                       :   subseq
            // join,on,equals,into          :   Cascade
            // orderby,ascending,descending : 
            // group, by


            //- basic  只返回所有人的姓
            // from p in persons selcet p.FirstName

            //- +condition
            // from c in cars where c.Make="BMW" && c.Speed>100 select c

            // generate new class obj
            // from c in cars where c.Make="BMW" && c.Speed>100 selcet new {c.Make,c.Owner}

            // Count 获取总数
            // (from p in persons selcet p).Count()

            // sort
            // ..... orderby p.FirstName ascending
            // ..... orderby p.FirstName descending

            // 多集合操作, Union, Differcen,Concatenation, Intersection(2者共有)        
            // (我所有的车).Except(你所有的车)    -> 我有,你没有的牌子
            // (我所有的车).Intersect(你所有的车) -> 你我都共有的牌子
            // (我所有的车).Union(你所有的车)     -> 去重后,你我所有的牌子
            //  (我所有的车).Concat(你所有的车)   -> 不去重 
            //                  .Concat(你所有的车).Distinct()  也可以去重

            // 聚合操作
            // ().Max(),  ().Min(), ().Average(), ().Sum()

        }

        static void Theory()
        {
            // LINQ 本质是上是对 Enumerable类中的方法的解析,
            //       System.Linq.Enumerable
            // 我们也可以自己来, 里面有不少方法接受一个委托,这可以引入lamba

            int[] ints = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

            var set = ints.Where(i => i > 5);

            foreach (int  i in set)
            {
                Console.WriteLine(i);
            }

        }

        static void QueryOverStrings()
        {
            string[] strs = {
                "Hello world",
                "This is not good",
                "for that quest",
                "Fallout 3",
                "Space"
            };
            // contain ' '
            var set = from g in strs
                      where g.Contains(' ')
                      orderby g
                      select g;
            foreach (string i in set)
            {
                Console.WriteLine(@"Contain ' ':" + i);
            }
            // the type is OrderedEnumerable<TElement,TKey>
            Console.WriteLine("var's type:{0}", set.GetType().ToString());


            // non-generic Collection
            ArrayList al = new ArrayList(strs);

            // how to say?
            //var set2 = from i in al
            //           where i.ff

            // -> convert into IEnumerabale<T>
            var ss = al.OfType<string>();

            var set2 = from i in ss where i.Length > 10 select i;

        }

        static void QueryOverInts()
        {
            int[] ints = { 0, 12, 34, 4, 453, 67, 23, 4236, 78, 24, 83, 27, 34, 46, 79, 199 };


            // < 100
            var set2 = from i in ints
                       where i < 100
                       orderby i
                       select i;
            // the clause will be impled at foreach 
            foreach (int i in set2)
            {
                Console.WriteLine("<100:{0}", i);
            }
            // 定义的查询语句不会立即执行,而是在实际用到的地方才会执行
            ints[0] = 10000;
            foreach (int i in set2)
            {
                Console.WriteLine("<100:{0}", i);
            }
            // 可以强制立即执行
            int[] ints2 = (from i in ints
                           where i > 100
                           select i).ToArray<int>();

        }

    }
}
