
/**
 * Module dependencies.
 */

var express = require('express')
  , routes = require('./routes');
var sock_server = require("./sock_server");

var app = module.exports = express.createServer();

// Configuration

app.configure(function(){
  app.set('views', __dirname + '/views');
  app.set('view engine', 'jade');
  app.set('view options',{
      layout: false
  });
  app.use(express.bodyParser());
  app.use(express.methodOverride());
  app.use(app.router);
  app.use(express.static(__dirname + '/public'));
});

app.configure('development', function(){
  app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
});

app.configure('production', function(){
  app.use(express.errorHandler());
});

// Routes

app.get('/', routes.index);
app.post('/message', routes.message);
app.post('/disconnect/:sid', routes.disconnect);
app.post('/chat', routes.chat);

app.listen(8282, function(){
  console.log("Express server listening on port %d in %s mode", app.address().port, app.settings.env);
});


// Create: Socket Server
routes.setSocketIO(sock_server.start(app));