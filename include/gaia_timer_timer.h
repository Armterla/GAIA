#ifndef		__GAIA_TIME_TIMER_H__
#define		__GAIA_TIME_TIMER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_book.h"

namespace GAIA
{
	namespace TIME
	{
		class Timer : public GAIA::RefObject
		{
		public:
			friend class GAIA::TIME::TimerMgr;

		public:
			typedef GAIA::N32 __UpdateTimesType;
			typedef GAIA::N64 __MicroSecType;
			typedef GAIA::CTN::Book<Timer*> __TimerBook;

		public:
			GAIA_ENUM_BEGIN(FIRE_REASON)
				FIRE_REASON_REGIST,
				FIRE_REASON_UNREGIST,
				FIRE_REASON_PAUSE,
				FIRE_REASON_RESUME,
				FIRE_REASON_UPDATE,
			GAIA_ENUM_END(FIRE_REASON)

		public:
			class CallBack : public GAIA::Base
			{
			public:
				GINL CallBack(){}
				virtual GAIA::GVOID UpdateTimer(GAIA::TIME::Timer* pTimer, GAIA::TIME::Timer::FIRE_REASON reason) = 0;
			};

			class FireDesc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					bRegistFire = GAIA::False;
					bUnregistFire = GAIA::False;
					bPauseFire = GAIA::False;
					bResumeFire = GAIA::False;
					bUpdate = GAIA::True;
				}
				GINL GAIA::BL check() const
				{
					return GAIA::True;
				}
				GAIA::U8 bRegistFire : 1; // Fired when regist timer to TimerMgr.
				GAIA::U8 bUnregistFire : 1; // Fired when unregist timer from TimerMgr.
				GAIA::U8 bPauseFire : 1; // Fired when pause the timer.
				GAIA::U8 bResumeFire : 1; // Fired when resume the timer.
				GAIA::U8 bUpdate : 1; // Fired when the time escape reached and update the TimerMgr.
			};
			class DirDesc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					bGoFrontAble = GAIA::True;
					bGoBackAble = GAIA::False;
				}
				GINL GAIA::BL check() const
				{
					return GAIA::True;
				}
				GAIA::U8 bGoFrontAble : 1; // Specified the timer can go front.
				GAIA::U8 bGoBackAble : 1; // Specified the timer can go back.
			};
			class Desc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					descFire.reset();
					descDir.reset();
					nEscapeUSec = 0;
					nMaxUpdateTimes = 0;
					pCallBack = GNIL;
					bAutoRelease = GAIA::True;
					bMultiCallBack = GAIA::True;
				}
				GINL GAIA::BL check() const
				{
					if(!descFire.check())
						return GAIA::False;
					if(!descDir.check())
						return GAIA::False;
					if(nEscapeUSec < 0)
						return GAIA::False;
					if(nMaxUpdateTimes <= 0)
						return GAIA::False;
					if(pCallBack == GNIL)
						return GAIA::False;
					return GAIA::True;
				}
				FireDesc descFire;
				DirDesc descDir;
				__MicroSecType nEscapeUSec;
				__UpdateTimesType nMaxUpdateTimes;
				CallBack* pCallBack;
				GAIA::U8 bAutoRelease : 1; // If the timer update times equal to max update times, the timer instance will released automatically.
				GAIA::U8 bMultiCallBack : 1; // If the timer will be callback many times in single frame.
			};

		public:
			GINL Timer(){this->init();}
			GINL virtual ~Timer(){this->Destroy();}

			GINL GAIA::BL Create(const GAIA::TIME::Timer::Desc& desc)
			{
				if(!desc.check())
					return GAIA::False;
				m_desc = desc;
				return GAIA::True;
			}
			GINL GAIA::GVOID Destroy();
			GINL const GAIA::TIME::Timer::Desc& GetDesc() const{return m_desc;}

			GINL const GAIA::TIME::Timer::FireDesc& GetFireDesc() const{return m_desc.descFire;}
			GINL GAIA::BL SetFireDesc(const GAIA::TIME::Timer::FireDesc& descFire){m_desc.descFire = descFire; return GAIA::True;}

			GINL GAIA::TIME::TimerMgr* GetTimerMgr() const{return m_pTimerMgr;}
			GINL GAIA::BL IsRegisted() const{return m_pTimerMgr != GNIL;}
			GINL const __MicroSecType& GetRegistTime() const{return m_nRegistTime;}
			GINL const __MicroSecType& GetLastUpdateTime() const{return m_nLastUpdateTime;}
			GINL const __UpdateTimesType& GetUpdateTimes() const{return m_nUpdateTimes;}

			GINL GAIA::BL Pause();
			GINL GAIA::BL Resume();
			GINL GAIA::BL IsPaused() const{return m_bPaused;}
			GINL GAIA::BL Update(GAIA::TIME::Timer::FIRE_REASON reason);

		private:
			GINL GAIA::GVOID init()
			{
				m_desc.reset();
				m_nRegistTime = 0;
				m_nLastUpdateTime = 0;
				m_nUpdateTimes = 0;
				m_pTimerMgr = GNIL;
				m_sGroupIndex = GINVALID;
				m_sIndex = GINVALID;
				m_bPaused = GAIA::False;
				m_bUnregisting = GAIA::False;
			}
			GINL GAIA::GVOID SetTimerMgr(GAIA::TIME::TimerMgr* pTimerMgr){m_pTimerMgr = pTimerMgr;}
			GINL GAIA::GVOID SetRegistTime(const __MicroSecType& t){m_nRegistTime = t;}
			GINL GAIA::GVOID SetLastUpdateTime(const __MicroSecType& t){m_nLastUpdateTime = t;}
			GINL GAIA::GVOID SetUpdateTimes(const __UpdateTimesType& t){m_nUpdateTimes = t;}
			GINL GAIA::GVOID SetGroupIndex(const GAIA::NUM& index){m_sGroupIndex = index;}
			GINL const GAIA::NUM& GetGroupIndex() const{return m_sGroupIndex;}
			GINL GAIA::GVOID SetIndex(const GAIA::NUM& index){m_sIndex = index;}
			GINL const GAIA::NUM& GetIndex() const{return m_sIndex;}
			GINL GAIA::GVOID SetUnregisting(GAIA::BL bUnregisting){m_bUnregisting = bUnregisting;}
			GINL GAIA::BL GetUnregisting() const{return m_bUnregisting;}

		private:
			Desc m_desc;
			__MicroSecType m_nRegistTime;
			__MicroSecType m_nLastUpdateTime;
			__UpdateTimesType m_nUpdateTimes;
			GAIA::TIME::TimerMgr* m_pTimerMgr;
			GAIA::NUM m_sGroupIndex;
			GAIA::NUM m_sIndex;
			GAIA::U8 m_bPaused : 1;
			GAIA::U8 m_bUnregisting : 1;
		};

		class TimerMgr : public GAIA::Base
		{
		public:
			typedef GAIA::CTN::Vector<Timer*> __TimerList;

		public:
			class Desc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					sGroupCount = 32;
				}
				GINL GAIA::BL check() const
				{
					if(sGroupCount <= 0)
						return GAIA::False;
					return GAIA::True;
				}
				GAIA::NUM sGroupCount;
			};

		public:
			GINL TimerMgr(){this->init();}
			GINL ~TimerMgr(){this->Destroy();}

			GINL GAIA::BL Create(const GAIA::TIME::TimerMgr::Desc& desc)
			{
				if(!desc.check())
					return GAIA::False;
				this->init_groups();
				return GAIA::True;
			}
			GINL GAIA::GVOID Destroy()
			{
				__TimerList listTimer;
				for(GAIA::NUM x = 0; x < m_groups.size(); ++x)
				{
					Group& g = m_groups[x];
					for(GAIA::TIME::Timer::__TimerBook::it it = g.timers.frontit(); !it.empty(); ++it)
					{
						GAIA::TIME::Timer* pTimer = *it;
						listTimer.push_back(pTimer);
					}
				}
				for(GAIA::NUM x = 0; x < listTimer.size(); ++x)
				{
					GAIA::TIME::Timer* pTimer = listTimer[x];
					this->Unregist(*pTimer);
				}
				m_groups.destroy();
				m_nLastUpdateTime = 0;
				m_desc.reset();
			}
			GINL const GAIA::TIME::TimerMgr::Desc& GetDesc() const{return m_desc;}

			GINL GAIA::BL Regist(GAIA::TIME::Timer& timer)
			{
				if(this->IsRegisted(timer))
					return GAIA::False;

				/* Add reference. */
				timer.rise_ref();

				/* Regist. */
				GAIA::NUM sGroupIndex = this->GetGroupIndexByTime(timer.GetDesc().nEscapeUSec);
				GAST(sGroupIndex != GINVALID);
				if(sGroupIndex >= m_groups.size())
					sGroupIndex = m_groups.size() - 1;
				Group& g = m_groups[sGroupIndex];
				GAIA::NUM sUsedIndex = g.timers.insert(&timer);
				timer.SetTimerMgr(this);
				timer.SetRegistTime(this->GetLastUpdateTime());
				timer.SetLastUpdateTime(this->GetLastUpdateTime());
				timer.SetGroupIndex(sGroupIndex);
				timer.SetIndex(g.timers.fixedindex(sUsedIndex));

				/* Regist fire. */
				if(timer.GetDesc().descFire.bRegistFire)
					timer.Update(GAIA::TIME::Timer::FIRE_REASON_REGIST);

				return GAIA::True;
			}
			GINL GAIA::BL Unregist(GAIA::TIME::Timer& timer)
			{
				if(!this->IsRegisted(timer))
					return GAIA::False;

				if(timer.GetUnregisting())
					return GAIA::True;

				timer.SetUnregisting(GAIA::True);
				{
					/* Unregist fire. */
					if(timer.GetDesc().descFire.bUnregistFire)
						timer.Update(GAIA::TIME::Timer::FIRE_REASON_UNREGIST);
					if(timer.GetDesc().bAutoRelease)
						timer.drop_ref();

					/* Unregist. */
					GAIA::NUM sGroupIndex = timer.GetGroupIndex();
					GAIA::NUM sFixedIndex = timer.GetIndex();
					GAST(sGroupIndex != GINVALID);
					GAST(sFixedIndex != GINVALID);
					Group& g = m_groups[sGroupIndex];
					GAIA::NUM sUsedIndex = g.timers.usedindex(sFixedIndex);
					if(!g.timers.erase(sUsedIndex))
					{
						GASTFALSE;
						return GAIA::False;
					}
					timer.SetTimerMgr(GNIL);
					timer.SetGroupIndex(GINVALID);
					timer.SetIndex(GINVALID);
				}
				timer.SetUnregisting(GAIA::False);

				/* Release. */
				timer.drop_ref();

				return GAIA::True;
			}
			GINL GAIA::BL IsRegisted(GAIA::TIME::Timer& timer) const
			{
				GAIA::TIME::TimerMgr* pTimerMgr = timer.GetTimerMgr();
				if(pTimerMgr == GNIL)
					return GAIA::False;
				if(pTimerMgr != this)
					return GAIA::False;
			#ifdef GAIA_DEBUG_SELFCHECK
				GAST(timer.GetGroupIndex() != GINVALID);
				GAST(timer.GetGroupIndex() < m_groups.size());
				const Group& g = m_groups[timer.GetGroupIndex()];
				GAIA::NUM sUseIndex = g.timers.usedindex(timer.GetIndex());
				GAST(sUseIndex != GINVALID);
				GAST(g.timers[sUseIndex] == &timer);
			#endif
				return GAIA::True;
			}

			GINL GAIA::GVOID Update(const GAIA::TIME::Timer::__MicroSecType& nEscape)
			{
				m_nLastUpdateTime += nEscape;
				for(GAIA::NUM x = 0; x < m_groups.size(); ++x)
				{
					Group& g = m_groups[x];
					if(x == 0)
					{
						this->SwitchToTempTimerList(g);
						{
							for(__TimerList::it it = m_TempTimerList.frontit(); !it.empty(); ++it)
							{
								GAIA::TIME::Timer* pTimer = *it;
								GAST(pTimer != GNIL);
								pTimer->SetLastUpdateTime(this->GetLastUpdateTime());
								if(pTimer->Update(GAIA::TIME::Timer::FIRE_REASON_UPDATE))
									this->Unregist(*pTimer);
							}
							g.nLastUpdate = this->GetLastUpdateTime();
						}
						this->SwitchToTempTimerListEnd();
					}
					else
					{
						/* Get need update for current group. */
						GAIA::BL bNeedUpdate = GAIA::False;
						GAIA::TIME::Timer::__MicroSecType nGroupOffsetTime = this->GetLastUpdateTime() - g.nLastUpdate;
						if(nGroupOffsetTime > m_groups[x - 1].nEscape)
							bNeedUpdate = GAIA::True;

						/* If current group need update, update it. */
						if(bNeedUpdate)
						{
							this->SwitchToTempTimerList(g);
							{
								for(__TimerList::it it = m_TempTimerList.frontit(); !it.empty(); ++it)
								{
									GAIA::TIME::Timer* pTimer = *it;
									GAIA::TIME::Timer::__MicroSecType nOffsetTime = this->GetLastUpdateTime() - pTimer->GetLastUpdateTime();
									const GAIA::TIME::Timer::Desc& descTimer = pTimer->GetDesc();
									GAIA::BL bComplete = GAIA::False;
									if(nOffsetTime > descTimer.nEscapeUSec)
									{
										if(pTimer->GetDesc().bMultiCallBack)
										{
											while(this->GetLastUpdateTime() - pTimer->GetLastUpdateTime() > descTimer.nEscapeUSec)
											{
												pTimer->SetLastUpdateTime(pTimer->GetLastUpdateTime() + pTimer->GetDesc().nEscapeUSec);
												if(pTimer->Update(GAIA::TIME::Timer::FIRE_REASON_UPDATE))
												{
													this->Unregist(*pTimer);
													bComplete = GAIA::True;
													break;
												}
											}
										}
										else
										{
											GAIA::TIME::Timer::__MicroSecType nFireTimes = nOffsetTime / descTimer.nEscapeUSec;
											pTimer->SetLastUpdateTime(pTimer->GetLastUpdateTime() + nFireTimes * pTimer->GetDesc().nEscapeUSec);
											if(pTimer->Update(GAIA::TIME::Timer::FIRE_REASON_UPDATE))
											{
												this->Unregist(*pTimer);
												bComplete = GAIA::True;
											}
										}
									}
									if(!bComplete)
										this->SwitchTimerToNewGroup(*pTimer);
								}
							}
							this->SwitchToTempTimerListEnd();
						}
					}
				}
			}
			GINL const GAIA::TIME::Timer::__MicroSecType& GetLastUpdateTime() const{return m_nLastUpdateTime;}

		private:
			class Group : public GAIA::Base
			{
			public:
				GAIA::TIME::Timer::__MicroSecType nEscape;
				GAIA::TIME::Timer::__MicroSecType nLastUpdate;
				GAIA::TIME::Timer::__TimerBook timers;
			};

		private:
			GINL GAIA::GVOID init()
			{
				m_desc.reset();
				m_nLastUpdateTime = 0;
			}
			GINL GAIA::GVOID init_groups()
			{
				GAST(m_groups.empty());
				m_groups.resize(m_desc.sGroupCount);
				for(GAIA::NUM x = 0; x < m_desc.sGroupCount; ++x)
				{
					Group& g = m_groups[x];
					g.nEscape = x * x * 1000 * 1000; // The first group's escape must zero for always update.
					g.nLastUpdate = 0;
				}
				m_groups.back().nEscape = GAIA::N64MAX;
			}

			GINL GAIA::NUM GetGroupIndexByTime(const GAIA::TIME::Timer::__MicroSecType& nEscapeUSec) const
			{
				if(nEscapeUSec == 0)
					return 0;
				GAIA::F64 fGroupIndex = GAIA::MATH::gsqrt(GSCAST(GAIA::F64)(nEscapeUSec) / 1000.0 / 1000.0);
				return GSCAST(GAIA::NUM)(GAIA::MATH::gceil(fGroupIndex));
			}
			GINL GAIA::GVOID SwitchTimerToNewGroup(GAIA::TIME::Timer& timer)
			{
				GAST(timer.IsRegisted());
				GAIA::TIME::Timer::__MicroSecType nOffsetTime = this->GetLastUpdateTime() - timer.GetLastUpdateTime();
				GAIA::NUM sNewGroupIndex = this->GetGroupIndexByTime(nOffsetTime);
				GAST(sNewGroupIndex != GINVALID);
				if(sNewGroupIndex != timer.GetGroupIndex())
				{
					GAST(sNewGroupIndex < m_groups.size());

					/* Erase from old group. */
					Group& gold = m_groups[timer.GetGroupIndex()];
					GAIA::NUM sOldUsedIndex = gold.timers.usedindex(timer.GetIndex());
					GAST(sOldUsedIndex != GINVALID);
					gold.timers.erase(sOldUsedIndex);

					/* Add to new group. */
					Group& g = m_groups[sNewGroupIndex];
					GAIA::NUM sNewUsedIndex = g.timers.insert(&timer);
					GAIA::NUM sNewFixedIndex = g.timers.fixedindex(sNewUsedIndex);

					/* Update index. */
					timer.SetGroupIndex(sNewGroupIndex);
					timer.SetIndex(sNewFixedIndex);
				}
			}
			GINL GAIA::GVOID SwitchToTempTimerList(Group& g)
			{
				GAST(m_TempTimerList.empty());
				for(GAIA::TIME::Timer::__TimerBook::it it = g.timers.frontit(); !it.empty(); ++it)
					m_TempTimerList.push_back(*it);
			}
			GINL GAIA::GVOID SwitchToTempTimerListEnd()
			{
				m_TempTimerList.clear();
			}
		private:
			typedef GAIA::CTN::Vector<Group> __GroupList;

		private:
			Desc m_desc;
			__GroupList m_groups;
			__TimerList m_TempTimerList;
			GAIA::TIME::Timer::__MicroSecType m_nLastUpdateTime;
		};
	}
}

#endif
