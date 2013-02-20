var querystring = require("querystring"),
    fs = require("fs");

var DEFAULT_ENCODING = 'utf8';
var RAW_ENCODING = 'binary';

function mobile(response, postData) {
  fs.readFile(__dirname + '/index.html', DEFAULT_ENCODING,
    function(error, file) {
      if(error) {
        response.writeHead(500, {"Content-Type": "text/plain"});
        response.write(error + "\n");
        response.end();
      } else {
        response.writeHead(200, {"Content-Type": "text/html"});
        response.write(file, DEFAULT_ENCODING);
        response.end();
      }
    });
}

function draw(response, postData) {
  fs.readFile(__dirname + '/draw.html', DEFAULT_ENCODING,
    function(error, file) {
      if(error) {
        response.writeHead(500, {"Content-Type": "text/plain"});
        response.write(error + "\n");
        response.end();
      } else {
        response.writeHead(200, {"Content-Type": "text/html"});
        response.write(file, DEFAULT_ENCODING);
        response.end();
      }
    });
}

function favicon(response, postData) {
  fs.readFile(__dirname + '/favicon.ico', RAW_ENCODING,
    function(error, file) {
      if(error) {
        response.writeHead(500, {"Content-Type": "text/plain"});
        response.write(error + "\n");
        response.end();
      } else {
        response.writeHead(200, {"Content-Type": "image/vnd.microsoft.icon"});
        response.write(file, RAW_ENCODING);
        response.end();
      }
    });
}


exports.mobile = mobile;
exports.draw = draw;
exports.favicon = favicon;