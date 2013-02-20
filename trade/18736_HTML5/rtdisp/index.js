var server = require("./server");
var router = require("./router");
var requestHandlers = require("./requestHandlers");

var handle = {}
handle["/"] = requestHandlers.mobile;
handle["/connect"] = requestHandlers.connect;
handle["/antechamber"] = requestHandlers.antechamber;
handle["/edit"] = requestHandlers.edit;
handle["/draw"] = requestHandlers.draw;
handle["/favicon.ico"] = requestHandlers.favicon;

server.start(router.route, handle, 52273);
