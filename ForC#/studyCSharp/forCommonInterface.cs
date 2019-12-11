using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace studyCSharp
{
    // IComparable<T>
    // IFormatable
    // IEnumerable<T>, IEnumerator   (LINQ based)   keyword: yield


    public class CIFDemo : IComparable<String>, IFormattable,IEnumerable<String>
    {
        int IComparable<string>.CompareTo(string other)
        {
            throw new NotImplementedException();
        }

        IEnumerator<string> IEnumerable<string>.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        string IFormattable.ToString(string format, IFormatProvider formatProvider)
        {
            throw new NotImplementedException();
        }
    }



    class forCommonInterface
    {
    }
}
