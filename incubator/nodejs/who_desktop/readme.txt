1. install node.js
 http://www.nodejs.org/#download
  x86: http://nodejs.org/dist/v0.6.17/
  x64: http://nodejs.org/dist/v0.6.17/x64/
2. install dependenced jade module
 (if not install, you meet "Error: Cannot find module 'jade'")
 > npm install jade
 > npm install socket.io
   (To install WS with blazing fast native extensions, use)
    > npm install ws --ws:native
3. Module patch
 - node_modules\socket.io\node_modules\socket.io-client\lib\socket.js
 - find "xhr.withCredentials = true;"
 - comment the line (it's not work in IE6.0)
4. V8 Addons node module build - wts.node
 Copy 'wts.node' file into directory of same as 'requestHandlers.js'.
5. Run Node.js
 > node index.js
6. Make Service
 refs. http://blog.tatham.oddie.com.au/2011/03/16/node-js-on-windows/
  Download : http://nssm.cc/download/?page=download
 > nssm install whoDesktop C:\who_desktop\node.exe C:\who_desktop\app.js
 > net start whoDesktop

 > nssm install whoDesktop C:\whoDesktop\node.exe C:\whoDesktop\index.js