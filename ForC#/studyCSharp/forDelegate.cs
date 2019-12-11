using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace studyCSharp
{
    public class forDelegate
    {
        // 委托就是函数对象, delegate这个关键字 和class是同level的
        // 委托是标准的观察者模型
        public static void Study()
        {
            // basic
            SimpleMath math = new SimpleMath();
            BinaryOp b = new BinaryOp(SimpleMath.Add);
            b += SimpleMath.Substract;
            b += math.Multile;

            DisplayDelegateInof(b);

            int c = b(1, 2);

            // generic
            // 系统预定义了 一些常用的泛型委托 Predicate, Func,Action
            //public delegate bool Predicate<in T>(T obj);
            //public delegate TResult Func<out TResult>();
            //public delegate TResult Func<in T, out TResult>(T arg);
            //public delegate TResult Func<in T1, in T2, out TResult>(T1 arg1, T2 arg2);
            Func<int, int, int> action = new Func<int, int, int>(SimpleMath.Add);
            action += SimpleMath.Substract;
            action += math.Multile;
            c = action(12, 32); // fire event
            //Action用于 只有参数没有返回值
            //public delegate void Action<in T1, in T2>(T1 arg1, T2 arg2)

        
            //
            //  use class
            //

            Car c1 = new Car("nono", 100, 10);

            c1.RegisterHandler(new Car.CarEngineHandler(OnCarEngineEvent));
            c1.RegisterHandler(OnCarEngineEvent2);

            for (int i = 0; i < 10; i++)
            {
                c1.Accererate(20);

            }


            Car2 c2 = new Car2("nono2", 100, 10);
            c2.AboutToBlow += OnBlow;
            // use anonymous method
            c2.AboutToBlow += delegate
            {
                Console.WriteLine("no name");
            };
            c2.AboutToBlow += delegate (object s, Car2.Car2EventArgs e)
            {
                Console.WriteLine("no name 2:" + e.msg);
            };

            c2.Exploded += C2_Exploded;
            for (int i = 0; i < 10; i++)
            {
                c2.Accererate(20);

            }


        }

        static void DisplayDelegateInof(Delegate obj)
        {
            foreach (Delegate item in obj.GetInvocationList())
            {
                Console.WriteLine("target:{0}\tmethod:{1}", item.Target, item.Method);
            }
        }


        private static void C2_Exploded(object sender, Car2.Car2EventArgs e)
        {
            Console.WriteLine("C2_Exploded:" + e.msg);
        }

        private static void OnBlow(object sender, Car2.Car2EventArgs e)
        {
            Console.WriteLine("OnBlow:" + e.msg);
        }

        static void OnCarEngineEvent(string msgForCaller)
        {
            Console.WriteLine("Event: {0}", msgForCaller);
        }

        static void OnCarEngineEvent2(string msgForCaller)
        {
            Console.WriteLine("Event: {0}", msgForCaller.ToUpper());
        }

    }

    public class SimpleMath
    {
        public static int Add(int x, int y)
        {
            Console.WriteLine("add");
            return x + y;
        }
        public static int Substract(int x, int y)
        {
            Console.WriteLine("sub");
            return x - y;
        }

        public int Multile(int x, int y)
        {
            Console.WriteLine("mul");
            return x * y;
        }

    }


    public delegate int BinaryOp(int x, int y);


    class Car
    {
        public int CurrentSpeed { get; set; }
        public int MaxSpeed { get; set; }
        public string PetName { get; set; }

        protected bool cartIsUsed;

        public Car()
        {
            MaxSpeed = 50;
        }

        public Car(string name, int maxSp, int CurrentSpeed)

        {
            PetName = name;
            MaxSpeed = maxSp;
            this.CurrentSpeed = CurrentSpeed;
        }


        // give a delegate
        public delegate void CarEngineHandler(string msgForCaller);


        // memver vars for delge
        private CarEngineHandler listOfHandler;

        // give caller a if to register it to the delegate
        public void RegisterHandler(CarEngineHandler handler)
        {
            listOfHandler += handler;
        }


        // 
        public void Accererate(int delta)
        {
            if (cartIsUsed)
            {
                if (listOfHandler != null)
                    listOfHandler("sorry the car id dead");
            }
            else
            {
                CurrentSpeed += delta;
                if (MaxSpeed - CurrentSpeed < 0 && listOfHandler != null)
                    listOfHandler("you are going to deat");
            }
            if (CurrentSpeed >= MaxSpeed)
            {
                cartIsUsed = true;
            }
            else
            {
                Console.WriteLine("current speed : {0}", CurrentSpeed);
            }
        }




    }

    class Car2 : Car
    {
        public class Car2EventArgs : EventArgs
        {
            public readonly string msg;
            public Car2EventArgs(string msg)
            {
                this.msg = msg;
            }
        }
        public Car2() : base()
        {

        }
        public Car2(string name, int maxsp, int cursp) : base(name, maxsp, cursp)
        {

        }


        public event EventHandler<Car2EventArgs> Exploded;
        public event EventHandler<Car2EventArgs> AboutToBlow;



        public void Accererate(int delta)
        {
            if (cartIsUsed)
            {
                if (Exploded != null)
                    Exploded(this, new Car2EventArgs("sorry the car id dead"));
            }
            else
            {
                CurrentSpeed += delta;
                if (MaxSpeed - CurrentSpeed < 0 && AboutToBlow != null)
                    AboutToBlow(this, new Car2EventArgs("you are going to deat"));
            }
            if (CurrentSpeed >= MaxSpeed)
            {
                cartIsUsed = true;
            }
            else
            {
                Console.WriteLine("current speed : {0}", CurrentSpeed);
            }
        }

    }


}
