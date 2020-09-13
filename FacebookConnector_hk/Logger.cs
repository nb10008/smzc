using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;

namespace csharp_oauth
{
    class Logger
    {
      
            static string FileName = "keyCodeBridgelog.log";
            const long MAXSIZE = 10 * 1024 * 1024;
            static System.Object lockThis = new object();




            public static void SetException(Exception exp)
            {
                SetException(exp, FileName);
            }

            public static void SetException(Exception exp, string strFileName)
            {
                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("异常详细信息：{0}\r\n", exp.Message);
                sb.AppendFormat("异常堆栈信息：{0}\r\n", exp.StackTrace);
                if (strFileName == "")
                    strFileName = FileName;
                WriteLog(sb.ToString(), strFileName);

            }


            public static void WriteLog(string str)
            {
                WriteLog(str, FileName);
            }

        public static void WriteLog(string str, string strFileName)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendFormat("[时间]：{0}\r\n", System.DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss ffff"));
            sb.Append(str + "\r\n");
            sb.Append("---------------------------------------\r\n\r\n\r\n");
            lock (lockThis)
            {
                StreamWriter w = null;
                try
                {
                    string fileName = strFileName.Substring(0,strFileName.Length-Path.GetExtension(strFileName).Length);
                    string fullFilePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, fileName + DateTime.Now.ToString("yyyyMMdd") + Path.GetExtension(strFileName));
                    FileInfo fileInfo = new FileInfo(fullFilePath);
                    //if (fileInfo.Exists && fileInfo.Length > MAXSIZE)
                    //{
                    //    fileInfo.Delete();
                    //}

                    w = new StreamWriter(fullFilePath, true, Encoding.GetEncoding("utf-8"));
                    w.Write(sb.ToString());
                    w.Flush();

                }
                catch
                {
                    //not do

                }
                finally
                {
                    if (w != null)
                    {
                        try
                        {
                            w.Close();
                            w.Dispose();
                        }
                        catch
                        {


                        }
                    }
                }


            }
        }
  
    }
}
