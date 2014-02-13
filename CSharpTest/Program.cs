using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices; 
namespace CSharpTest
{
    class Program
    {

        [DllImport("BlackJackDealer.dll", EntryPoint = "IsGameOver")]
        bool IsGameOver();
        [DllImport("BlackJackDealer.dll", EntryPoint = "BlackJackDealer")]
        BlackJackDealer* BlackJackDealer();
        static void Main(string[] args)
        {

        }
    }
}


