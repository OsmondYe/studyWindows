using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace studyCSharp
{
    class forInvokeCpp
    {
        static public void Study()
        {
            // This will depend on CppDLL project
            //show();
            //showMsgA(@"this is very good");
            //showMsgRenamed(@"this is good");
            int val = 100;
            //ShowUntyped1(ref val, 1);
            string str = @"The quick brown fox jumps over the lazy dog.";
            //ShowUntyped2(str, 2);

            // return StringBuilder
            StringBuilder sb1 = new StringBuilder(str.Length);
            StringBuilder sb2 = new StringBuilder(str.Length);
            StrReverseA(str, sb1);
            StrReverseW(str, sb2);

            string str2 = sb1.ToString();
            str2 = sb2.ToString();

            IntPtr pStr = GetString();
            // c# accept heap allocated value from c++
            str2 = Marshal.PtrToStringUni(pStr);
            // free pStr
            FreeString(pStr);

            int a = add(1, 2);
            int b = sub(1, 2);
        }
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


        // how to alloce heap string in CPP and free in c#;
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "GetStringFromHeap")]
        private static extern IntPtr GetString();

        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Unicode, EntryPoint = "FreeStringFromHeap")]
        private static extern void FreeString(IntPtr p);


        // Write cpp function 
        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int add(int a, int b);



        [DllImport("CPPDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int sub(int a, int b);







    }
}
