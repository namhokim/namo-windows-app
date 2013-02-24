function start(route, handle, port) {

	// handler
	function onRequest(request, response) {
		var url = require("url");
		var pathname = url.parse(request.url).pathname;
		console.log("Request for " + pathname + " received.");
		route(handle, pathname, response, request);
	}

	// start
	if (typeof port === "undefined") port = 80;

	var app = require('http').createServer(onRequest)
		, io = require('socket.io').listen(app);

	app.listen(port);

	io.sockets.on('connection', function(socket) {
		socket.emit('news', { hello: 'world' });
		socket.on('my other event', function (data) {
			console.log(data);
		});
	});
	console.log("Server has started on " + port + ".");
}

exports.start = start;