using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace studyCSharp
{
        
    class forInvokeCpp
    {
        [StructLayout(LayoutKind.Sequential)]
        private struct ManagedSimpleStruct
        {
            public byte byteValue;
            public short shortValue;
            public int intValue;
            public Int64 int64Value;
            public bool boolValue;
            public float floatValue;
            public double doubleValue;
        }

        public void T_Basic()
        {
            int a = add(1, 2);
            int b = sub(1, 2);

            // for string
            // c# provide buffer as StringBuilder to c++, let c++ the fill the Buffer.
            string str = @"The quick brown fox jumps over the lazy dog.";
            StringBuilder sb1 = new StringBuilder(str.Length);
            StringBuilder sb2 = new StringBuilder(str.Length);
            StrReverseA(str, sb1);
            StrReverseW(str, sb2);

            string str2 = sb1.ToString();

            Console.WriteLine("sb1:"+str2);
            str2 = sb2.ToString();
            Console.WriteLine("sb2:" + str2);

            /*
            
             DLLImport ,
               dll name seraching rules,  -1 current woking folder -2 win dir -3 Path var
               Charset, default is ANSI or Unicode to c++ world?   c# string to wchar_t* or char*?

                    out   [OUT]
                    ret   [In,Out]

            
             MarshalAs
                MarshalAs(UnmanagedType.LPStr)     -> char*
                MarshalAs(UnmanagedType.LPWStr)    -> wchar_t*
             
             In
             Out
            */

        }

        public void T_OutStrAsParam()
        {
            string inStr = @"The Quick Brown Fox Jumps Over The Lazy Dog. 1234567890";
            for(int i = 0; i < 1; i++)
            {
                string outStr= null;
                //extern void FillStringFromComMem(string inStr, ref string outStr);
                FillStringFromComMem(inStr, ref outStr);
                Console.WriteLine(outStr);

            }

            for (int i = 0; i < 1; i++)
            {                
                string outStr;
                IntPtr ptrOutStr = IntPtr.Zero;
                // must use IntPtr
                //extern string FillStringFromHeap(string inStr,ref IntPtr outStr);
                FillStringFromHeap(inStr, ref ptrOutStr);
                outStr = Marshal.PtrToStringUni(ptrOutStr);
                // release 
                Free_FillStringFromHeap(ptrOutStr);
                // If by COMTaskMEM can tall Mashal.FreeCoTaskMem                
                Console.WriteLine(outStr);

            }

        }


        public void T_ReturnString()
        {
            // c# accept heap allocated value from c++
            // wchar_t * GetStringFromHeap()
            // use IntPtr to represent all pointer-type
            for (int i = 0; i < 1; i++)
            {
                IntPtr pStr = GetStringFromHeap();
                // after received pointer, Use Marshal to convert the pointer to str type
                string str2 = Marshal.PtrToStringUni(pStr);
                // free pStr
                FreeStringFromHeap(pStr);

            }

            //
            // For easy use, in c++ ues CoTaskMemAlloc, 
            // c# will automatically convert wchar_t* to string 
            // and also relase bug by calling ::CoTaskFreeMem
            // 
            for (int i = 0; i < 1; i++)
            {
                //static extern string GetStringFromCOMMem();
                var str2 = GetStringFromCOMMem();
                Console.WriteLine(str2);
            }

            //
            //  COM alloc and release by c# code
            //
            for (int i = 0; i < 1; i++)
            {
                //static extern IntPtr GetStringFromCOMMem2();
                IntPtr pp = GetStringFromCOMMem2();
                string str2 = Marshal.PtrToStringUni(pp);
                // release by 
                Marshal.FreeCoTaskMem(pp);
                Console.WriteLine(str2);
            }

        }

        public void T_Struct()
        {
            ManagedSimpleStruct ss = new ManagedSimpleStruct();

            ss.byteValue = 0x64;
            ss.shortValue = 0x200;
            ss.intValue = 0x100;
            ss.int64Value = 0x1024;
            ss.boolValue = false;
            ss.floatValue = 1.001F;
            ss.doubleValue = 2.002;

            // define             
            Action<ManagedSimpleStruct> print = new Action<ManagedSimpleStruct>((i) =>
             {
                 Console.WriteLine(
                     "simple struct: byte={0},short={1},int={2},int64={3},bool={4},float={5},double={6}",
                     i.byteValue,
                     i.shortValue,
                     i.intValue,
                     i.int64Value,
                     i.boolValue,
                     i.floatValue,
                     i.doubleValue);
             }
             );

            Console.WriteLine("sizeof(ManagedSimpleStruct) in c#:{0}",
                Marshal.SizeOf(typeof(ManagedSimpleStruct)));

            print(ss);           

            Console.WriteLine("after call c++");
            //
            // as param send to c++ to modify basice value
            //

            FillStruct(ref ss);

            print(ss);

            //
            // receive a struct return by c+++ (using new )
            // 

            IntPtr  ps = ReturnStruct();
            ManagedSimpleStruct s1 = (ManagedSimpleStruct)Marshal.PtrToStructure(ps, typeof(ManagedSimpleStruct));

            // free ps immediately,since we have hold s1
            FreeStruct(ps);
            print(s1);

            //
            // receive a struct return by c++ (using ::CoTaskMemAlloc)
            //
            IntPtr ps2 = ReturnStructByComMem();
            ManagedSimpleStruct s2 = (ManagedSimpleStruct)Marshal.PtrToStructure(ps2, typeof(ManagedSimpleStruct));
            Marshal.FreeCoTaskMem(ps2);
            print(s2);

            //
            // receive a struct as out Param
            //
            IntPtr ps3=IntPtr.Zero;
            ReturnStructAsParam(ref ps3);
            ManagedSimpleStruct s3 = (ManagedSimpleStruct)Marshal.PtrToStructure(ps3, typeof(ManagedSimpleStruct));
            Marshal.FreeCoTaskMem(ps3);

            print(s3);


        }

        static public void Study()
        {
            var cases = new forInvokeCpp();


            cases.T_Basic();
            cases.T_OutStrAsParam();
            cases.T_ReturnString();
            cases.T_Struct();
            
           

        }
           
        //
        // DLLImport ,
        //   dll name seraching rules,  -1 current woking folder -2 win dir -3 Path var
        //

        // Write cpp function 
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int add(int a, int b);        

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int sub(int a, int b);


        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet =CharSet.Ansi , ExactSpelling =true)]
        private static extern void showMsgA(string msg);

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void show();

        // - Use EntryPoint, C# will rename the calling dll-func-name
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,EntryPoint = "showMsg")]
        private static extern void showMsgRenamed([MarshalAs(UnmanagedType.LPWStr) ]string msg);

        //
        // - C-style untyped parameter, in C# must give a strong type 
        // 
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "UntypedParamShow")]
        private static extern void ShowUntyped1(ref int val, int flag);  // ref int means int*

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "UntypedParamShow")]
        private static extern void ShowUntyped2(string str, int flag);

        //
        // return string by param,  all the returned string must Unicode, and must use StringBuilder as C# param
        //
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "StrReverseA")]
        private static extern void StrReverseA(string pIn, StringBuilder pOut);

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl, 
            CharSet =CharSet.Unicode, EntryPoint = "StrReverseW")]
        private static extern void StrReverseW(string pIn, StringBuilder pOut);


        // how to alloce c/c++ heap string in CPP and free in c#;
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "GetStringFromHeap")]
        private static extern IntPtr GetStringFromHeap();

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "FreeStringFromHeap")]
        private static extern void FreeStringFromHeap(IntPtr p);

        // how to alloce COM heap string in CPP, .Net will aotu release
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "GetStringFromCOMMem")]
        private static extern string GetStringFromCOMMem();
        // like above ,but we do release by ourself
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "GetStringFromCOMMem")]
        private static extern IntPtr GetStringFromCOMMem2();



        //
        // out string as param
        //
        // how to alloce COM heap string in CPP, .Net will aotu release
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "FillStringFromHeap")]
        private static extern void FillStringFromHeap(string inStr,ref IntPtr outStr);

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
           CharSet = CharSet.Unicode, EntryPoint = "Free_FillStringFromHeap")]
        private static extern string Free_FillStringFromHeap(IntPtr inStr );

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
           CharSet = CharSet.Unicode, EntryPoint = "FillStringFromComMem")]
        private static extern void FillStringFromComMem(string inStr, ref string outStr);


        //
        //  In Struct as Param
        //
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
           CharSet = CharSet.Unicode, EntryPoint = "FillStruct")]
        private static extern void FillStruct(ref ManagedSimpleStruct st);

        // Return a newed struct
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
               CharSet = CharSet.Unicode, EntryPoint = "ReturnStructByNew")]
        private static extern IntPtr ReturnStruct();

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
               CharSet = CharSet.Unicode, EntryPoint = "FreeStructByNewed")]
        private static extern void FreeStruct(IntPtr spt);

        //
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
               CharSet = CharSet.Unicode, EntryPoint = "ReturnStructByComMem")]
        private static extern IntPtr ReturnStructByComMem();

        //
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
           CharSet = CharSet.Unicode, EntryPoint = "ReturnStructAsParam")]
        private static extern void ReturnStructAsParam(ref IntPtr st);




    }
}
