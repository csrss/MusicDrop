using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MusicDrop
{
    public class HttpServerAsync : BackgroundWorker
    {
        HttpServerBindingParameters _parameters = new HttpServerBindingParameters();

        /// <summary>
        /// Server object.
        /// </summary>
        private Http.CHttpServer _server;

        public HttpServerAsync()
        {
            _server = 
                new Http.CHttpServer(_parameters.IPAddress, _parameters.Port);
            _server.HttpFileDropEvent = new HttpFileDropEvent();
            _server.HttpSongsExposeEvent = new HttpSongsExposeEvent();

            base.WorkerSupportsCancellation = true;

            base.DoWork += HttpServerAsync_DoWork;
            base.RunWorkerCompleted += HttpServerAsync_RunWorkerCompleted;
        }

        void HttpServerAsync_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            try
            {
                _server.Stop();
            }
            catch (Exception)
            {
                // Log error if any
            }
        }

        void HttpServerAsync_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                try
                {
                    using (Http.CHttpServerConnection pConnection = _server.Accept())
                    {
                        _server.Process(pConnection);
                    }
                }
                catch (Exception)
                {
                    // Log error if any
                }
            }
        }

        public String Address
        {
            get { return _parameters.IPAddress; }
        }

        public int Port
        {
            get { return _parameters.Port; }
        }
    }
}
