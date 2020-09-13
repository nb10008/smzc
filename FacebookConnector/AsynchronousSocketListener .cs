using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using csharp_oauth;
using System.Web;


namespace OAuth
{
    // State object for reading client data asynchronously
    public class StateObject
    {
        // Client  socket.
        public Socket workSocket = null;
        // Size of receive buffer.
        public const int BufferSize = 1024;
        // Receive buffer.
        public byte[] buffer = new byte[BufferSize];
        // Received data string.
        public StringBuilder sb = new StringBuilder();
    }

    public class AsynchronousSocketListener 
    {
        // Thread signal.
        public static ManualResetEvent allDone = new ManualResetEvent(false);

        public static Mutex mutex = new Mutex();
        public Thread m_thread;  

        public AsynchronousSocketListener()
        {
            try
            {
                m_thread = new Thread(new ThreadStart(StartListening));
                ThreadPool.SetMaxThreads(10, 5);

            }
            catch (Exception ex)
            {
                Logger.WriteLog(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        public  void reset_allDone(){
            allDone.Set();
        }

        public  void StartListening()
        {
            try
            {
                // Data buffer for incoming data.
                byte[] bytes = new Byte[1024];

                // Establish the local endpoint for the socket.
                // The DNS name of the computer
                // running the listener is "host.contoso.com".
                // IPHostEntry ipHostInfo = Dns.Resolve("localhost");

                IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Any, GlobalVar.Port);

                // Create a TCP/IP socket.
                Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                // Bind the socket to the local endpoint and listen for incoming connections.
                try
                {
                    listener.Bind(localEndPoint);
                    listener.Listen(200);

                    while (GlobalVar.dbok)
                    {
                        // Set the event to nonsignaled state.
                        allDone.Reset();
                        // Start an asynchronous socket to listen for connections.
                        Console.WriteLine("Waiting for a connection...");

                        listener.BeginAccept(new AsyncCallback(AcceptCallback), listener);

                        Logger.WriteLog("Get a connection.");

                        // Wait until a connection is made before continuing.
                        allDone.WaitOne();
                        Thread.Sleep(0);
                    }
                }
                catch (Exception e)
                {
                    throw e;
                }

            }
            catch (Exception ex)
            {
                Logger.WriteLog(ex.Message + "\r\n" + ex.StackTrace);
            }
        }

        public  void AcceptCallback(IAsyncResult ar)
        {
            try
            {
                // Signal the main thread to continue.
                allDone.Set();

                // Get the socket that handles the client request.
                Socket listener = (Socket)ar.AsyncState;
                Socket handler = listener.EndAccept(ar);

                // Create the state object.
                StateObject state = new StateObject();
                state.workSocket = handler;
                handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReadCallback), state);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        static void  GetWebPage(Object stateInfo)
        {
            string url = (string)stateInfo;
            mutex.WaitOne(); Logger.WriteLog("Url is :" + url); mutex.ReleaseMutex();
            try
            {
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);               
                request.Timeout = 20000; // 10 second
                HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                System.IO.Stream responseStream = response.GetResponseStream();
                byte[] buf = new byte[4096];
                while (responseStream.Read(buf, 0, buf.Length) != 0)
                {
                }
                responseStream.Close();
                response.Close();
//                return true;
            }
            catch (Exception ex)
            {
                mutex.WaitOne();
                Logger.WriteLog("GetWebPage error! Url is :" + url);
                Logger.WriteLog("Exception is :" + ex);
                mutex.ReleaseMutex();
            }
        }

        static void PostMessage(string pid , string tp, string charName, string desc)
        {
            string url = GlobalVar.facebookUrl + "?" + "gameid=" + GlobalVar.gameid + "&tp=" + tp + "&pid=" + pid + "&char=" + charName + "&desc=" + desc;
            string md5 = System.Web.Security.FormsAuthentication.HashPasswordForStoringInConfigFile(GlobalVar.gameid + pid + tp + charName + desc + GlobalVar.securityKey, "md5");
            url += "&authkey=" + md5;
            GetWebPage(url);
//            ThreadPool.QueueUserWorkItem(new WaitCallback(GetWebPage), url);     
        }

        static void ProcessMessage(Object stateinfo)
        {
            string message = (string)stateinfo;
            string [] parts = message.Split(new char[] {';' });
            if (parts.Length < 3)
            {
                mutex.WaitOne();Logger.WriteLog("message format error! message is :" + message);mutex.ReleaseMutex();
                return;
            }

            string pid = parts[1];
            string charName = HttpUtility.UrlEncode(parts[2], Encoding.UTF8);

           
            if (parts[0].Equals("001"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                PostMessage(pid, "lg", charName, "non");
            }
            else if (parts[0].Equals("002"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                string level = parts[3];
                PostMessage(pid, "lv", charName, level);
            }
            else if (parts[0].Equals("003"))
            {
                if (parts.Length != 3 && parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                PostMessage(pid, "ge", charName, "non");                
            }
            else if (parts[0].Equals("004"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                string level = parts[3];
                PostMessage(pid, "el", charName, level);
            }
            else if (parts[0].Equals("005"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                string guildName = HttpUtility.UrlEncode(parts[3], Encoding.UTF8);
                PostMessage(pid, "cg", charName, guildName);
            }
            else if (parts[0].Equals("006"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                string bossID = parts[3];
                PostMessage(pid, "bd", charName, "non");
            }
            else if (parts[0].Equals("007"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                string typeID = parts[3];
                PostMessage(pid, "eu", charName, "non");
            }
            else if (parts[0].Equals("008"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                string friendNumber = parts[3];
                PostMessage(pid, "fr", charName, "non");
            }
            else if (parts[0].Equals("009"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                string transferType = parts[3];
                PostMessage(pid, "ch", charName, "non");
            }
            else if (parts[0].Equals("010"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                string titleID = parts[3];
                PostMessage(pid, "ng", charName, "non");
            }
            else if (parts[0].Equals("011"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                PostMessage(pid, "dq", charName, "dq1");
            }
            else if (parts[0].Equals("012"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                PostMessage(pid, "dq", charName, "dq2");
            }
            else if (parts[0].Equals("012"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                PostMessage(pid, "dq", charName, "dq2");
            }
            else if (parts[0].Equals("013"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                PostMessage(pid, "dq", charName, "dq3");
            }
            else if (parts[0].Equals("014"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                PostMessage(pid, "dq", charName, "dq4");
            }
            else if (parts[0].Equals("015"))
            {
                if (parts.Length != 4)
                {
                    mutex.WaitOne(); Logger.WriteLog("message format error! message is :" + message); mutex.ReleaseMutex();
                    return;
                }
                PostMessage(pid, "dq", charName, "dq5");
            }
            else 
            {
                mutex.WaitOne(); Logger.WriteLog("Unknow message ID! message is :" + message); mutex.ReleaseMutex();
            }


        }

        public  void ReadCallback(IAsyncResult ar)
        {
            try
            {
                String content = String.Empty;

                StateObject state = (StateObject)ar.AsyncState;
                Socket handler = state.workSocket;

                int bytesRead = handler.EndReceive(ar);

                if (bytesRead > 0)
                {
                    // There  might be more data, so store the data received so far.
                    state.sb.Append(Encoding.UTF8.GetString(state.buffer, 0, bytesRead));

                    content = state.sb.ToString();
                    
                    if (content.IndexOf("\r\n") > -1)
                    {
                        content = content.Trim();
                        if (!content.Equals(""))
                        {
                            Logger.WriteLog("receive message£º " + content);
                            ThreadPool.QueueUserWorkItem(new WaitCallback(ProcessMessage), content);

                            if (true)
                            {
                                GlobalVar.isucc++;
                            }
                            else
                            {
                                GlobalVar.ifail++;
                            }
                        }
                        state = new StateObject();
                        state.workSocket = handler;
                        handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReadCallback), state);
                    }
                    else
                    {
                        handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReadCallback), state);
                    }
                }
            }
            catch (Exception ex)
            {
                Logger.WriteLog(ex.Message + "\r\n" + ex.StackTrace);
            }
        }
    }
}
