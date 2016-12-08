// ������Ҫ��ģ�飺http��socket.io
var http = require('http'), io = require('socket.io');
//����server
var server = http.createServer(function(req, res){
  // Send HTML headers and message
  res.writeHead(200,{ 'Content-Type': 'text/html' });
  res.end('# Hello Socket Lover!');
});
//�˿�8000
server.listen(8080);
//����socket
var io = io.listen(server);
//������Ӽ���
io.on('connection', function(socket){
	//socket.emit('news', "from server joke me!");
	socket.send('messages', "Hi I had get your req -- from server joke me!");
    //���ӳɹ���ִ������ļ���
    socket.on('message',function(event){
        console.log('Received message from client!',event);
    });
    //�Ͽ�����callback
    socket.on('disconnect',function(){
        console.log('Server has disconnected');
    });
	
	socket.on('login',function(event){
        console.log('yes you had login success!',event);
    });
	socket.send('login', "yes you had login success!");
});

/*
E:
cd E:\MyGithub\jokeme\nodejs
node socket.js
*/