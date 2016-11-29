#include "MessageEventManager.h"
#include "MsgEventBase.h"
#include "cocos2d.h"
USING_NS_CC;

CMessageEventManager::CMessageEventManager(void)
{
	m_nFiringEvent = 0;
}

CMessageEventManager::~CMessageEventManager(void)
{
	removeAllMsgEventObj();
}

CMessageEventManager& CMessageEventManager::getInstance()
{
	static CMessageEventManager instance;
	return instance;
}

bool CMessageEventManager::addMsgEventObj( int msgType, CMsgEventBase *obj )
{
	if (NULL == obj)
	{
		return false;
	}
	else
	{
// 		if (m_bFiringEvent)
// 		{
// 			m_AddMsgEventObj_buffer.push_back(stADDEVENTOBJInfo(msgType,obj));
// 		}
// 		else
		{

			MAP_MSGEVENTOBJITR itr = m_mapMsgEventObj.find(msgType);
			if (itr != m_mapMsgEventObj.end())
			{
				MAP_EVENT_REMOVEOBJ::iterator iter = m_RemoveMsgEventObj_buffer.find(obj);
				if (iter!=m_RemoveMsgEventObj_buffer.end())
				{
					stREMOVEEVENTOBJInfo& info = iter->second;
					std::set<int>::iterator iterset = info.nVecMsgType.find(msgType);
					if (iterset!=info.nVecMsgType.end())
					{
						info.nVecMsgType.erase(iterset);
						if (info.nVecMsgType.empty())
						{
							m_RemoveMsgEventObj_buffer.erase(iter);
						}
						return true;
					}
				}

				SET_MSGEVENTOBJ &setEventObj = itr->second;
				if (!setEventObj.insert(obj).second)
				{
// 					ccDebugLog("CMessageEventManager::addMsgEventObj reinsert the obj:%d by event:%d", (int)obj, msgType);
				}
			}
			else
			{
				SET_MSGEVENTOBJ setEventObj;
				setEventObj.insert(obj);
				{
					m_mapMsgEventObj[msgType] = setEventObj;
				}
			}
		}
		return true;
	}
	return false;
}

bool CMessageEventManager::removeMsgEventObj_OneObj( int msgType, CMsgEventBase *obj ,bool& bHasRemove)
{
	if (NULL == obj)
	{
		return false;
	}
		bHasRemove=false;
	if (m_nFiringEvent>0)
	{
		InsertRemoveObj(obj, msgType);

	}
	else
	{
		//remove from one obj event map
		MAP_ONEOBJMSGITR oneObjItr = m_mapOneObjMsg.find(msgType);
		if (oneObjItr != m_mapOneObjMsg.end())
		{
			if (oneObjItr->second == obj)
			{
				m_mapOneObjMsg.erase(oneObjItr++);
				bHasRemove = true;
			}
			else
			{
				oneObjItr++;
			}
		}
	}
	return true;
}

bool CMessageEventManager::removeMsgEventObj_Normal( int msgType, CMsgEventBase *obj,bool& bHasRemove  )
{
	if (NULL == obj)
	{
		return false;
	}

	bHasRemove = false;
	if (m_nFiringEvent>0)
	{
		InsertRemoveObj(obj,msgType);
	}
	else
	{
		//remove from AllObjEvent map
		MAP_MSGEVENTOBJITR mapEventObjItr = m_mapMsgEventObj.find(msgType);
		if ( mapEventObjItr != m_mapMsgEventObj.end() )
		{
			SET_MSGEVENTOBJ &setEventObj = mapEventObjItr->second;
			SET_MSGEVENTOBJITR itr = setEventObj.find(obj);
			if (itr != setEventObj.end())
			{
				setEventObj.erase(itr);
			}
			if (0 == setEventObj.size())
			{
				m_mapMsgEventObj.erase(mapEventObjItr++);
				bHasRemove = true;
			}
			else
			{
				mapEventObjItr++;
			}
		}

	}
	return true;
}
// bool CMessageEventManager::removeMsgEventObj( CMsgEventBase *obj  )
// {
// 	if (NULL == obj)
// 	{
// 		return false;
// 	}
// 
// 	if (m_nFiringEvent>0)
// 	{
// 		m_RemoveMsgEventObj_buffer.insert(obj);
// 	}
// 	else
// 	{
// 		{
// 			if ( !m_mapOneObjMsg.empty() )
// 			{
// 				MAP_ONEOBJMSGITR oneObjItr = m_mapOneObjMsg.begin();
// 				for (;oneObjItr!=m_mapOneObjMsg.end();)
// 				{
// 					int nType = oneObjItr->first;
// 					bool bRemove = false;
// 					removeMsgEventObj_OneObj(nType,obj,bRemove);
// 					if ( bRemove )
// 					{
// 						oneObjItr=m_mapOneObjMsg.begin();
// 					}
// 					else
// 					{
// 						oneObjItr++;
// 					}
// 				}// for loop
// 			}// if ( !m_mapOneObjMsg.empty() )
// 		}
// 		{
// 			if ( !m_mapMsgEventObj.empty() )
// 			{
// 				MAP_MSGEVENTOBJITR mapEventObjItr = m_mapMsgEventObj.begin();
// 				for(; mapEventObjItr != m_mapMsgEventObj.end();  )
// 				{
// 					int nType = mapEventObjItr->first;
// 					bool bRemove = false;
// 					removeMsgEventObj_Normal(nType,obj,bRemove);			
// 					if ( bRemove )
// 					{
// 						mapEventObjItr=m_mapMsgEventObj.begin();
// 					}
// 					else
// 					{
// 						mapEventObjItr++;
// 					}
// 				}// for loop
// 			}// if ( !m_mapMsgEventObj.empty() )
// 		}
// 	}
// 	return true;
// }


bool CMessageEventManager::fireEvent( int msgType, const void *pEventArg )
{
	m_nFiringEvent ++;
	if (msgType > (int)MT_JustSendToLastRegisterObjectBegin)
	{
		MAP_ONEOBJMSGITR itr = m_mapOneObjMsg.find(msgType);
		if (itr != m_mapOneObjMsg.end())
		{
			itr->second->fireEvent((MessageType)msgType, pEventArg);
		}
	}
	else
	{
		MAP_MSGEVENTOBJITR itr = m_mapMsgEventObj.find(msgType);
		if (itr != m_mapMsgEventObj.end())
		{
			SET_MSGEVENTOBJ &setMsgEventObj = itr->second;
			SET_MSGEVENTOBJ copyEvent;
			copyEvent = setMsgEventObj;
			for (SET_MSGEVENTOBJITR itrSet = copyEvent.begin(); itrSet != copyEvent.end(); itrSet++)
			{
				CMsgEventBase* pObj = *itrSet;
				MAP_EVENT_REMOVEOBJ::iterator iter = m_RemoveMsgEventObj_buffer.find(pObj);
				if (iter!=m_RemoveMsgEventObj_buffer.end())
				{
					stREMOVEEVENTOBJInfo& info = iter->second;
					if (info.nVecMsgType.find(msgType)!=info.nVecMsgType.end() )
					{
						continue;
					}
				}
				(*itrSet)->fireEvent((MessageType)msgType, pEventArg);
			}
		}
	}
	m_nFiringEvent--;
	return true;
}

bool CMessageEventManager::registerOneObjMessage( int msgType, CMsgEventBase *obj )
{
	m_mapOneObjMsg[msgType] = obj;
	return true;
}

void CMessageEventManager::removeAllMsgEventObj()
{
	MAP_MSGEVENTOBJITR mapEventObjItr = m_mapMsgEventObj.begin();
	while(mapEventObjItr != m_mapMsgEventObj.end())
	{
		SET_MSGEVENTOBJ &setEventObj = mapEventObjItr->second;
		for (SET_MSGEVENTOBJITR itr = setEventObj.begin(); itr != setEventObj.end(); itr++)
		{
			CMsgEventBase *obj = *itr;
			obj->setIsDeleteFromManager(true);
		}
		setEventObj.clear();
		mapEventObjItr++;
	}
	m_mapMsgEventObj.clear();

	MAP_ONEOBJMSGITR oneObjItr = m_mapOneObjMsg.begin();
	while (oneObjItr != m_mapOneObjMsg.end())
	{
		CMsgEventBase *obj = oneObjItr->second;
		obj->setIsDeleteFromManager(true);
		oneObjItr++;
	}
	m_mapOneObjMsg.clear();
}

void CMessageEventManager::update( float delta )
{
	if (m_AddMsgEventObj_buffer.size()>0)
	{
		std::vector<stADDEVENTOBJInfo>::iterator iter=m_AddMsgEventObj_buffer.begin();
		for (;iter!=m_AddMsgEventObj_buffer.end();iter++)
		{
			stADDEVENTOBJInfo& info = *iter;
			this->addMsgEventObj(info.nMsgType,info.pEventobj);
		}
		m_AddMsgEventObj_buffer.clear();
	}
	
	if (m_RemoveMsgEventObj_buffer.size()>0)
	{
		MAP_EVENT_REMOVEOBJ::iterator iter=m_RemoveMsgEventObj_buffer.begin();
		for (;iter!=m_RemoveMsgEventObj_buffer.end();iter++)
		{
			stREMOVEEVENTOBJInfo& info = iter->second;
			for (std::set<int>::iterator iterset = info.nVecMsgType.begin();iterset!=info.nVecMsgType.end();iterset++)
			{
				bool bHasRemove = true;
				int nMsgtype = *iterset;
				this->removeMsgEventObj_Normal(nMsgtype,info.pEventobj,bHasRemove);
				this->removeMsgEventObj_OneObj(nMsgtype,info.pEventobj,bHasRemove);
			}
			
		}
		m_RemoveMsgEventObj_buffer.clear();
	}
}

void CMessageEventManager::InsertRemoveObj( CMsgEventBase * obj, int msgType )
{
	MAP_EVENT_REMOVEOBJ::iterator iter = m_RemoveMsgEventObj_buffer.find(obj);
	if (iter!=m_RemoveMsgEventObj_buffer.end())
	{
		stREMOVEEVENTOBJInfo& info = iter->second;
		std::set<int>::iterator iterset =  info.nVecMsgType.find(msgType);
		if (iterset==info.nVecMsgType.end())
		{
			info.nVecMsgType.insert(msgType);
		}
		else
		{
// 			ccDebugLog("## error re insert same type 10002");
		}
	}
	else
	{
		m_RemoveMsgEventObj_buffer[obj]=stREMOVEEVENTOBJInfo(obj);
		stREMOVEEVENTOBJInfo& info = m_RemoveMsgEventObj_buffer[obj];
		info.nVecMsgType.insert(msgType);
	}
}
