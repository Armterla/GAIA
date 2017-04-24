#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	typedef GAIA::CTN::BasicTree<GAIA::CTN::TString, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __TestTreeType;
	
	class TestCaseRoot : public GAIA::TEST::Case
	{
	public:
		class Unit : public GAIA::TEST::Unit
		{
		public:
			class Ctx : public GAIA::TEST::Context
			{
			public:
				GINL Ctx(){pTree = GNIL; pRootNode = GNIL;}
				__TestTreeType* pTree;
				__TestTreeType::Node* pRootNode;
			};
		public:
			GINL Unit(){this->init();}
			virtual GAIA::TEST::Context* GetContext(){return &m_ctx;}
			virtual GAIA::BL Begin()
			{
				if(this->IsBegin())
					return GAIA::False;
				if(this->GetParent() != GNIL)
					return GAIA::False;
				m_ctx.pTree = gnew __TestTreeType;
				m_ctx.pTree->insert(_T("SuperRoot"), GNIL);
				m_ctx.pRootNode = m_ctx.pTree->insert(_T("Root"), m_ctx.pTree->root());
				m_bBegin = GAIA::True;
				return GAIA::True;
			}
			virtual GAIA::BL End()
			{
				if(!this->IsBegin())
					return GAIA::False;
				gdel m_ctx.pTree;
				m_bBegin = GAIA::False;
				return GAIA::True;
			}
			virtual GAIA::BL IsBegin() const
			{
				return m_bBegin;
			}
			virtual GAIA::BL ExecuteFront()
			{
				return GAIA::True;
			}
			virtual GAIA::BL ExecuteBack()
			{
				return GAIA::True;
			}
		private:
			GINL GAIA::GVOID init()
			{
				m_bBegin = GAIA::False;
			}
		private:
			Ctx m_ctx;
			GAIA::BL m_bBegin;
		};
	public:
		virtual GAIA::TEST::Unit* CreateUnit(){return gnew Unit;}
		virtual GAIA::BL ReleaseUnit(GAIA::TEST::Unit& unit){gdel &unit; return GAIA::True;}
	};

	class TestCaseChild : public GAIA::TEST::Case
	{
	public:
		class Unit : public GAIA::TEST::Unit
		{
		public:
			class Ctx : public GAIA::TEST::Context
			{
			public:
				GINL Ctx(){pTree = GNIL; pChildNode = GNIL;}
				__TestTreeType* pTree;
				__TestTreeType::Node* pChildNode;
			};
		public:
			GINL Unit(){this->init();}
			virtual GAIA::TEST::Context* GetContext(){return &m_ctx;}
			virtual GAIA::BL Begin()
			{
				if(this->IsBegin())
					return GAIA::False;
				TestCaseRoot::Unit* pRoot = (TestCaseRoot::Unit*)this->GetParent();
				if(pRoot == GNIL)
					return GAIA::False;
				TestCaseRoot::Unit::Ctx* pRootCtx = (TestCaseRoot::Unit::Ctx*)pRoot->GetContext();
				if(pRootCtx == GNIL)
					return GAIA::False;
				m_ctx.pTree = pRootCtx->pTree;
				m_bBegin = GAIA::True;
				return GAIA::True;
			}
			virtual GAIA::BL End()
			{
				if(!this->IsBegin())
					return GAIA::False;
				m_bBegin = GAIA::False;
				return GAIA::True;
			}
			virtual GAIA::BL IsBegin() const
			{
				return m_bBegin;
			}
			virtual GAIA::BL ExecuteFront()
			{
				TestCaseRoot::Unit* pRoot = (TestCaseRoot::Unit*)this->GetParent();
				if(pRoot == GNIL)
					return GAIA::False;
				TestCaseRoot::Unit::Ctx* pRootCtx = (TestCaseRoot::Unit::Ctx*)pRoot->GetContext();
				if(pRootCtx == GNIL)
					return GAIA::False;

				if(m_ctx.pChildNode == GNIL)
				{
					m_ctx.pChildNode = m_ctx.pTree->insert(_T("Child"), pRootCtx->pRootNode);
					if(m_ctx.pChildNode == GNIL)
						return GAIA::False;
				}

				return GAIA::True;
			}
			virtual GAIA::BL ExecuteBack()
			{
				return GAIA::True;
			}
		private:
			GINL GAIA::GVOID init()
			{
				m_bBegin = GAIA::False;
			}
		private:
			Ctx m_ctx;
			GAIA::BL m_bBegin;
		};
	public:
		virtual GAIA::TEST::Unit* CreateUnit(){return gnew Unit;}
		virtual GAIA::BL ReleaseUnit(GAIA::TEST::Unit& unit){gdel &unit; return GAIA::True;}
	};

	class TestCaseChildChild : public GAIA::TEST::Case
	{
	public:
		class Unit : public GAIA::TEST::Unit
		{
		public:
			class Ctx : public GAIA::TEST::Context
			{
			public:
				GINL Ctx(){pTree = GNIL; pChildChildNode = GNIL;}
				__TestTreeType* pTree;
				__TestTreeType::Node* pChildChildNode;
			};
		public:
			GINL Unit(){this->init();}
			virtual GAIA::TEST::Context* GetContext(){return &m_ctx;}
			virtual GAIA::BL Begin()
			{
				if(this->IsBegin())
					return GAIA::False;
				TestCaseChild::Unit* pChild = (TestCaseChild::Unit*)this->GetParent();
				if(pChild == GNIL)
					return GAIA::False;
				TestCaseChild::Unit::Ctx* pChildCtx = (TestCaseChild::Unit::Ctx*)pChild->GetContext();
				if(pChildCtx == GNIL)
					return GAIA::False;
				m_ctx.pTree = pChildCtx->pTree;
				m_bBegin = GAIA::True;
				return GAIA::True;
			}
			virtual GAIA::BL End()
			{
				if(!this->IsBegin())
					return GAIA::False;
				m_bBegin = GAIA::False;
				return GAIA::True;
			}
			virtual GAIA::BL IsBegin() const
			{
				return m_bBegin;
			}
			virtual GAIA::BL ExecuteFront()
			{
				TestCaseChild::Unit* pChild = (TestCaseChild::Unit*)this->GetParent();
				if(pChild == GNIL)
					return GAIA::False;
				TestCaseChild::Unit::Ctx* pChildCtx = (TestCaseChild::Unit::Ctx*)pChild->GetContext();
				if(pChildCtx == GNIL)
					return GAIA::False;

				if(m_ctx.pChildChildNode == GNIL)
				{
					m_ctx.pChildChildNode = m_ctx.pTree->insert(_T("ChildChild"), pChildCtx->pChildNode);
					if(m_ctx.pChildChildNode == GNIL)
						return GAIA::False;
				}

				return GAIA::True;
			}
			virtual GAIA::BL ExecuteBack()
			{
				return GAIA::True;
			}
		private:
			GINL GAIA::GVOID init()
			{
				m_bBegin = GAIA::False;
			}
		private:
			Ctx m_ctx;
			GAIA::BL m_bBegin;
		};

	public:
		virtual GAIA::TEST::Unit* CreateUnit(){return gnew Unit;}
		virtual GAIA::BL ReleaseUnit(GAIA::TEST::Unit& unit){gdel &unit; return GAIA::True;}
	};

	class TestCaseIgnore : public GAIA::TEST::Case
	{
	public:
		class Unit : public GAIA::TEST::Unit
		{
		public:
			class Ctx : public GAIA::TEST::Context
			{
			public:
				GINL Ctx(){}
			};
		public:
			GINL Unit(){this->init();}
			virtual GAIA::TEST::Context* GetContext(){return &m_ctx;}
			virtual GAIA::BL Begin()
			{
				if(this->IsBegin())
					return GAIA::False;
				m_bBegin = GAIA::True;
				return GAIA::True;
			}
			virtual GAIA::BL End()
			{
				if(!this->IsBegin())
					return GAIA::False;
				m_bBegin = GAIA::False;
				return GAIA::True;
			}
			virtual GAIA::BL IsBegin() const
			{
				return m_bBegin;
			}
			virtual GAIA::BL ExecuteFront()
			{
				return GAIA::True;
			}
			virtual GAIA::BL ExecuteBack()
			{
				return GAIA::True;
			}
		private:
			GINL GAIA::GVOID init()
			{
				m_bBegin = GAIA::False;
			}
		private:
			Ctx m_ctx;
			GAIA::BL m_bBegin;
		};
	public:
		GINL TestCaseIgnore(GAIA::LOG::Log& logobj){m_pLog = &logobj;}
		virtual GAIA::TEST::Unit* CreateUnit()
		{
			GAIA::LOG::Log& logobj = *m_pLog;
			TERROR;
			return gnew Unit;
		}
		virtual GAIA::BL ReleaseUnit(GAIA::TEST::Unit& unit)
		{
			GAIA::LOG::Log& logobj = *m_pLog;
			TERROR;
			gdel &unit;
			return GAIA::True;
		}
	private:
		GAIA::LOG::Log* m_pLog;
	};

	extern GAIA::GVOID t_test(GAIA::LOG::Log& logobj)
	{
		// Empty controller test.
		{
			GAIA::TEST::Controller::Desc descCtl;
			descCtl.reset();

			GAIA::TEST::Controller ctl;
			TAST(ctl.SetDesc(descCtl));

			if(ctl.GetCaseCount() != 0)
				TERROR;
			TAST(ctl.EraseAll());

			TAST(ctl.Begin());
			TAST(ctl.End());

			if(ctl.GetCaseCount() != 0)
				TERROR;
			TAST(ctl.EraseAll());
		}

		// Sync case test.
		{
			//
			GAIA::TEST::Controller::Desc descCtl;
			descCtl.reset();
			descCtl.sMaxExecuteBackCount = descCtl.sMaxExecuteFrontCount = 3;
			descCtl.sMaxUnitCount = 7;
			descCtl.listIgnoreWorkset.push_back(_T("Ignore1"));
			descCtl.listIgnoreWorkset.push_back(_T("Ignore2"));

			GAIA::TEST::Controller ctl;
			TAST(ctl.SetDesc(descCtl));

			GAIA::TEST::Case::Desc descCase;
			descCase.reset();
			descCase.sMinExecuteFrontCount = descCase.sMaxExecuteFrontCount = 5;
			descCase.sMinExecuteBackCount = descCase.sMaxExecuteBackCount = 7;
			descCase.sMaxUnitCount = descCase.sMinUnitCount = 4;

			//
			TestCaseRoot tcRoot;
			descCase.strName = "tcRoot";
			tcRoot.SetDesc(descCase);
			ctl.Insert(tcRoot);

			//
			TestCaseChild tcChild;
			descCase.strName = "tcChild";
			tcChild.SetDesc(descCase);
			tcChild.SetParent(&tcRoot);

			//
			TestCaseChildChild tcChildChild;
			descCase.strName = "tcChildChild";
			tcChildChild.SetDesc(descCase);
			tcChildChild.SetParent(&tcChild);

			//
			TestCaseIgnore tcIgnore1(logobj);
			descCase.strName = "tcIgnore1";
			descCase.listWorkset.push_back(_T("Ignore1"));
			descCase.listWorkset.push_back(_T("Ignore1"));
			descCase.listWorkset.push_back(_T("Ignore1"));
			tcIgnore1.SetDesc(descCase);
			tcIgnore1.SetParent(&tcChildChild);

			//
			TestCaseIgnore tcIgnore2(logobj);
			descCase.strName = "tcIgnore2";
			descCase.listWorkset.push_back(_T("Ignore2"));
			tcIgnore2.SetDesc(descCase);
			tcIgnore2.SetParent(&tcIgnore1);

			//
			for(GAIA::NUM x = 0; x < 5; ++x)
			{
				if(!ctl.Begin())
				{
					TERROR;
					break;
				}
				if(!ctl.End())
				{
					TERROR;
					break;
				}
			}

			ctl.Erase(tcRoot);
		}
	}
}
