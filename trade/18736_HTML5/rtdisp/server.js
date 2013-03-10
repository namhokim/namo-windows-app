function start(route, handle, port) {

	var refreshAgain = false;
	var refreshing = false;

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
			socket.emit('draw_once', drawData);
		}
		
		socket.on('data', function (data) {
			try {
				drawData = data;
				refreshAgain = data.draw.refreshAgain;
				console.log(drawData);
				socket.broadcast.emit('draw', drawData);
			} catch (err){
				console.log(err);
				return true;
			}
		});
	});

	console.log("Server has started on " + port + ".");
}

exports.start = start;