var querystring = require("querystring"),
    fs = require("fs");

var DEFAULT_ENCODING = 'utf8';
var RAW_ENCODING = 'binary';

//////////////////////////////////////////////////////////////////////////

// ex. getLastPart('/static/favicon.ico', '.')  => ico
// ex. getLastPart('/static/index.html', '.')  => html
function getLastPart(targetString, delimeter)
{
  if (delimeter.length > targetString.length) {
    return '';
  }
  else {
    var p = targetString.lastIndexOf(delimeter);
    if (p == -1) return '';
    var i = p + delimeter.length;
    var r = targetString.substring(i);  
    return r;
  }
}

// 정적 파일 핸들러
function static_handler(response, request, pathname) {
  console.log("static_handler for " + pathname);

  // MIME판단
  var enc = DEFAULT_ENCODING
    , type = 'application/octet-stream';
  switch (getLastPart(pathname, '.'))
  {
  case 'ico':
    enc = RAW_ENCODING;
    type = "image/vnd.microsoft.icon";
    break;
  case 'html':
    type = "text/html";
    break;
  case 'css':
    type = "text/css";
    break;
  case 'js':
    type = "text/javascript";
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

//////////////////////////////////////////////////////////////////////////

// #1. 모바일 초기화면
function mobile(response, request) {
  static_handler(response, request, '/static/index.html');
}

// #2. 모바일 접속중 화면
function connect(response, request) {
  static_handler(response, request, '/static/connecting.html');
}

// #3. 모바일 작업시작 화면
function antechamber(response, request) {
  static_handler(response, request, '/static/antechamber.html');
}

// #4. 모바일 편집 화면
function edit(response, request) {
  static_handler(response, request, '/static/edit.html');
}

// #5. 데스트탑 디스플레이 화면
function draw(response, request) {
  static_handler(response, request, '/static/draw.html');
}

// favicon.ico 요청 처리
function favicon(response, request) {
  static_handler(response, request, '/static/favicon.ico');
}


exports.mobile = mobile;
exports.connect = connect;
exports.antechamber = antechamber;
exports.edit = edit;
exports.draw = draw;
exports.favicon = favicon;
exports.static_handler = static_handler;