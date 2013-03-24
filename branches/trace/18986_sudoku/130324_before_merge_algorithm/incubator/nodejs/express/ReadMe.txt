http://firejune.io/express/guide

1. express framework 설치
 > npm install express

2. 애플리케이션 생성
 > node_modules\.bin\express.cmd foo && cd foo

3. 종속성 모듈 설치
 foo> npm install -d

4. 애플리케이션 실행
 > node app.js


---

1. HTTP 서버 만들기
 - 서버 실행
 > node http_server.js
 - 접속 테스트
 웹브라우저) http://localhost:3000/

2. HTTPS 서버 만들기
 - OpenSSL 설치(Git이 있다면 이미 설치)
 - 인증서 생성(privatekey.pem과 certificate.pem)
 > openssl genrsa -out privatekey.pem 1024
 > openssl req -new -key privatekey.pem -out certrequest.csr
 > openssl x509 -req -in certrequest.csr -signkey privatekey.pem -out certificate.pem
 - 서버 실행
 > node http_server.js
 - 접속 테스트
 웹브라우저) https://localhost:3000/
