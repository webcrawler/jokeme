
// https://cnodejs.org/topic/4fb1c1fd1975fe1e1310490b

var HOST = '127.0.0.1';
var PORT = 6969;

var net = require('net');
var server = net.createServer();
server.listen(PORT, HOST);
console.log('Server is listening on');

server.on('connection', function(sock) {
    console.log('CONNECTED: ' + sock.remoteAddress +':'+ sock.remotePort);
   
	 sock.write('123456');
	 // 

});

/*
E:
cd E:\MyGithub\jokeme\nodejs\nodejs_studylog
node Net.js
*/