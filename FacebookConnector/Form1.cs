using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using OAuth;
using System.Threading;


namespace csharp_oauth
{
    public partial class frmMain : Form
    {
        AsynchronousSocketListener server;        
        [System.Runtime.InteropServices.DllImport("kernel32.dll")]
        static extern uint GetTickCount();
        public void addLogs(string s){

          if  (logBox.Lines.Length >100) {
              logBox.Clear() ;
          }else{
              logBox.AppendText(DateTime.Now.ToString() +" : "+s+"\n");

          }
        }

        public frmMain()
        {
            InitializeComponent();
        }

        public bool ReadiniFile()
        {
            INIClass ini = new INIClass(@Application.StartupPath+ "\\config.ini");
            if (! ini.ExistINIFile() ){
             MessageBox.Show("not Exist INIFile!"); 
             return false;
            }

            GlobalVar.Port = Int32.Parse(ini.IniReadValue("TCP", "port"));
            return true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            labelcount.Text = "";
            GlobalVar.isucc = 0;
            GlobalVar.ifail = 0;
            GlobalVar.dbok = true;
            ReadiniFile();
//            btnStart_Click(sender,e);
        }


        private void btnStart_Click(object sender, EventArgs e)
        {
            btnStart.Enabled = false;
            uint t1 = GetTickCount();
            if (GlobalVar.dbok)
            {
                try
                {
                    server = new AsynchronousSocketListener();
                    server.m_thread.Start();
                }
                catch (Exception ex)
                {
                    addLogs(ex.Message + ex.StackTrace);
                }

                uint t2 = GetTickCount();
                addLogs("Init OK ! spend: " + (t2 - t1) + "ms");
            }
        }

        private void btnclear_Click(object sender, EventArgs e)
        {
            logBox.Clear();
        }

        private void frmMain_Leave(object sender, EventArgs e)
        {
           
        }

        private void frmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (server != null && server.m_thread != null)
            {
                server.m_thread.Abort();
                server.m_thread = null;
            }
            Application.Exit();
        }

        private void tmrDisp_Tick(object sender, EventArgs e)
        {
            if(GlobalVar.dbok){
                labelcount.Text = " ³É¹¦: " + GlobalVar.isucc + "  Ê§°Ü: " + GlobalVar.ifail;

            }
        }
    }
}