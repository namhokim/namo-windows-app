using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Web.UI;

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

            // Obtain a request & response object.
            HttpListenerRequest request = context.Request;
            HttpListenerResponse response = context.Response;

            // Routing
            switch (request.RawUrl)
            {
                case "/":
                    RemoteDesktopContent(response);
                    break;
                default:
                    NotFound(response);
                    break;
            }

            m_listener.BeginGetContext(new AsyncCallback(ListenerCallback), m_listener);
        }

        private void RemoteDesktopContent(HttpListenerResponse response)
        {
            response.ContentType = "text/html";

            StreamWriter textWriter = new StreamWriter(response.OutputStream, Encoding.UTF8);
            HtmlTextWriter writer = new HtmlTextWriter(textWriter);
            writer.RenderBeginTag(HtmlTextWriterTag.Html);

            writer.RenderBeginTag(HtmlTextWriterTag.Head);
            writer.AddAttribute("http-equiv", "Content-Type");
            writer.AddAttribute("content", "text/html; charset=utf-8");
            writer.RenderBeginTag(HtmlTextWriterTag.Meta);
            writer.RenderEndTag();    // </Meta>
            writer.RenderBeginTag(HtmlTextWriterTag.Title);
            writer.WriteEncodedText("누가 원격데스크탑을 하고 있나?");
            writer.RenderEndTag();    // </Title>
            writer.RenderEndTag();    // </Head>

            writer.RenderBeginTag(HtmlTextWriterTag.Body);
            writer.RenderBeginTag(HtmlTextWriterTag.H1);
            writer.WriteEncodedText("원격데스크탑 접속자");
            writer.RenderEndTag();    // </H1>

            writer.RenderBeginTag(HtmlTextWriterTag.Ol);
            List<string> list = RemoteDesktopFinder.Who();
            foreach (string li in list)
            {
                writer.RenderBeginTag(HtmlTextWriterTag.Li);
                writer.WriteEncodedText(li);
                writer.RenderEndTag();    // </Li>
            }
            writer.RenderEndTag();    // </Ol>

            writer.RenderEndTag();    // </Body>
            writer.RenderEndTag();    // </html>

            writer.EndRender();
            writer.Flush();
            writer.Close();
        }

        private void NotFound(HttpListenerResponse response)
        {
            response.StatusCode = 404;

            // refs. http://www.simpleisbest.net/archive/2007/01/08/1527.aspx
            StreamWriter textWriter = new StreamWriter(response.OutputStream, Encoding.UTF8);
            HtmlTextWriter writer = new HtmlTextWriter(textWriter);
            writer.RenderBeginTag(HtmlTextWriterTag.Html);
            writer.RenderEndTag();    // </html>
            writer.EndRender();
            writer.Flush();
            writer.Close();
        }
    }
}
