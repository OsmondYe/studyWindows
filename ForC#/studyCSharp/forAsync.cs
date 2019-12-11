using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Contexts;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace studyCSharp
{
    class forAsync
    {
        static public void Study()
        {
            // TPL
            // PLINQ
            // async await
            Basic();
            
        }

        private static void Basic()
        {
            // get self
            Thread t = Thread.CurrentThread;
            Console.WriteLine(t.IsAlive+""+t.IsBackground+""+t.IsThreadPoolThread+""+t.Name);
            Console.WriteLine("Domain is:"+Thread.GetDomain()+""+Thread.GetDomainID());

            // 

            AppDomain ad = Thread.GetDomain();
            Context ct = Thread.CurrentContext;

        }
    }
}
