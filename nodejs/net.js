ws.on('connection', function(client)
{
    // 广播函数
    var broadcast = function(msg, cl){
        for(var k in ws.sockets.sockets){
            if(ws.sockets.sockets.hasOwnProperty(k)){
                if(ws.sockets.sockets[k] && ws.sockets.sockets[k].id != cl.id){
                    ws.sockets.sockets[k].emit('position.change', msg);
                }
            }
        }
    };
    console.log('\033[92m有新的连接来:\033[39m', postions);
    // 客户端连接成功之后，就发送其它客户端的坐标信息
    client.emit('position.change', postions);
    // 接收客户端发送消息
    client.on('position.change', function(msg){
        // 目前客户端的消息就只有坐标消息
        postions[client.id] = msg;
        // 把消息广播给其它所有的客户端
        broadcast({
            type: 'position',
            postion: msg,
            id: client.id
        }, client);
    });
    // 接收客户端关闭连接消息
    client.on('close', function(){
        console.log('close!');
        // 删除客户端，并通知其它客户端
        delete postions[client.id];
        // 把消息广播给其它所有的客户端
        broadcast({
            type: 'disconnect',
            id: client.id
        }, client);
    });
    // 断开连接
    client.on('disconnect', function(){
        console.log('disconnect!');
        // 删除客户端，并通知其它客户端
        delete postions[client.id];
        // 把消息广播给其它所有的客户端
        broadcast({
            type: 'disconnect',
            id: client.id
        }, client);
    })
    // 定义客户端异常处理
    client.on('error', function(err){
        console.log('error->', err);
    })
});