using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace studyCSharp
{

    class People
    {
        public People(int age)
        {
            Age = age;
            Name = "NULL";
        }
        public int Age { get; set; }
        public string Name { get; set; }
    }

    class forLINQ
    {



        Student[] stuArray;
        public forLINQ()
        {
            FillStudentData(ref stuArray);
        }

        public static void Study()
        {
            
            forLINQ obj = new forLINQ();
            obj.TechShare();
            // Language Integrated Query
            //obj.Basic();
            //obj.Theory();
            //obj.QueryOverStrings();
            //obj.QueryOverInts();
            //obj.QueryOverStudents();
        }

        delegate bool Conditon(int i);

        delegate void Doing(int i);

        static bool Great20Less180(int i)
        {
            return i > 20 && i < 180;
        }

        static void Loop(int[] ints,Conditon cond)
        {
            foreach (var probe in ints)
            {
                if (cond(probe))
                {
                    Console.Write("{0}, ", probe);

                }
                //if (probe > 20 && probe < 180)
                //{
                //    Console.Write("{0}, ", probe);
                //}
            }
        }
        static void Loop2(int[] ints, Conditon cond,Doing doing)
        {
            foreach (var probe in ints)
            {
                if (cond(probe))
                {
                    doing(probe);
                }                
            }
        }

        void TechShare()
        {
            int[] ints = {
                0, 12, 34, 4, 453, 67,
                54,64,7324,1,8,65,37,
                23, 4236, 78, 24, 83,
                27, 34, 46, 79, 199 };


            // (20,180)
            for(int i=0; i < ints.Length; i++)
            {
                int probe = ints[i];
                if(probe>20 && probe < 180)
                {
                    Console.Write("{0}, ",probe);
                }
            }
                Console.WriteLine("");

            // for-each 
            foreach (var probe in ints)
            {
                if (probe > 20 && probe < 180)
                {
                    Console.Write("{0}, ", probe);
                }
            }
            //
            Console.WriteLine("Great20Less180");
            Loop(ints, Great20Less180);
            //
            Console.WriteLine("Anonymous");
            Loop(ints, delegate (int probe)
             {
                 return probe > 20 && probe < 180;
             }
            );
            Console.WriteLine("");
            Loop(ints, delegate (int probe)
            {
                return probe >= 300 && probe%2==0;
            }
            );           
            // Lambda
            Console.WriteLine("Lambda <20");
            //void Loop(int[] ints,Conditon cond)
            //delegate bool Conditon(int i);
            Loop(ints, Great20Less180);
            Loop(ints, delegate (int i){return i<20;});
            Loop(ints, (int i) => { return i < 20; });
            Loop(ints, (i) => i < 20 );

            // LINQ
            Console.WriteLine("");
            Console.WriteLine("LINQ");
            // Building a Iterator at runtime;
            var iterator = from i in ints  
                           where i < 100
                           orderby i descending
                           select new People(i);          

            for (int i = 0; i < ints.Length; i++) { ints[i] = 0; }
                                  
            foreach (var i in iterator)
            {
                Console.WriteLine("ID={0},IDSTR={1}",i.Age,i.Name);
            }

            //
            //
            //
            Student[] stu = null;
            FillStudentData(ref stu);

            var statement = from i in stu
                            orderby i.StudentName
                            group i by i.Age;

            foreach (var i in statement)
            {
                foreach(var ii in i)
                {
                    Console.WriteLine("{0},{1}",ii.StudentName,ii.Age);
                }
            }
            Console.WriteLine("Age>20");
            foreach (var item in stu.Where((s) => s.Age < 20) )
            {
                Console.WriteLine(item.StudentName);
            }

            

                             

        }


        void Basic()
        {
            // from i in set                :   Subject, 
            // where                        :   Condition
            // select                       :   subseq
            // join,on,equals,into          :   Cascade
            // orderby,ascending,descending : 
            // group, by

            /*
              form <rang variable> in  < IEnumerable<T> Collection >
              <Standard Query Operatators> <lambda expression>
              <select or groupBy operatore> <result formation>             
            */

            /*
                分类	标准查询运算符
                过滤	Where，OfType
                排序	OrderBy，OrderByDescending，ThenBy，ThenByDescending，Reverse
                分组	GroupBy，ToLookup
                加入	GroupJoin, Join
                投影	Select, SelectMany
                聚合	Aggregate, Average, Count, LongCount, Max, Min, Sum
                量词	All, Any, Contains
                元素	ElementAt，ElementAtOrDefault，First，FirstOrDefault，Last，LastOrDefault，Single，SingleOrDefault
                组	Distinct, Except, Intersect, Union
                分区	Skip, SkipWhile, Take, TakeWhile
                级联	Concat
                平等	SequenceEqual
                代	DefaultEmpty，Empty，Range，Repeat
                转换	AsEnumerable，AsQueryable，Cast，ToArray，ToDictionary，ToList
             */


            //- basic  只返回所有人的姓
            // from p in persons selcet p.FirstName

            //- condition
            // from c in cars where c.Make="BMW" && c.Speed>100 select c

            //- generate new class obj
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


            //
            //  Examples
            //
            IList<string> l = new List<string>
            {
                "C# Tutorials",
                "VB.NET Tutorials",
                "Learn C++",
                "MVC Tutorials" ,
                "Java",
                "Python"
            };

            var result = from s                             // every item ,named s ( s in {s1,s2,s3,s4,s5})
                         in l                               // target the collecion  l              // 集合L的每一个对象s
                         where s.Contains("Tutorials")      // define condition about every s
                         select s;                          // refine every s 

            var r1=result.ToList();

            var result2 = from s in l where s.Contains("Tutorials")  select s.Length;

            var r2 = result2.ToList();

            // multi where
            var result3 = from s in l where s.Contains("C") where s.Length > 2 select s;

            // order by
            var result4 = from s in l where s.Contains("C") where s.Length > 2  orderby s.Length ascending  select s;
            var result5 = from s in l where s.Contains("C") where s.Length > 2  orderby s,s.Length descending  select s;

            // group by  see QueryOverStudents

            // join, 跨表查询
            Student[] eng=null;
            FillStudentData(ref eng);
            Student[] chn = null;
            FillStudentDataChinese(ref chn);
            Gender[] gender = null;
            FillGender(ref gender);

            // find student's gender

            var result6 = from s in eng
                          join i in gender
                          on s.StudentID equals i.StudentID
                          select new { Name=s.StudentName, Age=s.Age, Gender=i.Sex  };
            var r6 = result6.ToArray();
                            


        }

        void Theory()
        {
            // LINQ 本质是上是对 Enumerable类中的方法的解析,
            //       System.Linq.Enumerable
            // 我们也可以自己来, 里面有不少方法接受一个委托,这可以引入lamba

            int[] ints = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

            // type: System.Collections.Generic.IEnumerable<int>
            var set = ints.Where(i => i > 5);

            foreach (int i in set)
            {
                Console.WriteLine(i);
            }


        }

        void QueryOverStrings()
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

            strs.Where((s, i) =>
            {
                // i : current index
                Console.WriteLine("s:{0},index:{1}", s, i);
                return true;
            }
            ).ToList();


        }

        void QueryOverInts()
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

        void QueryOverStudents()
        {
            // queary age (12,20)
            foreach (var item in this.stuArray)
            {
                if(item.Age> 12 && item.Age < 20)
                {
                    Console.WriteLine("id:{0},name:{1}",item.StudentID,item.StudentName);
                }
            }

            // how to generic foeach and condtions?  -> delegate
            var a=Student.Where(this.stuArray, delegate (Student std)
             {
                 return std.Age > 12 && std.Age < 20;
             }
             );           

            // simplify
            a = Student.Where(this.stuArray, i => i.Age > 12 && i.Age < 20 );

            // delegate vs Lambda
            /*
                delegate (Student std) { return std.Age > 12 && std.Age < 20; }
                             (std) =>  { return std.Age > 12 && std.Age < 20; }
                             std =>   std.Age > 12 && std.Age < 20
             */


            // use LINQ, bind with Lambda
            a = stuArray.Where(s => s.Age > 12 && s.Age < 20).ToArray();
            // find Where's defination, 


            // group and by  分组
            var result = from s in stuArray
                         orderby s.Age
                         group s by s.Age;
            Console.WriteLine("group:");
            foreach (var group in result)
            {
                Console.WriteLine("Age {0} group:",group.Key);
                foreach (var s in group)
                {
                    Console.WriteLine("name:{0},age:{1}",s.StudentName,s.Age);
                }

            }


        }



        private void FillStudentData(ref Student [] array)
        {
            Student[]  stuArray = new Student[]{
            new Student() { StudentID = 1, StudentName = "John", Age = 18 },
            new Student() { StudentID = 2, StudentName = "Steve", Age = 21 },
            new Student() { StudentID = 7, StudentName = "Zoom", Age = 19 },
            new Student() { StudentID = 3, StudentName = "Bill", Age = 25 },
            new Student() { StudentID = 4, StudentName = "Ram", Age = 20 },
            new Student() { StudentID = 7, StudentName = "Farm", Age = 19 },
            new Student() { StudentID = 5, StudentName = "Ron", Age = 31 },
            new Student() { StudentID = 6, StudentName = "Chris", Age = 17 },
            new Student() { StudentID = 7, StudentName = "Tom", Age = 19 },
            new Student() { StudentID = 7, StudentName = "Ama", Age = 19 },
            new Student() { StudentID = 8, StudentName = "Jerry", Age = 16 },
            new Student() { StudentID = 9, StudentName = "Jack", Age = 13 },
            new Student() { StudentID = 10, StudentName = "Lucy", Age = 19 },
            new Student() { StudentID = 11, StudentName = "Rob", Age = 19 },
            };
            array = stuArray;
        }

        private void FillStudentDataChinese(ref Student[] array)
        {
            Student[] stuArray = new Student[]{
            new Student() { StudentID = 1, StudentName = "王一", Age = 18 },
            new Student() { StudentID = 2, StudentName = "孙二", Age = 21 },
            new Student() { StudentID = 3, StudentName = "张三", Age = 25 },
            new Student() { StudentID = 4, StudentName = "李四", Age = 20 },
            new Student() { StudentID = 5, StudentName = "周五", Age = 31 },
            new Student() { StudentID = 6, StudentName = "郑六", Age = 17 },
            new Student() { StudentID = 7, StudentName = "蒋七", Age = 19 },
            new Student() { StudentID = 8, StudentName = "赵八", Age = 16 },
            new Student() { StudentID = 9, StudentName = "刘九", Age = 13 },
            new Student() { StudentID = 10, StudentName = "习十", Age = 19 },
            };
            array = stuArray;
        }

        private void FillGender(ref Gender[] array)
        {
            Gender[] l = new Gender[]
            {
                new Gender {StudentID=1,Sex="Male"},
                new Gender {StudentID=2,Sex="Female"},
                new Gender {StudentID=3,Sex="Male"},
                new Gender {StudentID=4,Sex="Female"},
                new Gender {StudentID=5,Sex="Female"},
                new Gender {StudentID=6,Sex="Male"},
                new Gender {StudentID=7,Sex="Female"},
                new Gender {StudentID=8,Sex="Male"},
                new Gender {StudentID=9,Sex="Male"},
                new Gender {StudentID=10,Sex="Female"},
            };

             array = l;
                
        }


    } // end for class forLINQ

    class Gender
    {
        public int StudentID { get; set; }
        public string Sex { get; set; }
    }
    class Student
    {
        public int StudentID { get; set; }
        public String StudentName { get; set; }
        public int Age { get; set; }


        public delegate bool FindStudent(Student stu);

        public static Student[] Where(Student[] array, FindStudent aDelegate)
        {
            List<Student> list = new List<Student>();
            foreach (var item in array)
            {
                if (aDelegate(item))
                {
                    list.Add(item);
                }
            }
            return list.ToArray();
        }        
    }// end class student

   


} // end ns
