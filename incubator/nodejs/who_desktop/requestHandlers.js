var MONITOR_PORT = 3389;

var fs = require("fs"),
    spawn = require('child_process').spawn,
    jade = require('jade')
	url = require('url');

var jadefile = fs.readFileSync(__dirname + '/start.jade');
var fn = jade.compile(jadefile.toString('utf8'));

function start(response) {
    console.log("Request handler 'start' was called.");
    
    var bin = "whoConnect.exe";
    var args = [MONITOR_PORT];
    var cspr = spawn(bin, args);
    
    cspr.stdout.setEncoding('utf8');
    cspr.stdout.on('data', function(data) {
		console.log("whoConnect");
        var str = data.toString(), lines = str.split("|");
        var html = fn({"lines":lines});
        response.writeHead(200, {"Content-Type": "text/html"});
        response.write(html);
        response.end();
    });
	cspr.stderr.on('data', function(data) {
		console.log("whoConnect stderr: " + data);
	});
	cspr.on('exit', function (data) {
		var html = fn({"lines":['사용자가 없습니다']});
		response.writeHead(200, {"Content-Type": "text/html"});
        response.write(html);
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


// http://www.shinstudio.com/blog/programming/pulling-query-string-in-node-js/
function arp(response, request) {
	var urlObj = url.parse(request.url, true);
	var q = urlObj.query["q"];
	console.log("Request handler 'arp' was called.");

	response.writeHead(200, {"Content-Type": "text/plain"});
	if(typeof q != "undefined") {
		console.log(" query : " + q);
		var bin = "SendARP.exe";
		var args = [q];
		var cspr = spawn(bin, args);
		cspr.stdout.setEncoding('utf8');
		cspr.stdout.on('data', function(data) {
			response.write(data);
			response.end();
		});
		cspr.stderr.on('data', function(data) {
			response.write(data);
			response.end();
		});
		cspr.on('exit', function (data) {
			response.write('NO DATA');
			response.end();
		});
	} else {
		response.write("NO PARAMETER(?q=xxx)");
		response.end();
	}
}

exports.start = start;
exports.favicon = favicon;
exports.arp = arp;