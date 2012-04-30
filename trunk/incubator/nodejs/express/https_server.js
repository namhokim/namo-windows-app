fs = require("fs");
var privateKey = fs.readFileSync('privatekey.pem').toString();
var certificate = fs.readFileSync('certificate.pem').toString();

var app = require('express').createServer({key: privateKey, cert: certificate});

app.get('/', function(req, res){
  res.send('hello world');
});

app.listen(3000);