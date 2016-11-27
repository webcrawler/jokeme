#ifndef MsgEventBase_h__
#define MsgEventBase_h__
#include <map>
#include "MessageEventManager.h"
#include "MessageType.h"

#define MsgEventSafeDelete(x) if (NULL != x) { delete(x); x = NULL; }


class CMemberFuncBase
{
public:
	virtual ~CMemberFuncBase() {}
	virtual bool operator()(const void *pArg) = 0;
};

template<class T>
class CMemberFuncSlot : public CMemberFuncBase
{
public:
	typedef bool(T::*MemberFunctionType)(const void *pArg);

	CMemberFuncSlot(MemberFunctionType func, T *obj) : 
		m_pObj(obj), 
		m_pMemberFunc(func)
	{}

	~CMemberFuncSlot() {}
	T* getObj() { return m_pObj; }
	bool operator()(const void *pArg)
	{
		if (NULL == m_pObj || NULL == m_pMemberFunc)
		{
			return false;
		}
		return (m_pObj->*m_pMemberFunc)(pArg);
	}
private:
	T *m_pObj;
	MemberFunctionType m_pMemberFunc;
};

class CMsgEventBase
{
public:


	typedef std::map<MessageType, CMemberFuncBase*> MAP_MSGFUNC;
	typedef MAP_MSGFUNC::iterator MAP_MSGFUNCITER;

public:
	CMsgEventBase(void) 
	{
		m_isDeleteFromManager = false;
	}

	virtual ~CMsgEventBase(void) 
	{
		if (!m_isDeleteFromManager /*&& 0 < m_mapMsgFunc.size()*/)
		{
			std::vector<MessageType> vecMsgType;
			MAP_MSGFUNC::iterator iter = m_mapMsgFunc.begin();
			for (;iter!=m_mapMsgFunc.end();iter++)
			{
				vecMsgType.push_back(iter->first);
			}
			for (unsigned int i=0;i<vecMsgType.size();i++)
			{
				removeMessage(vecMsgType[i]);
			}
		}
		removeAllMessage();
	}
private:
	template<class T>
	bool registerMessage(MessageType msgType, bool(T::*memberFunctionType)(const void *pArg), T *obj)
	{
		if (msgType > MT_JustSendToLastRegisterObjectBegin)
		{
			g_MsgEventManager.registerOneObjMessage((int)msgType, this);
			MAP_MSGFUNCITER itr = m_mapMsgFunc.find(msgType);
			if (itr != m_mapMsgFunc.end())
			{
				CMemberFuncBase* pOldObj = itr->second;
				CMemberFuncSlot<T>* pChildObj = (CMemberFuncSlot<T>* )pOldObj;
				if (pChildObj->getObj()!=obj)
				{
					MsgEventSafeDelete(itr->second);
					m_mapMsgFunc[msgType] = new CMemberFuncSlot<T>(memberFunctionType, obj);
					return true;
				}
			}
			else
			{
				m_mapMsgFunc[msgType] = new CMemberFuncSlot<T>(memberFunctionType, obj);
				return true;
			}
		}
		else
		{
			MAP_MSGFUNCITER itr = m_mapMsgFunc.find(msgType);
			if (itr != m_mapMsgFunc.end())
			{
				return false;
			}
			else
			{
				g_MsgEventManager.addMsgEventObj(msgType, this);
				m_mapMsgFunc[msgType] = new CMemberFuncSlot<T>(memberFunctionType, obj);
				return true;
			}
		}
		return false;
	}

protected:
	template<class T>
	bool registerMessageSelf(MessageType msgType, bool(T::*memberFunctionType)(const void *pArg))
	{
		return registerMessage(msgType, memberFunctionType, (T*)this);
	}

	bool removeMessage(MessageType msgType)
	{
		bool bHaveRemove=false;
		g_MsgEventManager.removeMsgEventObj_Normal(msgType,this,bHaveRemove);
		g_MsgEventManager.removeMsgEventObj_OneObj(msgType,this,bHaveRemove);

		MAP_MSGFUNCITER itr = m_mapMsgFunc.find(msgType);
		if (itr == m_mapMsgFunc.end())
		{
			return false;
		}
		MsgEventSafeDelete(itr->second);
		m_mapMsgFunc.erase(itr);
		return true;
	}

	bool removeAllMessage()
	{
		for (MAP_MSGFUNCITER itr = m_mapMsgFunc.begin(); itr != m_mapMsgFunc.end(); itr++)
		{
			MsgEventSafeDelete(itr->second);
		}
		m_mapMsgFunc.clear();
		return true;
	}
private:
	friend class CMessageEventManager;
	void setIsDeleteFromManager(bool isDelete)
	{
		m_isDeleteFromManager = isDelete;
	}

	bool fireEvent(MessageType msgType, const void *pEventArg)
	{
		MAP_MSGFUNCITER itr = m_mapMsgFunc.find(msgType);
		if (itr == m_mapMsgFunc.end())
		{
			return false;
		}
		if (itr->second != nullptr){
			return (*(itr->second))(pEventArg);
		}
		return true;
	}

private:
	MAP_MSGFUNC m_mapMsgFunc;
	bool m_isDeleteFromManager;
};
#endif // MsgEventBase_h__
