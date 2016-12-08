/**
*  @Brief: socketio
*  @Created by fjut on 16-12-07
*/

#include "SocketIOManager.h"

SocketIOManager::SocketIOManager()
{
	m_sioClient = nullptr;
}

SocketIOManager& SocketIOManager::getInstance()
{
	static SocketIOManager _instance;
	return _instance;
}

void SocketIOManager::destroyInstance()
{

}

void SocketIOManager::testSocketOpen()
{
	//create a client by using this static method, url does not need to contain the protocol
	//_sioClient = SocketIO::connect("ws://tools.itharbors.com:4000", *this);
	m_sioClient = SocketIO::connect("ws://localhost:8080", *this);
	//you may set a tag for the client for reference in callbacks
	m_sioClient->setTag("Test Client");

	//register event callbacks using the CC_CALLBACK_2() macro and passing the instance of the target class
	m_sioClient->on("testevent", CC_CALLBACK_2(SocketIOManager::testevent, this));
	m_sioClient->on("echotest", CC_CALLBACK_2(SocketIOManager::echotest, this));
	m_sioClient->on("message", CC_CALLBACK_2(SocketIOManager::message, this));
	m_sioClient->on("json", CC_CALLBACK_2(SocketIOManager::json, this));
	m_sioClient->on("connect", CC_CALLBACK_2(SocketIOManager::connect, this));
	m_sioClient->on("disconnect", CC_CALLBACK_2(SocketIOManager::disconnect, this));

	m_sioClient->on("heartbeat", [=](SIOClient* client, const std::string& data) {
		CCLOG("heartbeat: %d", data.c_str());
	});
}

void SocketIOManager::testSocketSend()
{
	if (m_sioClient != nullptr) m_sioClient->send("Hello jokeme!");
}

void SocketIOManager::testSocketEmit()
{
	if (m_sioClient != nullptr) m_sioClient->emit("login", "{\"name\":\"fjut\",\"type\":\"hehe\"}");
}

void SocketIOManager::disconnect1()
{
	if (m_sioClient != nullptr) m_sioClient->disconnect();
}

//test event callback handlers, these will be registered with socket.io
void SocketIOManager::testevent(SIOClient *client, const std::string& data) 
{
	CCLOGINFO("SocketIOManager::testevent called with data: %s", data.c_str());
	std::stringstream s;
	s << client->getTag() << " received event testevent with data: " << data.c_str();
}

void SocketIOManager::echotest(SIOClient *client, const std::string& data) 
{
	CCLOGINFO("SocketIOManager::echotest called with data: %s", data.c_str());

	std::stringstream s;
	s << client->getTag() << " received event echotest with data: " << data.c_str();
}

// onMessage is no longer a required override from the delegate class
// 'message' events and handlers are now registered in the same way that other events are
void SocketIOManager::message(SIOClient* client, const std::string& data)
{
	CCLOGINFO("SocketIOManager::message received: %s", data.c_str());

	std::stringstream s;
	s << client->getTag() << " received message with content: " << data.c_str();
}

void SocketIOManager::json(SIOClient* client, const std::string& data)
{
	CCLOGINFO("SocketIOManager::json received: %s", data.c_str());

	std::stringstream s;
	s << client->getTag() << " received json message with content: " << data.c_str();
}

void SocketIOManager::connect(SIOClient* client, const std::string& data)
{
	CCLOGINFO("SocketIOManager::connect called");

	std::stringstream s;
	s << client->getTag() << " connected!";

	// test
	//testSocketSend();
	testSocketEmit();
}

void SocketIOManager::disconnect(SIOClient* client, const std::string& data)
{
	CCLOGINFO("SocketIOManager::disconnect called");

	std::stringstream s;
	s << client->getTag() << " disconnected by server!";

	this->closedSocketAction(client);
}

void SocketIOManager::closedSocketAction(SIOClient* client)
{
	//set the local pointer to nullptr or connect to another client
	//the client object will be released on its own after this method completes
	if (client == m_sioClient) 
	{
		m_sioClient = nullptr;
	}
}

// SIODelegate methods to catch network/socket level events outside of the socket.io events
void SocketIOManager::onClose(SIOClient* client)
{
	CCLOGINFO("SocketIOTest::onClose called");

	std::stringstream s;
	s << client->getTag() << " closed!";

	this->closedSocketAction(client);
}

void SocketIOManager::onError(SIOClient* client, const std::string& data)
{
	CCLOGERROR("SocketIOTest::onError received: %s", data.c_str());

	std::stringstream s;
	s << client->getTag() << " received error with content: " << data.c_str();
}