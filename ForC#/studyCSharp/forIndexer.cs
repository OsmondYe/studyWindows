using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace studyCSharp
{
    public class Person
    {
        public string Name { get; set; }
        public int age { get; set; }

        public Person(string name,int age)
        {
            this.Name = name;
            this.age = age;
        }

    }
    public class PersonColleciton : IEnumerable
    {
        private Dictionary<string, Person> listPerson = new Dictionary<string, Person>();
        public IEnumerator GetEnumerator()
        {
            return listPerson.GetEnumerator();
        }

        //
        // how to write a basic indexer
        // 
        public Person this[string name]
        {
            get { return (Person)listPerson[name]; }
            set { listPerson[name] = value; }
        }

        public void ClearPerson()
        {
            listPerson.Clear();
        }

        public int Count
        {
            get { return listPerson.Count; }
        }

    }


    // Indexer at the Interface
    public interface IStringContainer
    {
        string this[int index] { get;set; }
    }

    public class StrClass : IStringContainer
    {
        private List<String> strs = new List<string>();
        public string this[int index] { get => strs[index]; set => strs.Insert(index,value); }
    }


    class forIndexer
    {
        public static void Study()
        {

            PersonColleciton pc = new PersonColleciton();
        
            pc["Abc"] = new Person("Abc", 10);


        }
    }
}
