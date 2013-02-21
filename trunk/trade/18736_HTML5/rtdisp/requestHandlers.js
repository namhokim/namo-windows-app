var querystring = require("querystring"),
    fs = require("fs");

var DEFAULT_ENCODING = 'utf8';
var RAW_ENCODING = 'binary';

// #1. 모바일 초기화면
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

// #2. 모바일 접속중 화면
function connect(response, postData) {
  fs.readFile(__dirname + '/connecting.html', DEFAULT_ENCODING,
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

// #3. 모바일 작업시작 화면
function antechamber(response, postData) {
  fs.readFile(__dirname + '/antechamber.html', DEFAULT_ENCODING,
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

// #4. 모바일 편집 화면
function edit(response, postData) {
  fs.readFile(__dirname + '/edit.html', DEFAULT_ENCODING,
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

// #5. 데스트탑 디스플레이 화면
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

function getLastPart(targetString, delimeter)
{
  if (delimeter.length > targetString.length) {
    return '';
  }
  else {
    var p = targetString.lastIndexOf(delimeter);
    if (p == -1) return '';
    var i = p + delimeter.length;
      document.write( i );
    var r = targetString.substring(i);  
    return r;
  }
}

function static_handler(response, resource, pathname) {
  console.log("static_handler for " + pathname);

  // MIME판단
  var enc, type;
  switch (getLastPart)
  {
  case 'ico':
    enc = RAW_ENCODING;
    type = "image/vnd.microsoft.icon";
    break;
  case 'html':
  default:
    enc = DEFAULT_ENCODING;
    type = "text/html";
    break;  
  }

  // 리소스 반환
  fs.readFile(__dirname + pathname, enc,
    function(error, file) {
      if(error) {
        response.writeHead(500, {"Content-Type": "text/plain"});
        response.write(error + "\n");
        response.end();
      } else {
        response.writeHead(200, {"Content-Type": type});
        response.write(file, enc);
        response.end();
      }
    });
}


exports.mobile = mobile;
exports.connect = connect;
exports.antechamber = antechamber;
exports.edit = edit;
exports.draw = draw;
exports.favicon = favicon;
exports.static_handler = static_handler;