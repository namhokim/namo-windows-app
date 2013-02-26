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

	var drawData = null;	// local draw data storage
	
	io.sockets.on('connection', function(socket) {
		if (drawData!=null) {
			socket.emit('draw', drawData);
		}
		
		socket.on('data', function (data) {
			drawData = data;
			console.log(drawData);
			socket.broadcast.emit('draw', drawData);
		});
	});

	console.log("Server has started on " + port + ".");
}

exports.start = start;