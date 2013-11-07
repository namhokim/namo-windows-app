var fs = require("fs"),
    jade = require('jade'),
	url = require('url'),
	wts = require('./wts'),
	qs = require('querystring');

var jadefile = fs.readFileSync(__dirname + '/start.jade');
var fn = jade.compile(jadefile.toString('utf8'));

function start(response) {
    console.log("Request handler 'start' was called.");
    var data = wts.enumerate();
    var json_data = eval('(' + data + ')');
    var html = fn(json_data);
    response.writeHead(200, {"Content-Type": "text/html"});
    response.write(html);
    response.end();
}

function favicon(response) {
  console.log("Request handler 'favicon' was called.");
  fs.readFile("favicon.ico", "binary", function(error, file) {
    if(error) {
      response.writeHead(500, {"Content-Type": "text/plain"});
      response.write(error + "\n");
      response.end();
    } else {
      response.writeHead(200, {"Content-Type": "image/vnd.microsoft.icon"});
      response.write(file, "binary");
      response.end();
    }
  });
}

function jquery(response) {
  console.log("Request handler 'favicon' was called.");
  fs.readFile("jquery-1.10.2.min.js", "utf8", function(error, file) {
    if(error) {
      response.writeHead(500, {"Content-Type": "text/plain"});
      response.write(error + "\n");
      response.end();
    } else {
      response.writeHead(200, {"Content-Type": "application/x-javascript"});
      response.write(file, "binary");
      response.end();
    }
  });
}

// this <- message()
function messageResponder(response, query) {
    var to = query["to"];
    var msg = query["msg"];
    
    try {
        var res = wts.message(parseInt(to, 10), msg);
        response.writeHead(200, { "Content-Type": "text/html" });
        response.write(res);
        response.end();
    } catch (e) {
        response.writeHead(200, { "Content-Type": "text/html" });
        response.write('<html><body><h1>Insufficien Parameter</h1>'
            + '<p>example: /message?to={number}&msg="{message}"</p></body>');
        response.end();
    }
}

function message(response, request) {
    console.log("Request handler 'message' was called.");

    if ('POST' == request.method) {
        var postData = "";
        request.addListener("data", function(chunk) {
            postData += chunk;
        });
        request.addListener("end", function() {
            var q = qs.parse(postData);
            messageResponder(response, q);
        });
        return;

    } else if ('GET' == request.method) {
        // refs. http://nodejs.org/docs/v0.6.15/api/url.html
        var urlObj = url.parse(request.url, true);
        var q = urlObj.query;
        messageResponder(response, q);
    } else {
        response.writeHead(403, { "Content-Type": "text/html" });
        response.write("Allow Only GET & POST");
        response.end();
        return;
    }
}

exports.start = start;
exports.favicon = favicon;
exports.jquery = jquery;
exports.message = message;
