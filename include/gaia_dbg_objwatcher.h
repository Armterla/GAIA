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
#include "gaia_time.h"
#include "gaia_log.h"

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
					uTime = 0;
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
				GAIA::U64 uTime;
				const GAIA::CH* pszInfo;
			};

			class Obj : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					p = GNIL;
					records.clear();
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
				GAIA::CTN::Vector<Record> records;
				GAIA::BL bBegin;
			};

		public:
			GINL ObjWatcher()
			{
			}
			GINL ~ObjWatcher()
			{
			}
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

				GAST(!pObj->bBegin);
				if(pObj->bBegin)
					return GAIA::False;

				pObj->bBegin = GAIA::True;

				Record r;
				r.p = p;
				if(uid != GNIL)
					r.uid = *uid;
				else
					r.uid.clear();
				r.sObjType = sObjType;
				r.uTime = GAIA::TIME::tick_time();
				r.pszInfo = OBJ_WATCHER_CTOR;
				pObj->records.push_back(r);

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

				GAST(pObj->bBegin);
				if(!pObj->bBegin)
					return GAIA::False;

				GAST(!pObj->records.empty());
				if(!pObj->records.empty())
				{
					if(uid != GNIL)
					{
						GAST(pObj->records.back().uid == *uid);
						if(pObj->records.back().uid != *uid)
							return GAIA::False;
					}

					if(sObjType != GINVALID)
					{
						GAST(pObj->records.back().sObjType == sObjType);
						if(pObj->records.back().sObjType != sObjType)
							return GAIA::False;
					}
				}
				else
				{
					GASTFALSE;
					return GAIA::False;
				}

				pObj->bBegin = GAIA::False;

				Record r;
				r.p = p;
				if(uid != GNIL)
					r.uid = *uid;
				else
					r.uid.clear();
				r.sObjType = sObjType;
				r.uTime = GAIA::TIME::tick_time();
				r.pszInfo = OBJ_WATCHER_DTOR;
				pObj->records.push_back(r);

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
						GAST(pObj->records.back().uid == *uid);
						if(pObj->records.back().uid != *uid)
							return GAIA::False;
					}

					if(sObjType != GINVALID)
					{
						GAST(pObj->records.back().sObjType == sObjType);
						if(pObj->records.back().sObjType != sObjType)
							return GAIA::False;
					}
				}
				else
				{
					GASTFALSE;
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

				GAST(pObj->bBegin);
				if(!pObj->bBegin)
					return GAIA::False;

				GAST(!pObj->records.empty());
				if(!pObj->records.empty())
				{
					if(uid != GNIL)
					{
						GAST(pObj->records.back().uid == *uid);
						if(pObj->records.back().uid != *uid)
							return GAIA::False;
					}

					if(sObjType != GINVALID)
					{
						GAST(pObj->records.back().sObjType == sObjType);
						if(pObj->records.back().sObjType != sObjType)
							return GAIA::False;
					}
				}
				else
				{
					GASTFALSE;
					return GAIA::False;
				}

				Record r;
				r.p = p;
				if(uid != GNIL)
					r.uid = *uid;
				else
					r.uid.clear();
				r.sObjType = sObjType;
				r.uTime = GAIA::TIME::tick_time();
				r.pszInfo = OBJ_WATCHER_CTOR;
				pObj->records.push_back(r);

				return GAIA::True;
			}
			GINL GAIA::BL Recycle(const GAIA::GVOID* p = GNIL, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID)
			{
				GAIA::SYNC::Autolock al(m_lr);

				if(p == GNIL)
				{

				}
				else
				{

				}

				return GAIA::True;
			}
			GINL GAIA::BL CollectRecords(GAIA::CTN::Vector<Record>& listResult, const GAIA::GVOID* p = GNIL, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID)
			{
				listResult.clear();

				GAIA::SYNC::Autolock al(m_lr);

				if(p == GNIL)
				{

				}
				else
				{

				}

				return GAIA::True;
			}
			GINL GAIA::BL CollectObjs(GAIA::CTN::Vector<Obj*>& listResult, const GAIA::GVOID* p = GNIL, GAIA::X128* uid = GNIL, GAIA::NUM sObjType = GINVALID)
			{
				listResult.clear();

				GAIA::SYNC::Autolock al(m_lr);

				if(p == GNIL)
				{

				}
				else
				{

				}

				return GAIA::True;
			}
		private:
			typedef Obj* ObjPtr;

		private:
			GAIA::CTN::Pool<Obj> m_objpool;
			GAIA::SYNC::Lock m_lr;
			GAIA::CTN::HashMap<const GAIA::GVOID*, Obj*> m_objs;
		};
	}
}

#endif
