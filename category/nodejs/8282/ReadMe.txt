1. ���� ���(npm install)
 - express [jade, connect, socket.io...]
   who_desktop> npm install
 - ws (�ʿ��: �ӵ�)
  who_desktop> npm install ws --ws:native
2. �߰� ���(V8 C++ Addons)
 - vsproj/native ������Ʈ ����
 - ���� ��� ����: wts.node
 - who_desktop/node_modules �� ����(64,32��Ʈ ����!)
3. IE 6.0 ��ġ(socket.io)
 - module_patch/socket.js �� \node_modules\socket.io\node_modules\socket.io-client\lib\ �� �����
4. ����(�׽�Ʈ)
 - who_desktop> node app.js
5. ����ȭ
 refs. http://blog.tatham.oddie.com.au/2011/03/16/node-js-on-windows/
  Download : http://nssm.cc/download/?page=download
 > nssm install whoDesktop C:\who_desktop\node.exe C:\who_desktop\index.js
 > net start whoDesktop
6. install node.js (standalone)
 - node.exe�� ����
 http://www.nodejs.org/#download
  x86: http://nodejs.org/dist/v0.6.17/
  x64: http://nodejs.org/dist/v0.6.17/x64/