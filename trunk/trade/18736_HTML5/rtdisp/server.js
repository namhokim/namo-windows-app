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
		socket.on('send', function (data) {
			try {
				console.log(data);
				socket.broadcast.emit('receive', data);
			} catch (err){
				console.log(err);
				return true;
			}
		});
		socket.on('requestData', function (data) {
			try {
				console.log(data);
				socket.broadcast.emit('requestData', data);
			} catch (err){
				console.log(err);
				return true;
			}
		});
		socket.on('responseData', function (data) {
			try {
				console.log(data);
				socket.broadcast.emit('responseData', data);
			} catch (err){
				console.log(err);
				return true;
			}
		});
	});

	console.log("Server has started on " + port + ".");
}

exports.start = start;