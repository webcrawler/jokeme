/**
*  @Brief: socketio
*  @Created by fjut on 16-12-07
*/

#ifndef __H_SOCKET_IO_MANAGER_H__
#define __H_SOCKET_IO_MANAGER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/SocketIO.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::network;

class SocketIOManager: public cocos2d::network::SocketIO::SIODelegate
{
public:
	SocketIOManager();
	static SocketIOManager& getInstance();
	static void destroyInstance();

public:
	void testSocketOpen();
	void testSocketSend();
	void testSocketEmit();

private:
	/**
	*  @brief Used for network level socket close (not for disconnect from the socket.io server)
	*/
	virtual void onClose(cocos2d::network::SIOClient* client)override;
	/**
	*  @brief Used for network level socket error (not for disconnect from the socket.io server)
	**/
	virtual void onError(cocos2d::network::SIOClient* client, const std::string& data)override;
	/**
	*  @brief Common function to call on both socket.io disconnect and websocket close
	**/
	void closedSocketAction(cocos2d::network::SIOClient* client);

private:
	/**
	*  @brief Socket.io event handler for custom event "testevent"
	**/
	void testevent(cocos2d::network::SIOClient *client, const std::string& data);
	/**
	*  @brief Socket.io event handler for custom event "echoevent"
	**/
	void echotest(cocos2d::network::SIOClient *client, const std::string& data);
	/**
	*  @brief Socket.io event handler for event "connect"
	**/
	void connect(cocos2d::network::SIOClient* client, const std::string& data);
	/**
	*  @brief Socket.io event handler for event "disconnect"
	**/
	void disconnect(cocos2d::network::SIOClient* client, const std::string& data);
	/**
	*  @brief Socket.io event handler for event "message"
	**/
	void message(cocos2d::network::SIOClient* client, const std::string& data);
	/**
	*  @brief Socket.io event handler for event "json"
	*         This is only used in v 0.9.x, in 1.x this is handled as a "message" event
	**/
	void json(cocos2d::network::SIOClient* client, const std::string& data);

private:
	cocos2d::network::SIOClient* m_sioClient;

};

#define g_socketio SocketIOManager::getInstance()

#endif 
