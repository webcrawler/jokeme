var app = require('express').createServer(),
    io = require('socket.io').listen(app);
app.listen(80);
app.get('/', function (req, res) {
    res.sendfile(__dirname + '/index.html');
});
io.sockets.on('connection', function (socket) {
    //发送消息给客户端
    socket.emit('news', { hello: 'world' });
    socket.on('my other event', function (data) {
        console.log(data);
    });
    //广播信息给除当前用户之外的用户
    socket.broadcast.emit('user connected');
    //广播给全体客户端
    io.sockets.emit('all users');
});