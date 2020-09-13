using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Web.Security;
using System.Text;

namespace csharp_oauth
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
//            "Œ§".get
/*
            byte[] abc = new byte[2];
            abc[0] = 140;
            abc[1] = 167;
            string s = Encoding.Unicode.GetString(abc, 0, 2);
            byte[] kkk;
            Encoding encoding = Encoding.GetEncoding("gb2312");
            kkk = encoding.GetBytes("Œ§");
            Console.WriteLine(s);
            Console.WriteLine(kkk[0]);
 */ 
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new frmMain());

            //csharp_oauth.biz.NPCService ns = new csharp_oauth.biz.NPCServiceImpl();
            //(new frmMain()).ReadiniFile();
            //Console.WriteLine(ns.checkdb());
            //Console.WriteLine(AzDG.Crypt("root", "pK93Unaq4angUmFwthM1qwTT"));
            //Console.WriteLine(FormsAuthentication.HashPasswordForStoringInConfigFile("WPOBTAC83CC005FTC1RP" + "kEHrDooxWHCWtfeSxvDvgqZq", "MD5"));
            //Console.WriteLine(ns.processKeyCode("1300000", "1300101", "127.0.0.1", "ellen", "SAXJUNKIDR7TQULUVAT8"));

        }
    }
}  