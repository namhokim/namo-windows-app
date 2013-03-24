http://firejune.io/express/guide

1. express framework ��ġ
 > npm install express

2. ���ø����̼� ����
 > node_modules\.bin\express.cmd foo && cd foo

3. ���Ӽ� ��� ��ġ
 foo> npm install -d

4. ���ø����̼� ����
 > node app.js


---

1. HTTP ���� �����
 - ���� ����
 > node http_server.js
 - ���� �׽�Ʈ
 ��������) http://localhost:3000/

2. HTTPS ���� �����
 - OpenSSL ��ġ(Git�� �ִٸ� �̹� ��ġ)
 - ������ ����(privatekey.pem�� certificate.pem)
 > openssl genrsa -out privatekey.pem 1024
 > openssl req -new -key privatekey.pem -out certrequest.csr
 > openssl x509 -req -in certrequest.csr -signkey privatekey.pem -out certificate.pem
 - ���� ����
 > node http_server.js
 - ���� �׽�Ʈ
 ��������) https://localhost:3000/
