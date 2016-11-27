#ifndef MessageEventManager_h__
#define MessageEventManager_h__

#include <map>
#include <set>
#include <vector>
#include "MessageType.h"

#define g_MsgEventManager CMessageEventManager::getInstance()

class CMsgEventBase;
class CMessageEventManager 
{
public:
	~CMessageEventManager(void);
private:
	CMessageEventManager(void);
public:
	static CMessageEventManager& getInstance();
	bool fireEvent(int msgType, const void *pEventArg);
	void update(float delta);
private:
	friend class CMsgEventBase;
	bool addMsgEventObj(int msgType, CMsgEventBase *obj);
	bool removeMsgEventObj_Normal( int msgType, CMsgEventBase *obj,bool& bHasRemove );
	bool removeMsgEventObj_OneObj( int msgType, CMsgEventBase *obj ,bool& bHasRemove);

	void InsertRemoveObj( CMsgEventBase * obj, int msgType );

	//! 掉这个接口可能导致事件系统出错，请慎重调用?!!!
	bool removeMsgEventObj( CMsgEventBase *obj );
	void removeAllMsgEventObj();

	bool registerOneObjMessage(int msgType, CMsgEventBase *obj);
private:
	typedef std::set<CMsgEventBase*> SET_MSGEVENTOBJ;
	typedef SET_MSGEVENTOBJ::iterator SET_MSGEVENTOBJITR;

	typedef std::map<int, SET_MSGEVENTOBJ> MAP_MSGEVENTOBJ;
	typedef MAP_MSGEVENTOBJ::iterator MAP_MSGEVENTOBJITR;
	MAP_MSGEVENTOBJ m_mapMsgEventObj;

	struct stADDEVENTOBJInfo
	{
		stADDEVENTOBJInfo(int nType,CMsgEventBase* pObj):nMsgType(nType),pEventobj(pObj) {}
		int nMsgType;
		CMsgEventBase* pEventobj;
	};
	struct stREMOVEEVENTOBJInfo
	{
		stREMOVEEVENTOBJInfo():pEventobj(NULL){}
		stREMOVEEVENTOBJInfo(CMsgEventBase* pObj):pEventobj(pObj) {}
		CMsgEventBase*	pEventobj;
		std::set<int>	nVecMsgType;
	};

	std::vector<stADDEVENTOBJInfo> m_AddMsgEventObj_buffer;
	typedef std::map<CMsgEventBase*,stREMOVEEVENTOBJInfo> MAP_EVENT_REMOVEOBJ;
	MAP_EVENT_REMOVEOBJ m_RemoveMsgEventObj_buffer;

	typedef std::map<int, CMsgEventBase*> MAP_ONEOBJMSG;
	typedef MAP_ONEOBJMSG::iterator MAP_ONEOBJMSGITR;
	MAP_ONEOBJMSG	m_mapOneObjMsg;
	int				m_nFiringEvent;
};
#endif // MessageEventManager_h__
