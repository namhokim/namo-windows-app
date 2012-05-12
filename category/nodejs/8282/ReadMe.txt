1. 관련 모듈(npm install)
 - express [jade, connect, socket.io...]
   who_desktop> npm install
 - ws (필요시: 속도)
  who_desktop> npm install ws --ws:native
2. 추가 모듈(V8 C++ Addons)
 - vsproj/native 프로젝트 빌드
 - 빌드 결과 파일: wts.node
 - who_desktop/node_modules 에 복사(64,32비트 구분!)
3. IE 6.0 패치(socket.io)
 - module_patch/socket.js 를 \node_modules\socket.io\node_modules\socket.io-client\lib\ 로 덮어씌움
4. 실행(테스트)
 - who_desktop> node app.js
5. 서비스화
 refs. http://blog.tatham.oddie.com.au/2011/03/16/node-js-on-windows/
  Download : http://nssm.cc/download/?page=download
 > nssm install whoDesktop C:\who_desktop\node.exe C:\who_desktop\index.js
 > net start whoDesktop
6. install node.js (standalone)
 - node.exe를 얻음
 http://www.nodejs.org/#download
  x86: http://nodejs.org/dist/v0.6.17/
  x64: http://nodejs.org/dist/v0.6.17/x64/