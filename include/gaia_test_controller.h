#ifndef		__GAIA_TEST_CONTROLLER_H__
#define		__GAIA_TEST_CONTROLLER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_base.h"
#include "gaia_thread.h"
#include "gaia_math_random_lcg.h"
#include "gaia_ctn_string.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_queue.h"
#include "gaia_ctn_book.h"
#include "gaia_ctn_set.h"

namespace GAIA
{
	namespace TEST
	{
		static const GAIA::CH* CONTROLLER_STAGE_NAME[] =
		{
			"CONTROLLER_STATE_INVALID",
			"CONTROLLER_STAGE_BEGIN",
			"CONTROLLER_STAGE_END",
			"CONTROLLER_STAGE_CREATEUNIT",
			"CONTROLLER_STAGE_RELEASEUNIT",
			"CONTROLLER_STAGE_UNIT_BEGIN",
			"CONTROLLER_STAGE_UNIT_END",
			"CONTROLLER_STAGE_UNIT_EXECUTE_FRONT",
			"CONTROLLER_STAGE_UNIT_EXECUTE_BACK",
		};
		
		GAIA_ENUM_BEGIN(CONTROLLER_STAGE)
			CONTROLLER_STAGE_BEGIN,
			CONTROLLER_STAGE_END,
			CONTROLLER_STAGE_CREATEUNIT,
			CONTROLLER_STAGE_RELEASEUNIT,
			CONTROLLER_STAGE_UNIT_BEGIN,
			CONTROLLER_STAGE_UNIT_END,
			CONTROLLER_STAGE_UNIT_EXECUTEFRONT,
			CONTROLLER_STAGE_UNIT_EXECUTEBACK,
		GAIA_ENUM_END(CONTROLLER_STAGE)
		
		class Controller : public GAIA::Base
		{
		public:
			class CallBack : public GAIA::Base
			{
			public:
				virtual GAIA::GVOID OnUserError(GAIA::TEST::Controller& ctl, const GAIA::CH* pszFileName, GAIA::N32 nCodeLine, GAIA::NUM sTimes){}
				virtual GAIA::GVOID OnCaseError(GAIA::TEST::Controller& ctl, GAIA::TEST::CONTROLLER_STAGE stage, GAIA::TEST::Case& cse){}
				virtual GAIA::GVOID OnUnitError(GAIA::TEST::Controller& ctl, GAIA::TEST::CONTROLLER_STAGE stage, GAIA::TEST::Case& cse, GAIA::TEST::Unit& unit){}
				virtual GAIA::GVOID OnStage(GAIA::TEST::Controller& ctl, GAIA::TEST::CONTROLLER_STAGE stage){}
				virtual GAIA::GVOID OnStageCase(GAIA::TEST::Controller& ctl, GAIA::TEST::CONTROLLER_STAGE stage, GAIA::TEST::Case& cse){}
				virtual GAIA::GVOID OnStageUnit(GAIA::TEST::Controller& ctl, GAIA::TEST::CONTROLLER_STAGE stage, GAIA::TEST::Case& cse, GAIA::TEST::Unit& unit){}
				virtual GAIA::GVOID OnProgress(GAIA::TEST::Controller& ctl, const GAIA::N64& nCurrent, const GAIA::N64& nTotal){}
				virtual GAIA::GVOID OnProgressCase(GAIA::TEST::Controller& ctl, const GAIA::N64& nCurrent, const GAIA::N64& nTotal, GAIA::TEST::Case& cse){}
				virtual GAIA::GVOID OnProgressUnit(GAIA::TEST::Controller& ctl, const GAIA::N64& nCurrent, const GAIA::N64& nTotal, GAIA::TEST::Case& cse, GAIA::TEST::Unit& unit){}
			};
			class Desc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					pCallBack = GNIL;
					uRandomSeed = 0;
					listExecuteWorkset.clear();
					listIgnoreWorkset.clear();
					sUnitErrStopCount = -1;
					sUserErrStopCount = -1;
					sMaxUnitCount = -1;
					sMaxExecuteFrontCount = -1;
					sMaxExecuteBackCount = -1;
					uMinExecuteSleepTime = 0;
					uMaxExecuteSleepTime = 0;
					sMaxDepth = -1;
					sThreadCount = 2;
					bMultiThread = GAIA::True;
				}
				GINL GAIA::BL check() const
				{
					if(!listExecuteWorkset.empty() && !listIgnoreWorkset.empty())
						return GAIA::False;
					if(sUnitErrStopCount != -1 && sUnitErrStopCount <= 0)
						return GAIA::False;
					if(sUserErrStopCount != -1 && sUserErrStopCount <= 0)
						return GAIA::False;
					if(sMaxUnitCount != -1 && sMaxUnitCount <= 0)
						return GAIA::False;
					if(sMaxExecuteFrontCount != -1 && sMaxExecuteFrontCount <= 0)
						return GAIA::False;
					if(sMaxExecuteBackCount != -1 && sMaxExecuteBackCount <= 0)
						return GAIA::False;
					if(uMinExecuteSleepTime > uMaxExecuteSleepTime)
						return GAIA::False;
					if(sMaxDepth != -1 && sMaxDepth < 0)
						return GAIA::False;
					if(sThreadCount < 0)
						return GAIA::False;
					if(bMultiThread && sThreadCount == 0)
						return GAIA::False;
					if(!bMultiThread && sThreadCount > 0)
						return GAIA::False;
					return GAIA::True;
				}
			public:
				CallBack* pCallBack;
				GAIA::U32 uRandomSeed;
				GAIA::TEST::__WorksetType listExecuteWorkset;
				GAIA::TEST::__WorksetType listIgnoreWorkset;
				GAIA::NUM sUnitErrStopCount;
				GAIA::NUM sUserErrStopCount;
				GAIA::NUM sMaxUnitCount;
				GAIA::NUM sMaxExecuteFrontCount;
				GAIA::NUM sMaxExecuteBackCount;
				GAIA::U64 uMinExecuteSleepTime;
				GAIA::U64 uMaxExecuteSleepTime;
				GAIA::NUM sMaxDepth; // If sMaxDepth == 0, means only root cases will been executed.
				GAIA::NUM sThreadCount;
				GAIA::U8 bMultiThread : 1;
			};
		public:
			GINL Controller(){this->init();}
			GINL ~Controller(){this->destruct();}

			GINL GAIA::BL SetDesc(const GAIA::TEST::Controller::Desc& desc)
			{
				GAST(desc.check());
				if(!desc.check())
					return GAIA::False;
				m_desc = desc;
				m_desc.listExecuteWorkset.sort();
				m_desc.listExecuteWorkset.unique();
				m_desc.listIgnoreWorkset.sort();
				m_desc.listIgnoreWorkset.unique();
				return GAIA::True;
			}
			GINL const GAIA::TEST::Controller::Desc& GetDesc() const{return m_desc;}

			GINL GAIA::NUM Insert(GAIA::TEST::Case& cse)
			{
				if(this->IsBegin())
					return GINVALID;
				__CaseListType::_sizetype usedindex = m_cases.insert(&cse);
				__CaseListType::_sizetype fixedindex = m_cases.fixedindex(usedindex);
				cse.SetControllerIndex(fixedindex);
				cse.SetController(this);
				cse.rise_ref();
				return fixedindex;
			}
			GINL GAIA::BL Erase(GAIA::TEST::Case& cse)
			{
				if(this->IsBegin())
					return GAIA::False;
				if(cse.GetController() == GNIL || cse.GetController() != this)
					return GAIA::False;
				GAIA::NUM fixedindex = cse.GetControllerIndex();
				__CaseListType::_sizetype usedindex = m_cases.usedindex(fixedindex);
				if(usedindex >= m_cases.size())
					return GAIA::False;
				if(!m_cases.erase(usedindex))
					return GAIA::False;
				cse.drop_ref();
				return GAIA::True;
			}
			GINL GAIA::BL EraseAll()
			{
				if(this->IsBegin())
					return GAIA::False;
				for(__CaseListType::_sizetype x = 0; x < m_cases.size(); ++x)
				{
					GAIA::TEST::Case* pCase = m_cases[x];
					GAST(pCase != GNIL);
					pCase->drop_ref();
				}
				m_cases.destroy();
				return GAIA::True;
			}
			GINL GAIA::NUM GetCaseCount() const{return m_cases.size();}
			GINL GAIA::TEST::Case* GetCase(GAIA::NUM sIndex) const
			{
				GPCHR_ABOVEEQUALZERO_RET(sIndex - this->GetCaseCount(), GNIL);
				return m_cases[sIndex];
			}
			GINL GAIA::BL Begin()
			{
				if(this->IsBegin())
					return GAIA::False;
				if(m_desc.pCallBack != GNIL)
					m_desc.pCallBack->OnStage(*this, CONTROLLER_STAGE_BEGIN);
				m_rand.seed(m_desc.uRandomSeed);
				this->CalculateMaxDepth();
				this->GenerateCaseCtx();
				this->BeginUnitThread();
				this->ExecuteCaseCtx(m_cctxs, GNIL);
				m_bBegin = GAIA::True;
				return GAIA::True;
			}
			GINL GAIA::BL End()
			{
				if(!this->IsBegin())
					return GAIA::False;
				this->WaitThreadComplete();
				this->DestroyThread();
				this->DestroyCaseCtx(m_gctx, m_cctxs);
				if(m_desc.pCallBack != GNIL)
					m_desc.pCallBack->OnStage(*this, CONTROLLER_STAGE_END);
				m_bBegin = GAIA::False;
				return GAIA::True;
			}
			GINL GAIA::BL IsBegin() const
			{
				return m_bBegin;
			}
			GINL GAIA::BL Error(const GAIA::CH* pszFileName, GAIA::N32 nCodeLine)
			{
				m_gctx.aUserErrCount.Increase();
				GAIA::SYNC::Autolock al(m_lrErr);
				ErrorNode finder;
				finder.strFileName.proxy(pszFileName);
				finder.nCodeLine = nCodeLine;
				{
					ErrorNode* pFinded = m_errs.find(finder);
					if(pFinded == GNIL)
					{
						finder.sCount = 1;
						m_errs.insert(finder);
						finder.strFileName.proxy(GNIL);
						if(m_desc.pCallBack != GNIL)
							m_desc.pCallBack->OnUserError(*this, pszFileName, nCodeLine, 1);
						return GAIA::True;
					}
					else
					{
						++pFinded->sCount;
						if(m_desc.pCallBack != GNIL)
							m_desc.pCallBack->OnUserError(*this, pszFileName, nCodeLine, pFinded->sCount);
					}
				}
				finder.strFileName.proxy(GNIL);
				return GAIA::True;
			}

		private:
			class CasePrio : public GAIA::Base
			{
			public:
				GCLASS_COMPARE(pCase->GetDesc().sPriority, CasePrio)
			public:
				GAIA::TEST::Case* pCase;
			};

			class GlobalCtx : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					aUnitErrCount = 0;
					aUserErrCount = 0;
					sMaxDepth = 0;
					nTotalUnitCount = 0;
					aCompleteUnitCount = 0;
				}
			public:
				GAIA::SYNC::Lock lr;
				GAIA::SYNC::Atomic aUnitErrCount;
				GAIA::SYNC::Atomic aUserErrCount;
				GAIA::NUM sMaxDepth;
				GAIA::N64 nTotalUnitCount;
				GAIA::SYNC::Atomic aCompleteUnitCount;
			};

			class CaseCtx;
			typedef GAIA::CTN::Vector<CaseCtx*> __CaseCtxListType;
			class CaseCtx : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					pCase = GNIL;
					sUnitCount = 0;
					parent = GNIL;
				}
				GCLASS_COMPARE(pCase, CaseCtx)
			public:
				GAIA::SYNC::Lock lr;
				GAIA::TEST::Case* pCase;
				GAIA::NUM sUnitCount;
				GAIA::CTN::Vector<GAIA::NUM> listExecuteFrontCount;
				GAIA::CTN::Vector<GAIA::NUM> listExecuteBackCount;
				GAIA::CTN::Vector<GAIA::NUM> listThreadIndex;
				__CaseCtxListType childs;
				CaseCtx* parent;
			};

			class RecursiveCtx : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					sCurrentDepth = 0;
				}
			public:
				GAIA::NUM sCurrentDepth;
			};

			class UnitTask : public GAIA::Base
			{
			public:
				GAIA::TEST::Unit* pUnit;
				CaseCtx* pCaseCtx;
				GAIA::NUM sUnitIndex;
			};
			typedef GAIA::CTN::Queue<UnitTask> __UnitTaskQueueType;

			class ThreadCtx : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					sThreadIndex = -1;
					pController = GNIL;
					pGlobalCtx = GNIL;
				}
			public:
				GAIA::SYNC::Lock lr;
				GAIA::NUM sThreadIndex;
				GAIA::TEST::Controller* pController;
				GlobalCtx* pGlobalCtx;
				__UnitTaskQueueType utasks;
				__UnitTaskQueueType utasks_swap;
			};

			class WorkThread : public GAIA::THREAD::Thread
			{
			public:
				GINL WorkThread(){this->init();}
				GINL ThreadCtx& GetCtx(){return m_ctx;}
				GINL GAIA::GVOID StopCmd(){m_bStopCmd = GAIA::True;}
				virtual GAIA::GVOID Run()
				{
					for(;;)
					{
						if(m_bStopCmd)
						{
							m_bStopCmd = GAIA::False;
							break;
						}
						if(!m_ctx.pController->ExecuteThread(m_ctx))
							GAIA::SYNC::gsleep(1);
					}
				}
			private:
				GINL GAIA::GVOID init()
				{
					this->SetName("GAIA::TEST::Controller::WorkThread");
					m_ctx.reset();
					m_bStopCmd = GAIA::False;
				}
			private:
				ThreadCtx m_ctx;
				GAIA::BL m_bStopCmd;
			};
			
			class ErrorNode : public GAIA::Base
			{
			public:
				GCLASS_COMPARE2(strFileName, nCodeLine, ErrorNode)
			public:
				GAIA::CTN::AString strFileName;
				GAIA::N32 nCodeLine;
				GAIA::NUM sCount;
			};

			typedef GAIA::CTN::Vector<GAIA::TEST::Unit*> __UnitListType;
			typedef GAIA::CTN::Book<GAIA::TEST::Case*> __CaseListType;
			typedef GAIA::CTN::Vector<CasePrio> __CasePrioListType;
			typedef GAIA::CTN::Vector<WorkThread*> __ThreadListType;

		private:
			GINL GAIA::GVOID init()
			{
				m_desc.reset();
				m_bBegin = GAIA::False;
				m_gctx.reset();
			}

			GINL GAIA::GVOID destruct()
			{
				if(this->IsBegin())
					this->End();
				this->EraseAll();
			}

			GINL GAIA::GVOID CalculateMaxDepth()
			{
				for(__CaseListType::_sizetype x = 0; x < m_cases.size(); ++x)
				{
					GAIA::TEST::Case* pCase = m_cases[x];
					GAST(pCase != GNIL);
					this->CalculateMaxDepth(m_gctx, *pCase, 0);
				}
			}
			GINL GAIA::GVOID CalculateMaxDepth(GlobalCtx& gctx, GAIA::TEST::Case& cse, GAIA::NUM sDepth)
			{
				if(gctx.sMaxDepth < sDepth + 1)
					gctx.sMaxDepth = sDepth + 1;
				for(GAIA::NUM x = 0; x < cse.GetChildCount(); ++x)
				{
					GAIA::TEST::Case* pChild = cse.GetChild(x);
					GAST(pChild != GNIL);
					this->CalculateMaxDepth(gctx, *pChild, sDepth + 1);
					pChild->drop_ref();
				}
			}
			GINL GAIA::GVOID GenerateCaseCtx()
			{
				GAST(m_cctxs.empty());

				// 
				RecursiveCtx rctx;
				rctx.reset();

				// Generate case priority list.
				__CasePrioListType listCasePriority;
				this->GenerateCasePriorityList(m_cases, listCasePriority);
				GAST(m_cases.size() == listCasePriority.size());
				if(listCasePriority.empty())
					return;

				//
				this->GenerateCaseCtxRecursive(rctx, GNIL, m_cctxs, listCasePriority);
			}

			GINL GAIA::GVOID GenerateCaseCtxRecursive(RecursiveCtx& rctx, CaseCtx* pParentCtx, __CaseCtxListType& cctxs, __CasePrioListType& prios)
			{
				// Fill case context list, and recursive.
				for(__CaseCtxListType::_sizetype x = 0; x < prios.size(); ++x)
				{
					//
					GAIA::TEST::Case* pCase = prios[x].pCase;
					GAST(pCase != GNIL);
					if(!m_desc.listIgnoreWorkset.empty())
					{
						if(this->MatchWorkset(pCase->GetDesc().listWorkset, m_desc.listIgnoreWorkset) != 0)
							continue;
					}
					if(!m_desc.listExecuteWorkset.empty())
					{
						if(this->MatchWorkset(pCase->GetDesc().listWorkset, m_desc.listExecuteWorkset) == 0)
							continue;
					}

					//
					CaseCtx* pCaseCtx = gnew CaseCtx;
					pCaseCtx->reset();
					pCaseCtx->parent = pParentCtx;
					pCaseCtx->pCase = pCase;
					cctxs.push_back(pCaseCtx);

					// Calculate unit count.
					const GAIA::TEST::Case::Desc& descCase = pCaseCtx->pCase->GetDesc();
					pCaseCtx->sUnitCount = this->CalculateCaseUnitCount(descCase);
					m_gctx.nTotalUnitCount += this->CalculatePracticeUnitCount(*pCaseCtx);

					// Calculate total execute count and thread index.
					for(GAIA::NUM x = 0; x < pCaseCtx->sUnitCount; ++x)
					{
						// Execute count.
						GAIA::NUM sExecuteFrontCount = this->CalculateUnitExecuteCount(
							descCase.sMinExecuteFrontCount,
							descCase.sMaxExecuteFrontCount,
							m_desc.sMaxExecuteFrontCount);
						GAIA::NUM sExecuteBackCount = this->CalculateUnitExecuteCount(
							descCase.sMinExecuteBackCount,
							descCase.sMaxExecuteBackCount,
							m_desc.sMaxExecuteBackCount);
						pCaseCtx->listExecuteFrontCount.push_back(sExecuteFrontCount);
						pCaseCtx->listExecuteBackCount.push_back(sExecuteBackCount);

						// Thread index.
						GAIA::NUM sThreadIndex = this->SelectThread(rctx.sCurrentDepth);
						pCaseCtx->listThreadIndex.push_back(sThreadIndex);
					}

					//
					if(m_desc.sMaxDepth == -1 || rctx.sCurrentDepth < m_desc.sMaxDepth)
					{
						//
						rctx.sCurrentDepth++;

						// Generate child case priority list.
						__CasePrioListType child_prios;
						this->GenerateCasePriorityList(*pCaseCtx->pCase, child_prios);
						GAST(pCaseCtx->pCase->GetChildCount() == child_prios.size());
						if(!child_prios.empty())
							this->GenerateCaseCtxRecursive(rctx, pCaseCtx, pCaseCtx->childs, child_prios);

						//
						rctx.sCurrentDepth--;
					}
				}
			}

			GINL GAIA::BL ExecuteCaseCtx(__CaseCtxListType& cctxs, GAIA::TEST::Unit* pParent)
			{
				if(cctxs.size() == 0)
					return GAIA::True;

				//
				GAIA::SYNC::Event e;
				__UnitListType units;

				//
				GAIA::NUM sOldPriority;
				{
					CaseCtx* pCaseCtx = cctxs[0];
					GAST(pCaseCtx != GNIL);
					GAIA::TEST::Case* pCase = pCaseCtx->pCase;
					GAST(pCase != GNIL);
					const GAIA::TEST::Case::Desc& desc = pCase->GetDesc();
					sOldPriority = desc.sPriority;
				}

				//
				for(__CaseCtxListType::_sizetype x = 0; x < cctxs.size(); ++x)
				{
					//
					CaseCtx* pCaseCtx = cctxs[x];
					GAST(pCaseCtx != GNIL);
					GAIA::TEST::Case* pCase = pCaseCtx->pCase;
					GAST(pCase != GNIL);
					const GAIA::TEST::Case::Desc& descCase = pCase->GetDesc();

					//
					if(descCase.sPriority == sOldPriority)
					{
						//
						for(GAIA::NUM y = 0; y < pCaseCtx->sUnitCount; ++y)
						{
							//
							GAIA::TEST::Unit* pUnit = pCase->CreateUnit();
							if(pUnit == GNIL)
							{
								if(m_desc.pCallBack != GNIL)
									m_desc.pCallBack->OnCaseError(*this, CONTROLLER_STAGE_CREATEUNIT, *pCase);
								continue;
							}
							pUnit->SetCase(pCase);
							pUnit->SetParent(pParent);

							//
							if(!m_desc.bMultiThread || m_desc.sThreadCount == 0 || !descCase.bMultiThread)
							{
								if(!this->ExecuteUnit(*pUnit, *pCaseCtx, y))
								{
									if(!pCase->ReleaseUnit(*pUnit))
									{
										if(m_desc.pCallBack != GNIL)
											m_desc.pCallBack->OnCaseError(*this, CONTROLLER_STAGE_RELEASEUNIT, *pCase);
									}
									return GAIA::False;
								}
								if(!pCase->ReleaseUnit(*pUnit))
								{
									if(m_desc.pCallBack != GNIL)
										m_desc.pCallBack->OnCaseError(*this, CONTROLLER_STAGE_RELEASEUNIT, *pCase);
								}
							}
							else
							{
								pUnit->SetBindEvent(&e);
								units.push_back(pUnit);
								this->SendUnitToThread(*pUnit, *pCaseCtx, y);
							}
						}
					}
					else
					{
						this->WaitExecuteComplete(e, units);
						sOldPriority = descCase.sPriority;
						--x;
					}
				}
				this->WaitExecuteComplete(e, units);

				return GAIA::True;
			}

			GINL GAIA::BL ExecuteUnit(GAIA::TEST::Unit& unit, CaseCtx& cctx, GAIA::NUM sUnitIndex)
			{
				// Calculate total unit progress.
				GAIA::N64 nUnitTotalProgress = 2 + cctx.listExecuteFrontCount[sUnitIndex] + cctx.listExecuteBackCount[sUnitIndex];
				GAIA::N64 nUnitCurrentProgress = 0;
				
				// Begin.
				if(m_desc.pCallBack != GNIL)
					m_desc.pCallBack->OnStageUnit(*this, CONTROLLER_STAGE_UNIT_BEGIN, *unit.GetCase(), unit);
				GAIA::BL bBegin = GAIA::True;
				if(!unit.Begin())
				{
					if(m_desc.pCallBack != GNIL)
						m_desc.pCallBack->OnUnitError(*this, CONTROLLER_STAGE_UNIT_BEGIN, *unit.GetCase(), unit);
					
					GAIA::N64 nResult = m_gctx.aUnitErrCount.Increase();
					if(m_desc.sUnitErrStopCount != -1 && nResult >= m_desc.sUnitErrStopCount)
					{
						this->FireUnitEvent(unit);
						return GAIA::False;
					}
					bBegin = GAIA::False;
				}
				if(m_desc.pCallBack != GNIL)
					m_desc.pCallBack->OnProgressUnit(*this, ++nUnitCurrentProgress, nUnitTotalProgress, *unit.GetCase(), unit);

				// Execute front.
				if(bBegin)
				{
					if(m_desc.pCallBack != GNIL)
						m_desc.pCallBack->OnStageUnit(*this, CONTROLLER_STAGE_UNIT_EXECUTEFRONT, *unit.GetCase(), unit);
					GAIA::NUM sExecuteFrontCount = cctx.listExecuteFrontCount[sUnitIndex];
					for(GAIA::NUM x = 0; x < sExecuteFrontCount; ++x)
					{
						if(!unit.ExecuteFront())
						{
							if(m_desc.pCallBack != GNIL)
								m_desc.pCallBack->OnUnitError(*this, CONTROLLER_STAGE_UNIT_EXECUTEFRONT, *unit.GetCase(), unit);
							
							GAIA::N64 nResult = m_gctx.aUnitErrCount.Increase();
							if(m_desc.sUnitErrStopCount != -1 && nResult >= m_desc.sUnitErrStopCount)
							{
								this->FireUnitEvent(unit);
								return GAIA::False;
							}
						}
						if(m_desc.pCallBack != GNIL)
							m_desc.pCallBack->OnProgressUnit(*this, ++nUnitCurrentProgress, nUnitTotalProgress, *unit.GetCase(), unit);
					}
				}

				// Do child.
				if(!cctx.childs.empty())
				{
					if(bBegin && !this->ExecuteCaseCtx(cctx.childs, &unit))
					{
						this->FireUnitEvent(unit);
						return GAIA::False;
					}
				}

				// Execute back.
				if(bBegin)
				{
					if(m_desc.pCallBack != GNIL)
						m_desc.pCallBack->OnStageUnit(*this, CONTROLLER_STAGE_UNIT_EXECUTEBACK, *unit.GetCase(), unit);
					GAIA::NUM sExecuteBackCount = cctx.listExecuteBackCount[sUnitIndex];
					for(GAIA::NUM x = 0; x < sExecuteBackCount; ++x)
					{
						if(!unit.ExecuteBack())
						{
							if(m_desc.pCallBack != GNIL)
								m_desc.pCallBack->OnUnitError(*this, CONTROLLER_STAGE_UNIT_EXECUTEBACK, *unit.GetCase(), unit);
							
							GAIA::N64 nResult = m_gctx.aUnitErrCount.Increase();
							if(m_desc.sUnitErrStopCount != -1 && nResult >= m_desc.sUnitErrStopCount)
							{
								this->FireUnitEvent(unit);
								return GAIA::False;
							}
						}
						if(m_desc.pCallBack != GNIL)
							m_desc.pCallBack->OnProgressUnit(*this, ++nUnitCurrentProgress, nUnitTotalProgress, *unit.GetCase(), unit);
					}
				}

				// End.
				if(bBegin)
				{
					if(m_desc.pCallBack != GNIL)
						m_desc.pCallBack->OnStageUnit(*this, CONTROLLER_STAGE_UNIT_BEGIN, *unit.GetCase(), unit);
					if(!unit.End())
					{
						if(m_desc.pCallBack != GNIL)
							m_desc.pCallBack->OnUnitError(*this, CONTROLLER_STAGE_UNIT_END, *unit.GetCase(), unit);
						GAIA::N64 nResult = m_gctx.aUnitErrCount.Increase();
						if(m_desc.sUnitErrStopCount != -1 && nResult >= m_desc.sUnitErrStopCount)
						{
							this->FireUnitEvent(unit);
							return GAIA::False;
						}
					}
					if(m_desc.pCallBack != GNIL)
						m_desc.pCallBack->OnProgressUnit(*this, ++nUnitCurrentProgress, nUnitTotalProgress, *unit.GetCase(), unit);
				}

				this->FireUnitEvent(unit);
				m_gctx.aCompleteUnitCount.Increase();
				if(m_desc.pCallBack != GNIL)
					m_desc.pCallBack->OnProgress(*this, (GAIA::N64)m_gctx.aCompleteUnitCount, m_gctx.nTotalUnitCount);
				
				if(m_desc.sUserErrStopCount != -1 && m_gctx.aUserErrCount >= m_desc.sUnitErrStopCount)
					return GAIA::False;
				return GAIA::True;
			}

			GINL GAIA::GVOID FireUnitEvent(GAIA::TEST::Unit& unit)
			{
				GAIA::SYNC::Event* pEvent = unit.GetBindEvent();
				if(pEvent != GNIL)
					pEvent->Fire();
			}

			GINL GAIA::GVOID SendUnitToThread(GAIA::TEST::Unit& unit, CaseCtx& cctx, GAIA::NUM sUnitIndex)
			{
				GAIA::NUM sThreadIndex = cctx.listThreadIndex[sUnitIndex];
				WorkThread* pThread = m_threads[sThreadIndex];
				ThreadCtx& ctx = pThread->GetCtx();
				GAIA::SYNC::Autolock al(ctx.lr);
				UnitTask ut;
				ut.pUnit = &unit;
				ut.pCaseCtx = &cctx;
				ut.sUnitIndex = sUnitIndex;
				ctx.utasks.push_back(ut);
			}
			GINL GAIA::GVOID WaitExecuteComplete(GAIA::SYNC::Event& e, __UnitListType& units)
			{
				// Wait all unit complete.
				for(GAIA::NUM y = 0; y < units.size(); ++y)
					e.Wait((GAIA::U32)GINVALID);

				// Release all unit.
				for(GAIA::NUM y = 0; y < units.size(); ++y)
				{
					GAIA::TEST::Unit* pUnit = units[y];
					GAIA::TEST::Case* pCase = pUnit->GetCase();
					if(!pCase->ReleaseUnit(*pUnit))
					{
						if(m_desc.pCallBack != GNIL)
							m_desc.pCallBack->OnCaseError(*this, CONTROLLER_STAGE_RELEASEUNIT, *pCase);
					}
				}
				units.clear();
			}
			GINL GAIA::BL ExecuteThread(ThreadCtx& ctx)
			{
				// Swap.
				{
					GAIA::SYNC::Autolock al(ctx.lr);
					ctx.utasks_swap = ctx.utasks;
					ctx.utasks.clear();
					if(ctx.utasks_swap.empty())
						return GAIA::False;
				}

				// Dispatch.
				GAIA::BL bExistFailed = GAIA::False;
				for(; !ctx.utasks_swap.empty(); ctx.utasks_swap.pop_front())
				{
					const UnitTask& ut = ctx.utasks_swap.front();
					if(bExistFailed)
					{
						this->FireUnitEvent(*ut.pUnit);
						continue;
					}
					const GAIA::TEST::Case::Desc& desc = ut.pCaseCtx->pCase->GetDesc();
					if(desc.bMutex)
						ut.pCaseCtx->lr.Enter();
					if(!this->ExecuteUnit(*ut.pUnit, *ut.pCaseCtx, ut.sUnitIndex))
						bExistFailed = GAIA::True;
					if(desc.bMutex)
						ut.pCaseCtx->lr.Leave();
				}

				return GAIA::True;
			}

			GINL GAIA::GVOID DestroyCaseCtx(GlobalCtx& gctx, __CaseCtxListType& listCaseContext)
			{
				this->DestroyCaseCtxRecursive(listCaseContext);
				gctx.reset();
			}

			GINL GAIA::GVOID DestroyCaseCtxRecursive(__CaseCtxListType& listCaseContext)
			{
				for(__CaseCtxListType::_sizetype x = 0; x < listCaseContext.size(); ++x)
				{
					CaseCtx* pCaseCtx = listCaseContext[x];
					GAST(pCaseCtx != GNIL);
					this->DestroyCaseCtxRecursive(pCaseCtx->childs);
					gdel pCaseCtx;
				}
				listCaseContext.destroy();
			}

			GINL GAIA::GVOID BeginUnitThread()
			{
				if(m_desc.sThreadCount <= 0 || !m_desc.bMultiThread)
					return;

				GAIA::NUM sThreadCount = m_desc.sThreadCount * m_gctx.sMaxDepth;
				for(GAIA::NUM x = 0; x < sThreadCount; ++x)
				{
					WorkThread* pThread = gnew WorkThread;
					ThreadCtx& ctx = pThread->GetCtx();
					ctx.sThreadIndex = x;
					ctx.pController = this;
					ctx.pGlobalCtx = &m_gctx;
					m_threads.push_back(pThread);
				}

				for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
				{
					WorkThread* pThread = m_threads[x];
					GAST(pThread != GNIL);
					pThread->Start();
				}
			}

			GINL GAIA::GVOID WaitThreadComplete()
			{
				for(__ThreadListType::_sizetype x = 0; x < m_threads.size(); ++x)
				{
					WorkThread* pThread = m_threads[x];
					GAST(pThread != GNIL);
					pThread->StopCmd();
					pThread->Wait();
				}
			}

			GINL GAIA::GVOID DestroyThread()
			{
				for(__ThreadListType::_sizetype x = 0; x < m_threads.size(); ++x)
					gdel m_threads[x];
				m_threads.destroy();
			}

			GINL GAIA::GVOID GenerateCasePriorityList(const __CaseListType& listCase, __CasePrioListType& listCasePriority)
			{
				for(__CaseListType::_sizetype x = 0; x < listCase.size(); ++x)
				{
					GAIA::TEST::Case* pCase = listCase[x];
					GAST(pCase != GNIL);
					CasePrio cp;
					cp.pCase = pCase;
					listCasePriority.push_back(cp);
				}
				listCasePriority.sort();
			}

			GINL GAIA::GVOID GenerateCasePriorityList(const GAIA::TEST::Case& cse, __CasePrioListType& listCasePriority)
			{
				for(GAIA::NUM x = 0; x < cse.GetChildCount(); ++x)
				{
					GAIA::TEST::Case* pCase = cse.GetChild(x);
					GAST(pCase != GNIL);
					CasePrio cp;
					cp.pCase = pCase;
					listCasePriority.push_back(cp);
					pCase->drop_ref();
				}
				listCasePriority.sort();
			}

			GINL GAIA::NUM CalculateCaseUnitCount(const GAIA::TEST::Case::Desc& desc)
			{
				GAIA::NUM ret = m_rand.random() % (desc.sMaxUnitCount - desc.sMinUnitCount + 1) + desc.sMinUnitCount;
				if(m_desc.sMaxUnitCount != -1 && ret > m_desc.sMaxUnitCount)
					ret = m_desc.sMaxUnitCount;
				return ret;
			}

			GINL GAIA::N64 CalculatePracticeUnitCount(CaseCtx& cctx) const
			{
				GAIA::N64 nParentUnitCount = 1;
				if(cctx.parent != GNIL)
					nParentUnitCount *= this->CalculatePracticeUnitCount(*cctx.parent);
				return (GAIA::N64)cctx.sUnitCount * nParentUnitCount;
			}

			GINL GAIA::NUM CalculateUnitExecuteCount(GAIA::NUM sMin, GAIA::NUM sMax, GAIA::NUM sMaxLimit)
			{
				GAIA::NUM ret = m_rand.random() % (sMax - sMin + 1) + sMin;
				if(sMaxLimit != -1 && ret > sMaxLimit)
					ret = sMaxLimit;
				return ret;
			}

			GINL GAIA::NUM SelectThread(GAIA::NUM sThreadIndex)
			{
				if(m_desc.sThreadCount == 0)
					return GINVALID;
				GAIA::NUM ret = sThreadIndex % m_gctx.sMaxDepth;
				ret += (m_rand.random() % m_desc.sThreadCount) * m_gctx.sMaxDepth;
				return ret;
			}

			GINL GAIA::NUM MatchWorkset(const GAIA::TEST::__WorksetType& src, const GAIA::TEST::__WorksetType& dst) const
			{
				GAIA::NUM ret = 0;
				if(!src.empty() && !dst.empty())
					GAIA::ALGO::counts_order(src.fptr(), src.bptr(), dst.fptr(), dst.bptr(), ret);
				return ret;
			}

		private:
			Desc m_desc;
			GAIA::MATH::RandomLCG m_rand;
			__CaseListType m_cases;
			GAIA::BL m_bBegin;
			GlobalCtx m_gctx;
			__CaseCtxListType m_cctxs;
			__ThreadListType m_threads;
			GAIA::SYNC::Lock m_lrErr;
			GAIA::CTN::Set<ErrorNode> m_errs;
		};
	}
}

#endif
