var http = require("http")
  , url = require("url");

function start(route, handle, port) {
  function onRequest(request, response) {
    var pathname = url.parse(request.url).pathname;
    console.log("Request for " + pathname + " received.");
    route(handle, pathname, response, request);
  }

  if (typeof port == "undefined")
  {
    port = 80;
  }

  var app = http.createServer(onRequest).listen(port);
  var io = require('socket.io').listen(app);
  console.log("Server has started on " + port + ".");
}

exports.start = start;