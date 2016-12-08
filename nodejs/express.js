var app = require('express')()
 , server = require('http').createServer(app)
 , io = require('socket.io').listen(server);

server.listen(8080);

app.get('/', function (req, res) {
 //res.sendfile(__dirname + '/index.html');
   // Send HTML headers and message
  res.writeHead(200,{ 'Content-Type': 'text/html' });
  res.end('# Hello Socket Lover!');
});

io.sockets.on('connection', function (socket) {
 socket.emit('news', { hello: 'world' });
 socket.on('my other event', function (data) {
   console.log(data);
 });
});

/*
E:
cd E:\MyGithub\jokeme\nodejs
node express.js
*/