#ifndef		__GAIA_DBG_PERF_H__
#define		__GAIA_DBG_PERF_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_stream_stdstream.h"
#include "gaia_thread_base.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_algo_compare.h"
#include "gaia_algo_string.h"
#include "gaia_ctn_ref.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_set.h"
#include "gaia_ctn_staticstringptrpool.h"
#include "gaia_time.h"
#include "gaia_log.h"

extern GAIA::LOG::Log g_gaia_log;
extern GAIA::LOG::InvalidLog g_gaia_invalidlog;

namespace GAIA
{
	namespace DBG
	{
		class PerfCollector : public GAIA::Base
		{
		public:
			class CallBack : public GAIA::Base
			{
			public:
				/*!
					@brief Callback per collected performance item.
				
					@param pszItemName [in] Specify the item name.

					@param uThreadID [in] Specify the threadid of current performance item.
						If uThreadID is GINVALID, means it is the combination of item which specified by parameter pszItemName, it ignore the thread's difference.

					@param uTotalTime [in] Specify the total time of current performance item.

					@param uMinTime [in] Specify the min time of current performance item.

					@param uMaxTime [in] Specify the max time of current performance item.

					@param uTotalCount [in] Specify the count of current performance item.
				
					@return If the moduler user return GAIA::True, the collect procedule will continued, return GAIA::False, the collect procedule will breaked.
				*/
				virtual GAIA::BL OnCollect(const GAIA::CH* pszItemName, GAIA::UM uThreadID, const GAIA::U64& uTotalTime, const GAIA::U64& uMinTime, const GAIA::U64& uMaxTime, const GAIA::U64& uTotalCount) = 0;
			};

		private:
			class CallBackForLog : public CallBack
			{
				friend class PerfCollector;
			public:
				GINL CallBackForLog(GAIA::LOG::Log& gaialog, GAIA::LOG::Log::TYPE logtype, GAIA::U32 uLogFilter)
				{
					m_pGAIALog = &gaialog;
					m_logtype = logtype;
					m_uLogFilter = uLogFilter;
				}
				virtual GAIA::BL OnCollect(const GAIA::CH* pszItemName, GAIA::UM uThreadID, const GAIA::U64& uTotalTime, const GAIA::U64& uMinTime, const GAIA::U64& uMaxTime, const GAIA::U64& uTotalCount)
				{
					if(uThreadID == GINVALID)
					{
						if(m_pGAIALog != GNIL)
						{
							GAIA::LOG::Log& gaialog = *m_pGAIALog;
							gaialog << gaialog.Type(m_logtype) 
									<< gaialog.UserFilter(m_uLogFilter) 
									<< pszItemName 
									<< " Count=" << uTotalCount
									<< " Total=" << uTotalTime 
									<< " Avg=" << uTotalTime / uTotalCount 
									<< " Min=" << uMinTime 
									<< " Max=" << uMaxTime 
									<< gaialog.End();
						}
					}
					return GAIA::True;
				}
			private:
				GAIA::LOG::Log* m_pGAIALog;
				GAIA::LOG::Log::TYPE m_logtype;
				GAIA::U32 m_uLogFilter;
			};

		public:
			GINL PerfCollector()
			{
			}

			GINL ~PerfCollector()
			{
			}

			GINL GAIA::GVOID Reset()
			{
				GAIA::SYNC::Autolock al(m_lr);
				m_nodes.destroy();
				m_nodepool.destroy();
				m_strpool.destroy();
			}

			GINL GAIA::GVOID Begin(const GAIA::CH* pszItemName, const GAIA::N64& nInstanceID = GINVALID)
			{
				GAST(!GAIA::ALGO::gstremp(pszItemName));
				GAIA::UM uThreadID = GAIA::THREAD::threadid();
				GAIA::SYNC::Autolock al(m_lr);
				Node* pNode = this->GetNode(pszItemName, uThreadID, GAIA::True, nInstanceID);
				pNode->uLastStartTime = GAIA::TIME::tick_time();
			}

			GINL GAIA::BL End(const GAIA::CH* pszItemName, const GAIA::N64& nInstanceID = GINVALID)
			{
				GAST(!GAIA::ALGO::gstremp(pszItemName));
				GAIA::UM uThreadID = GAIA::THREAD::threadid();
				GAIA::SYNC::Autolock al(m_lr);
				Node* pNode = this->GetNode(pszItemName, uThreadID, GAIA::False, nInstanceID);
				if(pNode == GNIL)
					return GAIA::False;
				if(pNode->uLastStartTime == GINVALID)
					return GAIA::False;
				GAIA::U64 uTime = GAIA::TIME::tick_time() - pNode->uLastStartTime;
				pNode->uTotalTime += uTime;
				if(uTime < pNode->uMinTime)
					pNode->uMinTime = uTime;
				if(uTime > pNode->uMaxTime)
					pNode->uMaxTime = uTime;
				pNode->uTotalCount++;
				pNode->uLastStartTime = GINVALID;
				if(nInstanceID != GINVALID)
				{
					m_nodes.erase(pNode);
					pNode->nInstanceID = GINVALID;
					GAIA::CTN::Ref<Node>* pFindedRef = m_nodes.find(pNode);
					if(pFindedRef == GNIL)
						m_nodes.insert(pNode);
					else
					{
						Node* pFinded = *pFindedRef;
						GAST(pFinded != GNIL);
						pFinded->uTotalTime += pNode->uTotalTime;
						pFinded->uMinTime = GAIA::ALGO::gmin(pFinded->uMinTime, pNode->uMinTime);
						pFinded->uMaxTime = GAIA::ALGO::gmax(pFinded->uMaxTime, pNode->uMaxTime);
						pFinded->uTotalCount += pNode->uTotalCount;
						m_nodepool.release(pNode);
					}
				}
				return GAIA::True;
			}

			GINL GAIA::BL IsBegin(const GAIA::CH* pszItemName, const GAIA::N64& nInstanceID = GINVALID, GAIA::UM uThreadID = GINVALID) const // uThreadID equal GINVALID means current thread.
			{
				GAST(!GAIA::ALGO::gstremp(pszItemName));
				if(uThreadID == GINVALID)
					uThreadID = GAIA::THREAD::threadid();
				GAIA::SYNC::Autolock al(GCCAST(PerfCollector*)(this)->m_lr);
				Node* pNode = GCCAST(PerfCollector*)(this)->GetNode(pszItemName, uThreadID, GAIA::False, nInstanceID);
				if(pNode == GNIL)
					return GAIA::False;
				if(pNode->uLastStartTime == GINVALID)
					return GAIA::False;
				return GAIA::True;
			}

			GINL GAIA::GVOID Lock()
			{
				m_lr.Enter();
			}

			GINL GAIA::GVOID Unlock()
			{
				m_lr.Leave();
			}

			GINL GAIA::BL Get(const GAIA::CH* pszItemName, GAIA::U64* pTotalTime, GAIA::U64* pMinTime, GAIA::U64* pMaxTime, GAIA::U64* pTotalCount) const
			{
				GAST(!GAIA::ALGO::gstremp(pszItemName));
				GAIA::SYNC::Autolock al(GCCAST(PerfCollector*)(this)->m_lr);
				Node finder;
				finder.pszItemName = pszItemName;
				finder.uThreadID = 0;
				finder.nInstanceID = 0;
				__NodeSetType::it itfront = GCCAST(PerfCollector*)(this)->m_nodes.upper_equal(GAIA::CTN::Ref<Node>(&finder));
				if(itfront.empty())
					return GAIA::False;
				Node finded;
				finded.reset();
				while(!itfront.empty())
				{
					Node* pNode = *itfront;
					GAST(pNode != GNIL);
					if(!GAIA::ALGO::gstrequal(pNode->pszItemName, pszItemName))
						break;
					finded.pszItemName = pNode->pszItemName;
					finded.uThreadID = pNode->uThreadID;
					finded.uTotalTime += pNode->uTotalTime;
					finded.uMinTime = GAIA::ALGO::gmin(finded.uMinTime, pNode->uMinTime);
					finded.uMaxTime = GAIA::ALGO::gmax(finded.uMaxTime, pNode->uMaxTime);
					finded.uTotalCount += pNode->uTotalCount;
					++itfront;
				}
				if(pTotalTime != GNIL)
					*pTotalTime = finded.uTotalTime;
				if(pMinTime != GNIL)
					*pMinTime = finded.uMinTime;
				if(pMaxTime != GNIL)
					*pMaxTime = finded.uMaxTime;
				if(pTotalCount != GNIL)
					*pTotalCount = finded.uTotalCount;
				return GAIA::True;
			}

			GINL GAIA::BL GetOnThread(const GAIA::CH* pszItemName, GAIA::U64* pTotalTime, GAIA::U64* pMinTime, GAIA::U64* pMaxTime, GAIA::U64* pTotalCount, GAIA::UM uThreadID = GINVALID) const // ThreadID equal GINVALID means current thread.
			{
				GAST(!GAIA::ALGO::gstremp(pszItemName));
				if(uThreadID == GINVALID)
					uThreadID = GAIA::THREAD::threadid();
				GAIA::SYNC::Autolock al(GCCAST(PerfCollector*)(this)->m_lr);
				Node* pNode = GCCAST(PerfCollector*)(this)->GetNode(pszItemName, uThreadID, GAIA::False);
				if(pNode == GNIL)
					return GAIA::False;
				if(pTotalTime != GNIL)
					*pTotalTime = pNode->uTotalTime;
				if(pMinTime != GNIL)
					*pMinTime = pNode->uMinTime;
				if(pMaxTime != GNIL)
					*pMaxTime = pNode->uMaxTime;
				if(pTotalCount != GNIL)
					*pTotalCount = pNode->uTotalCount;
				return GAIA::True;
			}

			GINL GAIA::GVOID Collect(GAIA::CTN::Vector<const GAIA::CH*>& listResult) const
			{
				GAIA::SYNC::Autolock al(GCCAST(PerfCollector*)(this)->m_lr);
				for(__NodeSetType::const_it it = m_nodes.const_frontit(); !it.empty(); ++it)
				{
					const Node* pNode = *it;
					GAST(pNode != GNIL);
					listResult.push_back(pNode->pszItemName);
				}
			}

			GINL GAIA::BL Collect(GAIA::DBG::PerfCollector::CallBack& cb) const
			{
				GAIA::SYNC::Autolock al(GCCAST(PerfCollector*)(this)->m_lr);
				GAIA::U64 uTotalTime = 0;
				GAIA::U64 uMinTime = GAIA::U64MAX;
				GAIA::U64 uMaxTime = 0;
				GAIA::U64 uTotalCount = 0;
				const GAIA::CH* pszLastItemName = GNIL;
				GAIA::UM uLastThreadID = GINVALID;
				for(__NodeSetType::const_it it = m_nodes.const_frontit(); !it.empty(); ++it)
				{
					const Node* pNode = *it;
					GAST(pNode != GNIL);
					if(pszLastItemName == GNIL)
					{
						pszLastItemName = pNode->pszItemName;
						uLastThreadID = pNode->uThreadID;
						uTotalTime = 0;
						uMinTime = GAIA::U64MAX;
						uMaxTime = 0;
						uTotalCount = 0;
					}
					else if(!GAIA::ALGO::gstrequal(pszLastItemName, pNode->pszItemName))
					{
						if(!cb.OnCollect(pszLastItemName, GINVALID, uTotalTime, uMinTime, uMaxTime, uTotalCount))
							return GAIA::False;
						pszLastItemName = pNode->pszItemName;
						uLastThreadID = pNode->uThreadID;
						uTotalTime = 0;
						uMinTime = GAIA::U64MAX;
						uMaxTime = 0;
						uTotalCount = 0;
					}
					if(!cb.OnCollect(pNode->pszItemName, pNode->uThreadID, pNode->uTotalTime, pNode->uMinTime, pNode->uMaxTime, pNode->uTotalCount))
						return GAIA::False;
					uTotalTime += pNode->uTotalTime;
					uMinTime = GAIA::ALGO::gmin(uMinTime, pNode->uMinTime);
					uMaxTime = GAIA::ALGO::gmax(uMaxTime, pNode->uMaxTime);
					uTotalCount += pNode->uTotalCount;
				}
				if(pszLastItemName != GNIL)
				{
					if(!cb.OnCollect(pszLastItemName, GINVALID, uTotalTime, uMinTime, uMaxTime, uTotalCount))
						return GAIA::False;
				}
				return GAIA::True;
			}

			GINL GAIA::BL DumpToLog(GAIA::LOG::Log* pLog = GNIL, GAIA::LOG::Log::TYPE logtype = GAIA::LOG::Log::TYPE_LOG, GAIA::U32 uLogFilter = 0x0003)
			{
				if(pLog == GNIL)
				{
				#ifdef GAIA_DEBUG_LOG
					pLog = &g_gaia_log;
				#endif
				}
				CallBackForLog cb(*pLog, logtype, uLogFilter);
				this->Collect(cb);
				return GAIA::True;
			}

		private:
			class Node : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					pszItemName = GNIL;
					uThreadID = GINVALID;
					nInstanceID = GINVALID;
					uTotalTime = 0;
					uMinTime = GAIA::U64MAX;
					uMaxTime = 0;
					uTotalCount = 0;
					uLastStartTime = GINVALID;
				}
				GINL GAIA::N32 compare(const Node& src) const
				{
					GAIA::N32 nCmp = GAIA::ALGO::gstrcmp(pszItemName, src.pszItemName);
					if(nCmp != 0)
						return nCmp;
					if(nInstanceID != GINVALID && src.nInstanceID == GINVALID)
						return -1;
					else if(nInstanceID == GINVALID && src.nInstanceID != GINVALID)
						return +1;
					else if(nInstanceID != GINVALID && src.nInstanceID != GINVALID)
					{
						if(nInstanceID < src.nInstanceID)
							return -1;
						else if(nInstanceID > src.nInstanceID)
							return +1;
					}
					else
					{
						if(uThreadID < src.uThreadID)
							return -1;
						else if(uThreadID > src.uThreadID)
							return +1;	
					}
					
					return 0;
				}
				GCLASS_COMPARE_BYCOMPARE(Node)
			public:
				const GAIA::CH* pszItemName;
				GAIA::UM uThreadID;
				GAIA::N64 nInstanceID;
				GAIA::U64 uTotalTime;
				GAIA::U64 uMinTime;
				GAIA::U64 uMaxTime;
				GAIA::U64 uTotalCount;
				GAIA::U64 uLastStartTime; // GINVALID means not startupped.
			};

		private:
			typedef GAIA::CTN::Set<GAIA::CTN::Ref<Node> > __NodeSetType;

		private:
			GINL Node* GetNode(const GAIA::CH* pszItemName, GAIA::UM uThreadID, GAIA::BL bNotExistCreate, const GAIA::N64& nInstanceID = GINVALID)
			{
				Node finder;
				finder.pszItemName = pszItemName;
				finder.uThreadID = uThreadID;
				finder.nInstanceID = nInstanceID;
				GAIA::CTN::Ref<Node> reffinder = &finder;
				GAIA::CTN::Ref<Node>* pFindedRef = m_nodes.find(reffinder);
				if(pFindedRef != GNIL)
					return *pFindedRef;
				else if(bNotExistCreate)
				{
					Node* pNewNode = m_nodepool.alloc();
					pNewNode->reset();
					pNewNode->pszItemName = m_strpool.alloc(pszItemName);
					pNewNode->uThreadID = uThreadID;
					pNewNode->nInstanceID = nInstanceID;
					__NodeSetType::it it;
					m_nodes.insert(GAIA::CTN::Ref<Node>(pNewNode), &it);
					return *it;
				}
				else
					return GNIL;
			}

		private:
			GAIA::SYNC::Lock m_lr;
			__NodeSetType m_nodes;
			GAIA::CTN::Pool<Node> m_nodepool;
			GAIA::CTN::StaticStringPtrPool<GAIA::CH> m_strpool;
		};

		class PerfCollectorAuto : public GAIA::Base
		{
		public:
			GINL PerfCollectorAuto(GAIA::DBG::PerfCollector& perf, const GAIA::CH* pszItemName, const GAIA::N64& nInstanceID)
			{
				m_pPerfCollector = &perf;
				m_pszItemName = pszItemName;
				m_nInstanceID = nInstanceID;
				m_pPerfCollector->Begin(m_pszItemName, m_nInstanceID);
			}
			GINL ~PerfCollectorAuto()
			{
				m_pPerfCollector->End(m_pszItemName, m_nInstanceID);
			}
		private:
			GAIA::DBG::PerfCollector* m_pPerfCollector;
			const GAIA::CH* m_pszItemName;
			GAIA::N64 m_nInstanceID;
		};
	}
}

#endif
