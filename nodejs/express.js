var app = require('express').createServer(),
    io = require('socket.io').listen(app);
app.listen(80);
app.get('/', function (req, res) {
    res.sendfile(__dirname + '/index.html');
});
io.sockets.on('connection', function (socket) {
    //������Ϣ���ͻ���
    socket.emit('news', { hello: 'world' });
    socket.on('my other event', function (data) {
        console.log(data);
    });
    //�㲥��Ϣ������ǰ�û�֮����û�
    socket.broadcast.emit('user connected');
    //�㲥��ȫ��ͻ���
    io.sockets.emit('all users');
});