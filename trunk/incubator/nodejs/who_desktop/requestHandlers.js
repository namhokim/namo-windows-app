var MONITOR_PORT = 3389;

var fs = require("fs"),
    spawn = require('child_process').spawn,
    jade = require('jade');

var jadefile = fs.readFileSync(__dirname + '/start.jade');
var fn = jade.compile(jadefile.toString('utf8'));

function start(response) {
    console.log("Request handler 'start' was called.");
    
    var bin = "whoConnect.exe";
    var args = [MONITOR_PORT];
    var cspr = spawn(bin, args);
    
    cspr.stdout.setEncoding('utf8');
    cspr.stdout.on('data', function(data) {  
        var str = data.toString(), lines = str.split(",");
        var html = fn({"lines":lines});
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


exports.start = start;
exports.favicon = favicon;