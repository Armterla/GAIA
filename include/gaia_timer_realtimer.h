#ifndef		__GAIA_TIME_REALTIMER_H__
#define		__GAIA_TIME_REALTIMER_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace TIME
	{
		class RealTimer : public GAIA::RefObject
		{
		public:
			class RealTimerDesc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
				}
				GINL GAIA::BL check() const
				{
					return GAIA::True;
				}
			};

		public:
			GINL RealTimer(){this->init();}
			GINL virtual ~RealTimer(){this->Destroy();}

			GINL GAIA::BL Create(const GAIA::TIME::RealTimer::RealTimerDesc& desc);
			GINL GAIA::GVOID Destroy(){}
			GINL const GAIA::TIME::RealTimer::RealTimerDesc& GetDesc() const{return m_desc;}

		private:
			GINL GAIA::GVOID init()
			{
			}

		private:
			GAIA::TIME::RealTimer::RealTimerDesc m_desc;
		};

		class RealTimerMgr : public GAIA::Base
		{
		public:
			class RealTimerMgrDesc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
				}
				GINL GAIA::BL check() const
				{
					return GAIA::True;
				}
			};

		public:
			GINL RealTimerMgr(){this->init();}
			GINL ~RealTimerMgr(){this->Destroy();}

			GINL GAIA::BL Create(const GAIA::TIME::RealTimerMgr::RealTimerMgrDesc& desc);
			GINL GAIA::GVOID Destroy(){}
			GINL const GAIA::TIME::RealTimerMgr::RealTimerMgrDesc& GetDesc() const{return m_desc;}

			GINL GAIA::BL Regist(GAIA::TIME::RealTimer* pRealTimer);
			GINL GAIA::BL Unregist(GAIA::TIME::RealTimer* pRealTimer);
			GINL GAIA::BL IsRegisted(GAIA::TIME::RealTimer* pRealTimer) const;

			GINL GAIA::BL Update(const GAIA::TIME::Time& t);

		private:
			GINL GAIA::GVOID init()
			{
			}

		private:
			GAIA::TIME::RealTimerMgr::RealTimerMgrDesc m_desc;
		};
	}
}

#endif
