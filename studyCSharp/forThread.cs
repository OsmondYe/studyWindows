using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace studyCSharp
{
    class forThread
    {


        static public void Study()
        {
            
            UseDelegate();
            UseThread();

        }


        public delegate int TasksAWhileDelegate(int date, int ms);

        static int TaskAWhile(int data,int ms)
        {
            Console.WriteLine("TaskAWhile started!");
            Thread.Sleep(ms);
            Console.WriteLine("TaskAWhile completed!");
            return ++data;
        }

        private static void UseDelegate()
        {
            // Delegate原生支持异步委托,通过 BeginInvoke和EndInvoke(获取委托函数的返回值) 实现
            TasksAWhileDelegate aD = TaskAWhile;
            IAsyncResult result=aD.BeginInvoke(1, 3000, null, null);
            while (!result.IsCompleted)
            {
                Console.WriteLine("wait delegate to finish");
                Thread.Sleep(50);
            }
            Console.WriteLine("result of delegate:{0}", aD.EndInvoke(result));

            // 使用waiteone 进行等待

            TasksAWhileDelegate d2 = TaskAWhile;
            var r2 = d2.BeginInvoke(1, 3000, null, null);
            while (true)
            {
                Console.WriteLine("wait d2...");
                if (r2.AsyncWaitHandle.WaitOne(50, false))
                {
                    Console.WriteLine("Can get the result now");
                    break;
                }
            }
            Console.WriteLine("result d2:{0}",d2.EndInvoke(r2));


            // 使用 异步回调
            TasksAWhileDelegate d3 = TaskAWhile;           

            d3.BeginInvoke(1,3000,ar =>              
            {
                // 后台线程调用此代码
                // will be called when task has finished
                Console.WriteLine("result d3 is:{0}",d3.EndInvoke(ar));
            },
            null);

            Thread.Sleep(5000);// big enough for async call back to be called

        }


        private static void UseThread()
        {
            var t1 = new Thread(() =>
            {
                // be runned at worker t
                Console.WriteLine("runned at worker t_id:{0}", Thread.CurrentThread.ManagedThreadId);
            })
            {Name="aName",IsBackground=true };

            t1.Start();

            Console.WriteLine("this is main thread t_id{0}",Thread.CurrentThread.ManagedThreadId);
        }

    }
}
