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
        static void Main(string[] args)
        {
        }
    }
}

namespace BlackJackDealer
{
    [DllImport("BlackJackDealer.dll", EntryPoint = "IsGameOver", CharSet = Unicode)]
   int MessageBox(void* hWnd, wchar_t* lpText, wchar_t* lpCaption, 
                  unsigned int uType);
}
