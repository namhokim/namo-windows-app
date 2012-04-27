var server = require("./server");
var router = require("./router");
var requestHandlers = require("./requestHandlers");

var handle = {}
handle["/"] = requestHandlers.start;
handle["/favicon.ico"] = requestHandlers.favicon;
handle["/message"] = requestHandlers.message;

server.start(router.route, handle);