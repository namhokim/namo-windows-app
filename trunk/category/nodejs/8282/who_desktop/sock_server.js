var socketio = require("socket.io");
var wts = require('wts');
var fs = require('fs');
var jade = require('jade');

var conn_list_template = fs.readFileSync('./views/conn_list.jade', 'utf8');
var fn = jade.compile(conn_list_template);

function connection_mon(io) {
    wts.monitor(function(code) {
        var data = wts.enumerate();
        var json_data = eval('(' + data + ')');
        var html = fn(json_data);
        io.sockets.emit('refresh_connection', html);
        connection_mon(io);   // repeat
    });
}

exports.start = function(server) {
    var io = socketio.listen(server);
    var clients = 0;    // http://stackoverflow.com/questions/7352164/update-all-clients-using-socket-io
    io.set('log level', 2); // https://github.com/LearnBoost/Socket.IO/wiki/Configuring-Socket.IO
    io.set('transports', [
	    'websocket', 'flashsocket', 'htmlfile', 'xhr-polling', 'jsonp-polling'
      ]);

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

    //monitoring connection & disconnection
    connection_mon(io);

    return io;
};