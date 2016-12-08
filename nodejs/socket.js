// 引入需要的模块：http和socket.io
var http = require('http'), io = require('socket.io');
//创建server
var server = http.createServer(function(req, res){
  // Send HTML headers and message
  res.writeHead(200,{ 'Content-Type': 'text/html' });
  res.end('# Hello Socket Lover!');
});
//端口8000
server.listen(8080);
//创建socket
var io = io.listen(server);
//添加连接监听
io.on('connection', function(socket){
	//socket.emit('news', "from server joke me!");
	socket.send('messages', "Hi I had get your req -- from server joke me!");
    //连接成功则执行下面的监听
    socket.on('message',function(event){
        console.log('Received message from client!',event);
    });
    //断开连接callback
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