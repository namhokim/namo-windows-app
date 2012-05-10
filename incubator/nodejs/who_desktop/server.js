var http = require("http");
var url = require("url");
var socketio = require("socket.io");

function start(route, handle) {
    function onRequest(request, response) {
        var pathname = url.parse(request.url).pathname;
        console.log("Request for " + pathname + " received.");
        route(handle, pathname, response, request);
    }

    // Create: Web Server
    var server = http.createServer(onRequest).listen(8282, function() {
        console.log("Server Running at http://localhost:8282");
    });

    // Create: Socket Server
    var io = socketio.listen(server);
    var clients = 0;    // http://stackoverflow.com/questions/7352164/update-all-clients-using-socket-io
    io.set('log level', 2); // https://github.com/LearnBoost/Socket.IO/wiki/Configuring-Socket.IO
    io.sockets.on('connection', function(socket) {
        ++clients;
        socket.broadcast.emit('users_count', { number: clients });

        // message event
        socket.on('message', function(data) {
            // emit message
            io.sockets.emit('message', data);
        });
        socket.on('disconnect', function() {
            --clients;
            socket.broadcast.emit('users_count', { number: clients });  // except for self
        });

        io.sockets.sockets[socket.id].emit('users_count', { number: clients, id: socket.id }); // to self
    });

}

exports.start = start;