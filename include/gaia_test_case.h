#ifndef		__GAIA_TEST_CASE_H__
#define		__GAIA_TEST_CASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_string.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_book.h"

namespace GAIA
{
	namespace TEST
	{
		typedef GAIA::CTN::Vector<GAIA::CTN::TString> __WorksetType;

		class Case : public GAIA::RefObject
		{
			friend class Controller;

		public:
			class Desc : public GAIA::Entity
			{
			public:
				GINL GAIA::GVOID reset()
				{
					listWorkset.clear();
					strName.clear();
					sPriority = 0;
					sMinUnitCount = 1;
					sMaxUnitCount = 1;
					sMinExecuteFrontCount = 1;
					sMaxExecuteFrontCount = 1;
					sMinExecuteBackCount = 1;
					sMaxExecuteBackCount = 1;
					uMinExecuteSleepTime = 0;
					uMaxExecuteSleepTime = 0;
					bMultiThread = GAIA::True;
					bMutex = GAIA::True;
				}
				GINL GAIA::BL check() const
				{
					if(strName.empty())
						return GAIA::False;
					if(sMinUnitCount < 0 || sMaxUnitCount < 0 || sMinUnitCount > sMaxUnitCount)
						return GAIA::False;
					if(sMinExecuteFrontCount < 0 || sMaxExecuteFrontCount < 0 || sMinExecuteFrontCount > sMaxExecuteFrontCount)
						return GAIA::False;
					if(sMinExecuteBackCount < 0 || sMaxExecuteBackCount < 0 || sMinExecuteBackCount > sMaxExecuteBackCount)
						return GAIA::False;
					if(uMinExecuteSleepTime > uMaxExecuteSleepTime)
						return GAIA::False;
					return GAIA::True;
				}
			public:
				GAIA::TEST::__WorksetType listWorkset;
				GAIA::CTN::TString strName;
				GAIA::NUM sPriority; // Execute from 0 to +infinite.
				GAIA::NUM sMinUnitCount;
				GAIA::NUM sMaxUnitCount;
				GAIA::NUM sMinExecuteFrontCount;
				GAIA::NUM sMaxExecuteFrontCount;
				GAIA::NUM sMinExecuteBackCount;
				GAIA::NUM sMaxExecuteBackCount;
				GAIA::U64 uMinExecuteSleepTime;
				GAIA::U64 uMaxExecuteSleepTime;
				GAIA::U8 bMultiThread : 1;
				GAIA::U8 bMutex : 1;
			};

		public:
			GINL Case(){this->init();}
			GINL virtual ~Case(){this->destruct();}

			GINL GAIA::BL SetDesc(const GAIA::TEST::Case::Desc& desc)
			{
				GAST(desc.check());
				if(!desc.check())
					return GAIA::False;
				m_desc = desc;
				m_desc.listWorkset.sort();
				m_desc.listWorkset.unique();
				return GAIA::True;
			}
			GINL const GAIA::TEST::Case::Desc& GetDesc() const{return m_desc;}

			GINL GAIA::BL SetParent(Case* pParent)
			{
				if(pParent == m_pParent)
					return GAIA::True;
				if(m_pParent != GNIL)
				{
					GAIA::NUM usedindex = m_pParent->m_childs.usedindex(m_sParentIndex);
					if(!m_pParent->m_childs.erase(usedindex))
						return GAIA::False;
					m_pParent = GNIL;
					m_sParentIndex = GINVALID;
					this->drop_ref();
				}
				if(pParent != GNIL)
				{
					m_pParent = pParent;
					GAIA::NUM usedindex = m_pParent->m_childs.insert(this);
					m_sParentIndex = m_pParent->m_childs.fixedindex(usedindex);
					this->rise_ref();
				}
				return GAIA::True;
			}
			GINL Case* GetParent() const
			{
				if(m_pParent != GNIL)
					m_pParent->rise_ref();
				return m_pParent;
			}
			GINL Case* GetRoot() const
			{
				Case* pRoot = m_pParent;
				while(pRoot != GNIL)
				{
					if(pRoot->m_pParent != GNIL)
						pRoot = pRoot->m_pParent;
					else
						break;
				}
				if(pRoot != GNIL)
					pRoot->rise_ref();
				return pRoot;
			}
			GINL GAIA::NUM GetChildCount() const{return m_childs.size();}
			GINL Case* GetChild(GAIA::NUM sIndex) const
			{
				GAST(sIndex < m_childs.size());
				if(sIndex >= m_childs.size())
					return GNIL;
				m_childs[sIndex]->rise_ref();
				return m_childs[sIndex];
			}
			GINL GAIA::BL RemoveChildAll()
			{
				for(GAIA::NUM x = 0; x < m_childs.size(); ++x)
				{
					Case* pChild = m_childs[x];
					GAST(pChild != GNIL);
					pChild->SetParent(GNIL);
				}
				return GAIA::True;
			}

			GINL GAIA::TEST::Controller* GetController() const{return m_pController;}
			GINL GAIA::BL Error(const GAIA::CH* pszFileName, GAIA::N32 nCodeLine);

		private:
			GINL GAIA::GVOID init()
			{
				m_desc.reset();
				m_pParent = GNIL;
				m_sParentIndex = GINVALID;
				m_pController = GNIL;
				m_sControllerIndex = GINVALID;
			}

			GINL GAIA::GVOID destruct()
			{
				this->RemoveChildAll();
				this->SetParent(GNIL);
			}

			GINL GAIA::GVOID SetController(GAIA::TEST::Controller* pController){m_pController = pController;}
			GINL GAIA::GVOID SetControllerIndex(GAIA::NUM sIndex){m_sControllerIndex = sIndex;}
			GINL GAIA::NUM GetControllerIndex() const{return m_sControllerIndex;}

			virtual GAIA::TEST::Unit* CreateUnit() = 0;
			virtual GAIA::BL ReleaseUnit(GAIA::TEST::Unit& unit) = 0;

		private:
			typedef GAIA::CTN::Book<Case*> __CaseListType;

		private:
			Desc m_desc;
			Case* m_pParent;
			GAIA::NUM m_sParentIndex;
			__CaseListType m_childs;
			GAIA::TEST::Controller* m_pController;
			GAIA::NUM m_sControllerIndex;
		};
	}
}

#endif
