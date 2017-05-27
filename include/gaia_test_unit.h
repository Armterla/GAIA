#ifndef		__GAIA_TEST_UNIT_H__
#define		__GAIA_TEST_UNIT_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_event.h"
#include "gaia_test_context.h"

namespace GAIA
{
	namespace TEST
	{
		class Unit : public GAIA::Entity
		{
			friend class Controller;
		protected:
			GINL Unit(){this->init();}
		public:
			GINL ~Unit(){this->destruct();}

		private:
			GINL GAIA::GVOID SetCase(GAIA::TEST::Case* pCase){m_pCase = pCase;}
			GINL GAIA::GVOID SetParent(GAIA::TEST::Unit* pParent){m_pParent = pParent;}
			GINL GAIA::GVOID SetBindEvent(GAIA::SYNC::Event* pEvent){m_pEvent = pEvent;}
			GINL GAIA::SYNC::Event* GetBindEvent() const{return m_pEvent;}
			
			virtual GAIA::BL Begin(){return GAIA::False;}
			virtual GAIA::BL End(){return GAIA::False;}
			virtual GAIA::BL ExecuteFront(){return GAIA::True;}
			virtual GAIA::BL ExecuteBack(){return GAIA::True;}

		protected:
			virtual GAIA::TEST::Context* GetContext(){return GNIL;}
			virtual GAIA::BL IsBegin() const{return GAIA::False;}
			GINL GAIA::TEST::Case* GetCase() const{return m_pCase;}
			GINL GAIA::TEST::Unit* GetParent() const{return m_pParent;}
			GINL GAIA::TEST::Unit* GetRoot() const
			{
				if(m_pParent == GNIL)
					return GNIL;
				GAIA::TEST::Unit* pRoot = m_pParent;
				if(pRoot->m_pParent != GNIL)
					pRoot = pRoot->m_pParent;
				return pRoot;
			}
			GINL GAIA::BL Error(const GAIA::CH* pszFileName, GAIA::N32 nCodeLine);

		private:
			GINL GAIA::GVOID init()
			{
				m_pCase = GNIL;
				m_pParent = GNIL;
				m_pEvent = GNIL;
			}

			GINL GAIA::GVOID destruct()
			{
				if(this->IsBegin())
					this->End();
			}

		private:
			GAIA::TEST::Case* m_pCase;
			Unit* m_pParent;
			GAIA::SYNC::Event* m_pEvent;
		};
	}
}

#endif
