using System;
using System.Collections.Generic;
using System.Text;

namespace OAuth
{
    public static class GlobalVar
    {



        static bool _dbok;

        public static bool dbok
        {
            get
            {
                return _dbok;
            }
            set
            {
                _dbok = value;
            }
        }




        static int _port;

        public static int Port
        {
            get
            {
                return _port;
            }
            set
            {
                _port = value;
            }
        }




        static int _isucc;

        public static int isucc
        {
            get
            {
                return _isucc;
            }
            set
            {
                _isucc = value;
            }
        }


        static int _ifail;

        public static int ifail
        {
            get
            {
                return _ifail;
            }
            set
            {
                _ifail = value;
            }
        }

        static string _facebookUrl = "http://sm.omg.com.tw/Facebook/PublishMessageToFaceBook.aspx";
        public static string facebookUrl
        {
            get
            {
                return _facebookUrl;
            }
            set
            {
                _facebookUrl = value;
            }
        }

        static string _securityKey = "g4ai65vm3p";
        public static string securityKey
        {
            get
            {
                return _securityKey;
            }
            set
            {
                _securityKey = value;
            }
        }

        static string _gameid = "1055";
        public static string gameid
        {
            get
            {
                return _gameid;
            }
            set
            {
                _gameid = value;
            }
        }
    }
}
