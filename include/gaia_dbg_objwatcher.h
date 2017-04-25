#ifndef		__GAIA_DBG_OBJWATCHER_H__
#define		__GAIA_DBG_OBJWATCHER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_stream_stdstream.h"
#include "gaia_thread_base.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_algo_compare.h"
#include "gaia_algo_string.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_hashmap.h"
#include "gaia_ctn_stringptrpool.h"
#include "gaia_time.h"
#include "gaia_log.h"

/*
	ObjType predeclaration :

		Default C++ class	0
		GAIA::Base			1
		GAIA::Entity		2
		GAIA::Object		3
		GAIA::RefObject		4
		Other class			N
*/

extern GAIA::LOG::Log g_gaia_log;
extern GAIA::LOG::InvalidLog g_gaia_invalidlog;

namespace GAIA
{
	namespace DBG
	{
		static const GAIA::CH* OBJ_WATCHER_CTOR = "ctor"; // Constructor.
		static const GAIA::CH* OBJ_WATCHER_DTOR = "dtor"; // Destructor.

		class ObjWatcher : public GAIA::Base
		{
		public:
			class Record : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					p = GNIL;
					uid.clear();
					sObjType = GINVALID;
					uThreadID = 0;
					uTime = 0;
					szInfo[0] = '\0';
					pszInfo = GNIL;
				}
				GINL GAIA::N32 compare(const Record& src) const
				{
					if(uTime < src.uTime)
						return -1;
					else if(uTime > src.uTime)
						return +1;
					return 0;
				}
				GCLASS_COMPARE_BYCOMPARE(Record)

			public:
				const GAIA::GVOID* p;
				GAIA::X128 uid;
				GAIA::NUM sObjType;
				GAIA::UM uThreadID;
				GAIA::U64 uTime;
				GAIA::CH szInfo[33];
				const GAIA::CH* pszInfo;
			};
			typedef GAIA::CTN::Vector<Record> __RecordVector;

			class Obj : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					p = GNIL;
					records.clear();
					nBeginCount = 0;
					bBegin = GAIA::False;
				}
				GINL GAIA::N32 compare(const Obj& src) const
				{
					if(p < src.p)
						return -1;
					else if(p > src.p)
						return +1;
					return 0;
				}
				GCLASS_COMPARE_BYCOMPARE(Obj)

			public:
				const GAIA::GVOID* p;
				__RecordVector records;
				GAIA::N32 nBeginCount;
				GAIA::BL bBegin;
			};

		public:
			GINL ObjWatcher()
			{
				m_uRecycleTime = 1000 * 1000 * 60;
				m_sRecycleBeginCount = 16;
				m_pLog = GNIL;
				m_bEnableLog = GAIA::False;
			}
			GINL ~ObjWatcher()
			{
				//// NOT NEED RECYCLE ALL THE STRING TO STRPOOL, BECAUSE OF STRING POOL IS THE MEMBER VARIABLES.
			}
			GINL GAIA::GVOID Reset()
			{
				GAIA::SYNC::Autolock al(m_lr);
				m_objpool.destroy();
				m_strpool.destroy();
				m_objs.destroy();
			}
			GINL GAIA::GVOID SetRecycleTime(const GAIA::U64& uRecycleTime){m_uRecycleTime = uRecycleTime;}
			GINL const GAIA::U64& GetRecycleTime() const{return m_uRecycleTime;}
			GINL GAIA::GVOID SetRecycleBeginCount(GAIA::NUM sRecycleBeginCount){m_sRecycleBeginCount = sRecycleBeginCount;}
			GINL GAIA::NUM GetRecycleBeginCount() const{return m_sRecycleBeginCount;}
			GINL GAIA::GVOID SetLog(GAIA::LOG::Log* pLog){m_pLog = pLog;}
			GINL GAIA::LOG::Log* GetLog() const{return m_pLog;}
			GINL GAIA::GVOID EnableLog(GAIA::BL bEnable){m_bEnableLog = bEnable;}
			GINL GAIA::BL IsEnableLog() const{return m_bEnableLog;}
			GINL GAIA::BL Begin(const GAIA::GVOID* p, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID)
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GAIA::False;

				GAIA::SYNC::Autolock al(m_lr);
				Obj* pObj;
				Obj** ppObj = m_objs.find(p);
				if(ppObj == GNIL)
				{
					pObj = m_objpool.alloc();
					pObj->reset();
					pObj->p = p;
					m_objs.insert(p, pObj);
				}
				else
				{
					pObj = *ppObj;
					GAST(pObj != GNIL);
				}

				if(pObj->bBegin)
				{
					GAIA::LOG::Log& l = this->select_log();
					l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
					l << "Watch " << (GAIA::U64)p << ", found a error: Cannot begin again";
					l << ", Tid=" << GAIA::THREAD::threadid();
					if(uid != GNIL)
					{
						GAIA::CH szUid[33];
						uid->tostring(szUid);
						l << ", NewUid=" << szUid;
					}
					if(sObjType != GINVALID)
						l << ", NewObjType=" << sObjType;
					l << ", see the records followed for more details";
					l << l.End();
					this->logout_obj(*pObj);
					GAST(!pObj->bBegin);
					return GAIA::False;
				}

				pObj->nBeginCount++;
				pObj->bBegin = GAIA::True;

				Record r;
				r.p = p;
				if(uid != GNIL)
					r.uid = *uid;
				else
					r.uid.uuid();
				r.sObjType = sObjType;
				r.uThreadID = GAIA::THREAD::threadid();
				r.uTime = GAIA::TIME::process_time();
				r.pszInfo = OBJ_WATCHER_CTOR;
				pObj->records.push_back(r);

				if(m_bEnableLog)
					this->logout_record(r);

				return GAIA::True;
			}
			GINL GAIA::BL End(const GAIA::GVOID* p, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID)
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GAIA::False;

				GAIA::SYNC::Autolock al(m_lr);
				Obj** ppObj = m_objs.find(p);
				GAST(ppObj != GNIL);
				if(ppObj == GNIL)
					return GAIA::False;

				Obj* pObj = *ppObj;
				GAST(pObj != GNIL);

				if(!pObj->bBegin)
				{
					GAIA::LOG::Log& l = this->select_log();
					l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
					l << "Watch " << (GAIA::U64)p << ", found a error: Cannot end a when have not began";
					l << ", Tid=" << GAIA::THREAD::threadid();
					if(uid != GNIL)
					{
						GAIA::CH szUid[33];
						uid->tostring(szUid);
						l << ", NewUid=" << szUid;
					}
					if(sObjType != GINVALID)
						l << ", NewObjType=" << sObjType;
					l << ", see the records followed for more details";
					l << l.End();
					this->logout_obj(*pObj);
					GAST(pObj->bBegin);
					return GAIA::False;
				}

				if(!pObj->records.empty())
				{
					if(uid != GNIL)
					{
						if(pObj->records.back().uid != *uid)
						{
							GAIA::LOG::Log& l = this->select_log();
							l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
							l << "Watch " << (GAIA::U64)p << ", found a error: Last uuid is not match when end";
							l << ", Tid=" << GAIA::THREAD::threadid();
							if(uid != GNIL)
							{
								GAIA::CH szUid[33];
								uid->tostring(szUid);
								l << ", NewUid=" << szUid;
							}
							if(sObjType != GINVALID)
								l << ", NewObjType=" << sObjType;
							l << ", see the records followed for more details";
							l << l.End();
							this->logout_obj(*pObj);
							GAST(pObj->records.back().uid == *uid);
							return GAIA::False;
						}
					}

					if(sObjType != GINVALID)
					{
						if(pObj->records.back().sObjType != sObjType)
						{
							GAIA::LOG::Log& l = this->select_log();
							l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
							l << "Watch " << (GAIA::U64)p << ", found a error: Last ObjType is not match when end";
							l << ", Tid=" << GAIA::THREAD::threadid();
							if(uid != GNIL)
							{
								GAIA::CH szUid[33];
								uid->tostring(szUid);
								l << ", NewUid=" << szUid;
							}
							if(sObjType != GINVALID)
								l << ", NewObjType=" << sObjType;
							l << ", see the records followed for more details";
							l << l.End();
							this->logout_obj(*pObj);
							GAST(pObj->records.back().sObjType == sObjType);
							return GAIA::False;
						}
					}
				}
				else
				{
					GAST(!pObj->records.empty());
					return GAIA::False;
				}

				pObj->bBegin = GAIA::False;

				Record r;
				r.p = p;
				if(uid != GNIL)
					r.uid = *uid;
				else
					r.uid = pObj->records.back().uid;
				if(sObjType != GINVALID)
					r.sObjType = sObjType;
				else
					r.sObjType = pObj->records.back().sObjType;
				r.uThreadID = GAIA::THREAD::threadid();
				r.uTime = GAIA::TIME::process_time();
				r.pszInfo = OBJ_WATCHER_DTOR;
				pObj->records.push_back(r);

				if(m_bEnableLog)
					this->logout_record(r);

				return GAIA::True;
			}
			GINL GAIA::BL IsBegin(const GAIA::GVOID* p, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID) const
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GAIA::False;

				GAIA::SYNC::Autolock al(GCCAST(ObjWatcher*)(this)->m_lr);
				const ObjPtr* ppObj = m_objs.find(p);
				GAST(ppObj != GNIL);
				if(ppObj == GNIL)
					return GAIA::False;

				Obj* pObj = *ppObj;
				GAST(pObj != GNIL);

				if(!pObj->bBegin)
					return GAIA::False;

				if(!pObj->records.empty())
				{
					if(uid != GNIL)
					{
						if(pObj->records.back().uid != *uid)
						{
							GAIA::LOG::Log& l = this->select_log();
							l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
							l << "Watch " << (GAIA::U64)p << ", found a error: Last uuid is not match when check began";
							l << ", Tid=" << GAIA::THREAD::threadid();
							if(uid != GNIL)
							{
								GAIA::CH szUid[33];
								uid->tostring(szUid);
								l << ", NewUid=" << szUid;
							}
							if(sObjType != GINVALID)
								l << ", NewObjType=" << sObjType;
							l << ", see the records followed for more details";
							l << l.End();
							this->logout_obj(*pObj);
							GAST(pObj->records.back().uid == *uid);
							return GAIA::False;
						}
					}

					if(sObjType != GINVALID)
					{
						if(pObj->records.back().sObjType != sObjType)
						{
							GAIA::LOG::Log& l = this->select_log();
							l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
							l << "Watch " << (GAIA::U64)p << ", found a error: Last ObjType is not match when check began";
							l << ", Tid=" << GAIA::THREAD::threadid();
							if(uid != GNIL)
							{
								GAIA::CH szUid[33];
								uid->tostring(szUid);
								l << ", NewUid=" << szUid;
							}
							if(sObjType != GINVALID)
								l << ", NewObjType=" << sObjType;
							l << ", see the records followed for more details";
							l << l.End();
							this->logout_obj(*pObj);
							GAST(pObj->records.back().sObjType == sObjType);
							return GAIA::False;
						}
					}
				}
				else
				{
					GAST(!pObj->records.empty());
					return GAIA::False;
				}

				return GAIA::True;
			}
			GINL GAIA::BL IsExist(const GAIA::GVOID* p, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID) const
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GAIA::False;

				GAIA::SYNC::Autolock al(GCCAST(ObjWatcher*)(this)->m_lr);
				const ObjPtr* ppObj = m_objs.find(p);
				if(ppObj == GNIL)
					return GAIA::False;

				Obj* pObj = *ppObj;
				GAST(pObj != GNIL);

				if(uid != GNIL && sObjType != GINVALID)
				{
					for(GAIA::NUM x = 0; x < pObj->records.size(); ++x)
					{
						const Record& r = pObj->records[x];
						if(r.uid == *uid && r.sObjType == sObjType)
							return GAIA::True;
					}
				}
				else if(uid != GNIL && sObjType == GINVALID)
				{
					for(GAIA::NUM x = 0; x < pObj->records.size(); ++x)
					{
						const Record& r = pObj->records[x];
						if(r.uid == *uid)
							return GAIA::True;
					}
				}
				else if(uid == GNIL && sObjType != GINVALID)
				{
					for(GAIA::NUM x = 0; x < pObj->records.size(); ++x)
					{
						const Record& r = pObj->records[x];
						if(r.sObjType == sObjType)
							return GAIA::True;
					}
				}
				else
					return GAIA::True;

				return GAIA::False;
			}
			GINL GAIA::BL Update(const GAIA::GVOID* p, const GAIA::CH* pszInfo, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID)
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GAIA::False;

				GAST(pszInfo != GNIL);
				if(pszInfo == GNIL)
					return GAIA::False;

				GAIA::SYNC::Autolock al(m_lr);
				Obj** ppObj = m_objs.find(p);
				GAST(ppObj != GNIL);
				if(ppObj == GNIL)
					return GAIA::False;

				Obj* pObj = *ppObj;
				GAST(pObj != GNIL);

				if(!pObj->bBegin)
				{
					GAIA::LOG::Log& l = this->select_log();
					l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
					l << "Watch " << (GAIA::U64)p << ", found a error: Cannot update when have not began";
					l << ", Tid=" << GAIA::THREAD::threadid();
					l << ", Info=" << pszInfo;
					if(uid != GNIL)
					{
						GAIA::CH szUid[33];
						uid->tostring(szUid);
						l << ", NewUid=" << szUid;
					}
					if(sObjType != GINVALID)
						l << ", NewObjType=" << sObjType;
					l << ", see the records followed for more details";
					l << l.End();
					this->logout_obj(*pObj);
					GAST(pObj->bBegin);
					return GAIA::False;
				}

				if(!pObj->records.empty())
				{
					if(uid != GNIL)
					{
						if(pObj->records.back().uid != *uid)
						{
							GAIA::LOG::Log& l = this->select_log();
							l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
							l << "Watch " << (GAIA::U64)p << ", found a error: Last uuid is not match when update";
							l << ", Tid=" << GAIA::THREAD::threadid();
							l << ", Info=" << pszInfo;
							if(uid != GNIL)
							{
								GAIA::CH szUid[33];
								uid->tostring(szUid);
								l << ", NewUid=" << szUid;
							}
							if(sObjType != GINVALID)
								l << ", NewObjType=" << sObjType;
							l << ", see the records followed for more details";
							l << l.End();
							this->logout_obj(*pObj);
							GAST(pObj->records.back().uid == *uid);
							return GAIA::False;
						}
					}

					if(sObjType != GINVALID)
					{
						if(pObj->records.back().sObjType != sObjType)
						{
							GAIA::LOG::Log& l = this->select_log();
							l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
							l << "Watch " << (GAIA::U64)p << ", found a error: Last ObjType is not match when update";
							l << ", Tid=" << GAIA::THREAD::threadid();
							l << ", Info=" << pszInfo;
							if(uid != GNIL)
							{
								GAIA::CH szUid[33];
								uid->tostring(szUid);
								l << ", NewUid=" << szUid;
							}
							if(sObjType != GINVALID)
								l << ", NewObjType=" << sObjType;
							l << ", see the records followed for more details";
							l << l.End();
							this->logout_obj(*pObj);
							GAST(pObj->records.back().sObjType == sObjType);
							return GAIA::False;
						}
					}
				}
				else
				{
					GAST(!pObj->records.empty());
					return GAIA::False;
				}

				Record r;
				r.p = p;
				if(uid != GNIL)
					r.uid = *uid;
				else
					r.uid = pObj->records.back().uid;
				if(sObjType != GINVALID)
					r.sObjType = sObjType;
				else
					r.sObjType = pObj->records.back().sObjType;
				r.uThreadID = GAIA::THREAD::threadid();
				r.uTime = GAIA::TIME::process_time();
				GAIA::NUM sInfoLen = GAIA::ALGO::gstrlen(pszInfo);
				if(sInfoLen < sizeof(r.szInfo))
				{
					r.pszInfo = r.szInfo;
					GAIA::ALGO::gstrcpy((GAIA::CH*)r.pszInfo, pszInfo);
				}
				else
					r.pszInfo = m_strpool.alloc(pszInfo);
				pObj->records.push_back(r);

				if(m_bEnableLog)
					this->logout_record(r);

				return GAIA::True;
			}
			GINL GAIA::BL CollectRecords(GAIA::CTN::Vector<Record>& listResult, const GAIA::GVOID* p = GNIL, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID)
			{
				listResult.clear();

				GAIA::SYNC::Autolock al(m_lr);

				if(p == GNIL)
				{
					for(__ObjHashMap::it it = m_objs.frontit(); !it.empty(); ++it)
					{
						Obj* pObj = *it;
						GAST(pObj != GNIL);
						for(__RecordVector::it itt = pObj->records.frontit(); !itt.empty(); ++itt)
						{
							const Record& r = *itt;
							if(uid != GNIL)
							{
								if(r.uid != *uid)
									continue;
							}
							if(sObjType != GINVALID)
							{
								if(r.sObjType != sObjType)
									continue;
							}
							listResult.push_back(r);
						}
					}
				}
				else
				{
					Obj** ppObj = m_objs.find(p);
					if(ppObj != GNIL)
					{
						Obj* pObj = *ppObj;
						GAST(pObj != GNIL);

						for(__RecordVector::it it = pObj->records.frontit(); !it.empty(); ++it)
						{
							const Record& r = *it;
							if(uid != GNIL)
							{
								if(r.uid != *uid)
									continue;
							}
							if(sObjType != GINVALID)
							{
								if(r.sObjType != sObjType)
									continue;
							}
							listResult.push_back(r);
						}
					}
				}

				if(listResult.empty())
					return GAIA::False;

				listResult.sort();

				return GAIA::True;
			}
			GINL GAIA::BL Recycle()
			{
				GAIA::U64 uCurrentTime = GAIA::TIME::process_time();

				GAIA::SYNC::Autolock al(m_lr);

				for(__ObjHashMap::it it = m_objs.frontit(); !it.empty(); )
				{
					Obj* pObj = *it;
					GAST(pObj != GNIL);

					if(pObj->bBegin)
					{
						++it;
						continue;
					}

					if(pObj->records.empty() || uCurrentTime - pObj->records.back().uTime > m_uRecycleTime)
					{
						for(GAIA::NUM x = 0; x < pObj->records.size(); ++x)
						{
							Record& r = pObj->records[x];
							if(r.pszInfo != r.szInfo &&
							   r.pszInfo != OBJ_WATCHER_CTOR &&
							   r.pszInfo != OBJ_WATCHER_DTOR)
								m_strpool.release(r.pszInfo);
						}
						pObj->records.clear();
						m_objpool.release(pObj);
						it.erase();
						continue;
					}

					if(pObj->nBeginCount > m_sRecycleBeginCount)
					{
						GAIA::X128 lastuid = pObj->records.front().uid;
						GAIA::NUM sOffset = 0;
						for(GAIA::NUM x = 0; x < pObj->records.size(); ++x)
						{
							const Record& r = pObj->records[x];
							if(r.uid != lastuid)
							{
								lastuid = r.uid;
								sOffset = x;
								pObj->nBeginCount--;
								if(pObj->nBeginCount <= m_sRecycleBeginCount)
									break;
							}
						}
						for(GAIA::NUM x = 0; x < sOffset; ++x)
						{
							Record& r = pObj->records[x];
							if(r.pszInfo != r.szInfo &&
							   r.pszInfo != OBJ_WATCHER_CTOR &&
							   r.pszInfo != OBJ_WATCHER_DTOR)
								m_strpool.release(r.pszInfo);
						}
						pObj->records.keep(pObj->records.size() - sOffset);
					}

					++it;
				}

				return GAIA::True;
			}

		private:
			typedef Obj* ObjPtr;
			typedef GAIA::CTN::HashMap<const GAIA::GVOID*, Obj*> __ObjHashMap;

		private:
			GINL GAIA::LOG::Log& select_log() const
			{
				GAIA::LOG::Log* pLog;
				if(m_pLog == GNIL)
					pLog = &g_gaia_log;
				else
					pLog = m_pLog;
				return *pLog;
			}
			GINL GAIA::GVOID logout_obj(const Obj& obj) const
			{
				GAIA::LOG::Log& l = this->select_log();
				l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
				{
					l << "Watch " << (GAIA::U64)obj.p
					  << ", RecordCnt=" << obj.records.size()
					  << ", BeginCnt=" << obj.nBeginCount << ", IsBegan="
					  << obj.bBegin;
				}
				l << l.End();

				for(GAIA::NUM x = 0; x < obj.records.size(); ++x)
				{
					const Record& r = obj.records[x];
					this->logout_record(r);
				}
			}
			GINL GAIA::GVOID logout_record(const Record& r) const
			{
				// Convert data to string.
				GAIA::CH szUid[33];
				r.uid.tostring(szUid);

				// Log out.
				GAIA::LOG::Log& l = this->select_log();
				l << l.Type(GAIA::LOG::Log::TYPE_LOG) << l.UserFilter(0x0002);
				{
					GAIA::CH szTime[32];
					GAIA::TIME::deltatime2string(r.uTime, szTime);

					l << "Watch " << (GAIA::U64)r.p
					  << ", Uid=" << szUid
					  << ", ObjType=" << r.sObjType
					  << ", Tid=" << r.uThreadID
					  << ", Time=" << szTime
					  << ", Info=" << r.pszInfo;
				}
				l << l.End();
			}

		private:
			GAIA::SYNC::Lock m_lr;
			GAIA::CTN::Pool<Obj> m_objpool;
			GAIA::CTN::StringPtrPool<GAIA::CH> m_strpool;
			__ObjHashMap m_objs;
			GAIA::U64 m_uRecycleTime;
			GAIA::NUM m_sRecycleBeginCount;
			GAIA::LOG::Log* m_pLog;
			GAIA::BL m_bEnableLog;
		};
	}
}

#endif
