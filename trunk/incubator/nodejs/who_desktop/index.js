var server = require("./server");
var router = require("./router");
var requestHandlers = require("./requestHandlers");

var handle = {};
handle["/"] = requestHandlers.start;
handle["/favicon.ico"] = requestHandlers.favicon;
handle["/jquery-1.7.2.min.js"] = requestHandlers.jquery;
handle["/message"] = requestHandlers.message;

server.start(router.route, handle);