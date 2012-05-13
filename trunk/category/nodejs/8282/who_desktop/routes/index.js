var wts = require('wts');
var io = null;

exports.setSocketIO = function(sio) {
    io = sio;
};

/*
 * GET home page.
 */
exports.index = function(req, res) {
  var data = wts.enumerate();
  var json_data = eval('(' + data + ')');
  res.render('index', json_data)
};

/*
 * POST message page.
 */
exports.message = function(req, res) {
    var to = req.param('to');
    var msg = req.param('msg');
    
    try {
        var r = wts.message(parseInt(to, 10), msg);
        res.writeHead(200, { "Content-Type": "text/html" });
        res.write(r);
        res.end();
    } catch (e) {
        res.writeHead(200, { "Content-Type": "text/html" });
        res.write('<html><body><h1>Insufficien Parameter</h1>'
            + '<p>example: /message?to={number}&msg="{message}"</p></body>');
        res.end();
    }
};

/*
* POST chat page.
*/
exports.chat = function(req, res) {
    var name = req.param('name');
    var message = req.param('message');
    var date = req.param('date');
    var json = {'name': name, 'message': message, 'date': date};

    try {
        io.sockets.emit('message', json);
        res.writeHead(200, { "Content-Type": "text/html" });
        res.write('OK');
        res.end();
    } catch (e) {
        res.writeHead(200, { "Content-Type": "text/html" });
        res.write('<html><body><h1>Insufficien Parameter</h1>'
            + '<p>io.sockets.emit error</p></body>');
        res.end();
    }
};


/*
* POST disconnect page.
*/
exports.disconnect = function(req, res) {
    var sid = req.param('sid');

    try {
        var r = wts.disconnect(parseInt(sid, 10));
        res.writeHead(200, { "Content-Type": "text/html" });
        res.write(r);
        res.end();
    } catch (e) {
        res.writeHead(200, { "Content-Type": "text/html" });
        res.write('<html><body><h1>Insufficien Parameter</h1>'
            + '<p>example: /disconnect/{session idnumber}');
        res.end();
    }
};