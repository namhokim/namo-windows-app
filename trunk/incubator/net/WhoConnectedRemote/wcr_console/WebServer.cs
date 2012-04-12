using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace wcr_console
{
    public class WebServer
    {
        private static string Prefix = "http://*:8282/";
        // refs. http://msdn.microsoft.com/en-us/library/system.net.httplistener(v=vs.90).aspx
        private HttpListener m_listener;

        public void Start()
        {
            if (!HttpListener.IsSupported)
            {
                throw new Exception("Windows XP SP2 or Server 2003 is required to use the HttpListener class.");
            }

            m_listener = new HttpListener();
            m_listener.Prefixes.Add(Prefix);
            m_listener.Start();
            m_listener.BeginGetContext(new AsyncCallback(ListenerCallback), m_listener);
        }

        public void Stop()
        {
            m_listener.Close();
        }

        private void ListenerCallback(IAsyncResult result)
        {
            HttpListener listener = (HttpListener)result.AsyncState;
            // Call EndGetContext to complete the asynchronous operation.
            HttpListenerContext context = listener.EndGetContext(result);
            HttpListenerRequest request = context.Request;
            // Obtain a response object.
            HttpListenerResponse response = context.Response;
            // Construct a response.
            byte[] buffer = System.Text.Encoding.UTF8.GetBytes(RemoteDesktopFinder.WhoWithHtml());
            // Get a response stream and write the response to it.
            response.ContentLength64 = buffer.Length;
            System.IO.Stream output = response.OutputStream;
            output.Write(buffer, 0, buffer.Length);
            // You must close the output stream.
            output.Close();

            m_listener.BeginGetContext(new AsyncCallback(ListenerCallback), m_listener);
        }
    }
}
