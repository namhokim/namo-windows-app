var fs = require("fs"),
    spawn = require('child_process').spawn,
    jade = require('jade')
	url = require('url');

var jadefile = fs.readFileSync(__dirname + '/start.jade');
var fn = jade.compile(jadefile.toString('utf8'));

function start(response) {
    console.log("Request handler 'start' was called.");
    
    var bin = "whoConnect.exe";
    var args = [];
    var cspr = spawn(bin, args);
    
    cspr.stdout.setEncoding('utf8');
    cspr.stdout.on('data', function(data) {
		console.log("whoConnect");
		var json_data = eval('(' + data + ')');
        var html = fn(json_data);
        response.writeHead(200, {"Content-Type": "text/html"});
        response.write(html);
        response.end();
    });
	cspr.stderr.on('data', function(data) {
		response.writeHead(200, {"Content-Type": "text/text"});
		console.log("whoConnect stderr: " + data);
		response.end();
	});
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

function message(response, request) {
	// refs. http://nodejs.org/docs/v0.6.15/api/url.html
	var urlObj = url.parse(request.url, true);
	var q = urlObj.query;
	var to = q["to"];
	var msg = q["msg"];
//	console.log("Request for " + to + " received.");

	console.log("Request handler 'message' was called.");
    
    var bin = "whoConnect.exe";
    var args = [to, msg];
    var cspr = spawn(bin, args);
    
    cspr.stdout.setEncoding('utf8');
    cspr.stdout.on('data', function(data) {
		console.log("whoConnect");
		var json_data = eval('(' + data + ')');
        var html = fn(json_data);
        response.writeHead(200, {"Content-Type": "text/html"});
        response.write('stdout - OK');
        response.end();
    });
	cspr.stderr.on('data', function(data) {
		response.writeHead(200, {"Content-Type": "text/html"});
		console.log("whoConnect stderr: " + data);
		response.write('stderr - OK');
		response.end();
	});
	cspr.on('exit', function(code) {
		console.log("child process exited with code " + code);

		response.writeHead(200, {"Content-Type": "text/html"});
		response.write('exit - OK');
		response.end();
	});
}

exports.start = start;
exports.favicon = favicon;
exports.message = message;
