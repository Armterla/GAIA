#include "preheader.h"
#include "t_common.h"

#include <typeinfo>

namespace TEST
{
	#define DERIVED_METHOD public virtual
	class BaseBaseForVirtualDerived : DERIVED_METHOD GAIA::Base
	{
	public:
		virtual GAIA::N32 func(){return m_nbase;}
		const GAIA::GVOID* GetThisPtr() const{return this;}
	public:
		GAIA::N32 m_nbase;
	};
	class Base1ForVirtualDerived : DERIVED_METHOD BaseBaseForVirtualDerived
	{
	public:
		virtual GAIA::N32 func() const{return m_n1;}
		const GAIA::GVOID* GetThisPtr() const{return this;}
	public:
		GAIA::N32 m_n1;
	};
	class Base2ForVirtualDerived : DERIVED_METHOD BaseBaseForVirtualDerived
	{
	public:
		virtual GAIA::N32 func() const{return m_n2;}
		const GAIA::GVOID* GetThisPtr() const{return this;}
	public:
		GAIA::N32 m_n2;
	};
	class BaseExtendForVirtualDerived : DERIVED_METHOD Base1ForVirtualDerived, DERIVED_METHOD Base2ForVirtualDerived
	{
	public:
		virtual GAIA::N32 func() const{return m_v;}
		const GAIA::GVOID* GetThisPtr() const{return this;}
	public:
		GAIA::NUM m_v;
	};
	
	extern GAIA::GVOID tp_compiler(GAIA::LOG::Log& logobj)
	{
		BaseExtendForVirtualDerived n;
		BaseExtendForVirtualDerived* pn = &n;
		Base1ForVirtualDerived* p1 = pn;
		Base2ForVirtualDerived* p2 = pn;
		BaseBaseForVirtualDerived* pbase = pn;
		GAIA::GVOID* pvoid = pn;
		if(((GAIA::N64)p1) == ((GAIA::N64)pn))
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)p1 << ", Right = " << (GAIA::N64)pn << "\n";
			TERROR;
		}
		if(((GAIA::N64)p2) == ((GAIA::N64)pn))
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)p2 << ", Right = " << (GAIA::N64)pn << "\n";
			TERROR;
		}
		if(((GAIA::N64)pbase) == ((GAIA::N64)pn))
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)pbase << ", Right = " << (GAIA::N64)pn << "\n";
			TERROR;
		}
		if(((GAIA::N64)pvoid) != ((GAIA::N64)pn))
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)pvoid << ", Right = " << (GAIA::N64)pn << "\n";
			TERROR;
		}
		if(((GAIA::N64)p1) != ((GAIA::N64)p1->GetThisPtr()))
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)p1 << ", Right = " << (GAIA::N64)pn << "\n";
			TERROR;
		}
		if(((GAIA::N64)p2) != ((GAIA::N64)p2->GetThisPtr()))
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)p2 << ", Right = " << (GAIA::N64)pn << "\n";
			TERROR;
		}
		if(((GAIA::N64)pbase) != ((GAIA::N64)pbase->GetThisPtr()))
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)pbase << ", Right = " << (GAIA::N64)pn << "\n";
			TERROR;
		}
		if(((GAIA::N64)pn) != ((GAIA::N64)pn->GetThisPtr()))
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)pvoid << ", Right = " << (GAIA::N64)pn << "\n";
			TERROR;
		}
		if(n.GetThisPtr() == n.Base1ForVirtualDerived::GetThisPtr())
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)n.GetThisPtr() << ", Right = " << (GAIA::N64)n.Base1ForVirtualDerived::GetThisPtr() << "\n";
			TERROR;
		}
		if(n.GetThisPtr() == n.Base2ForVirtualDerived::GetThisPtr())
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)n.GetThisPtr() << ", Right = " << (GAIA::N64)n.Base2ForVirtualDerived::GetThisPtr() << "\n";
			TERROR;
		}
		if(n.GetThisPtr() == n.BaseBaseForVirtualDerived::GetThisPtr())
		{
			g_gaia_stdstream << "Left = " << (GAIA::N64)n.GetThisPtr() << ", Right = " << (GAIA::N64)n.BaseBaseForVirtualDerived::GetThisPtr() << "\n";
			TERROR;
		}
		
		GAIA::STREAM::StringStream s;

		s << "[TYPE SIZE]" << GAIA_FILELINEBREAK_RN;
		s << "bool size = " << sizeof(bool) << GAIA_FILELINEBREAK_RN;
		s << "wchar_t size = " << sizeof(wchar_t) << GAIA_FILELINEBREAK_RN;
		s << "char size = " << sizeof(char) << GAIA_FILELINEBREAK_RN;
		s << "short size = " << sizeof(short) << GAIA_FILELINEBREAK_RN;
		s << "int size = " << sizeof(int) << GAIA_FILELINEBREAK_RN;
		s << "long size = " << sizeof(long) << GAIA_FILELINEBREAK_RN;
		s << "long int size = " << sizeof(long int) << GAIA_FILELINEBREAK_RN;
		s << "long long int size = " << sizeof(long long int) << GAIA_FILELINEBREAK_RN;
		s << "float size = " << sizeof(float) << GAIA_FILELINEBREAK_RN;
		s << "double size = " << sizeof(double) << GAIA_FILELINEBREAK_RN;
		s << "size_t size = " << sizeof(size_t) << GAIA_FILELINEBREAK_RN;

		s << GAIA_FILELINEBREAK_RN << "[TYPE NAME]" << GAIA_FILELINEBREAK_RN;
		s << "bool type name = " << typeid(bool).name() << GAIA_FILELINEBREAK_RN;
		s << "wchar_t type name = " << typeid(wchar_t).name() << GAIA_FILELINEBREAK_RN;
		s << "char type name = " << typeid(char).name() << GAIA_FILELINEBREAK_RN;
		s << "short type name = " << typeid(short).name() << GAIA_FILELINEBREAK_RN;
		s << "int type name = " << typeid(int).name() << GAIA_FILELINEBREAK_RN;
		s << "long type name = " << typeid(long).name() << GAIA_FILELINEBREAK_RN;
		s << "long int type name = " << typeid(long int).name() << GAIA_FILELINEBREAK_RN;
		s << "long long int type name = " << typeid(long long int).name() << GAIA_FILELINEBREAK_RN;
		s << "unsigned char type name = " << typeid(unsigned char).name() << GAIA_FILELINEBREAK_RN;
		s << "unsigned short type name = " << typeid(unsigned short).name() << GAIA_FILELINEBREAK_RN;
		s << "unsigned int type name = " << typeid(unsigned int).name() << GAIA_FILELINEBREAK_RN;
		s << "unsigned long type name = " << typeid(unsigned long).name() << GAIA_FILELINEBREAK_RN;
		s << "unsigned long int type name = " << typeid(unsigned long int).name() << GAIA_FILELINEBREAK_RN;
		s << "unsigned long long int type name = " << typeid(unsigned long long int).name() << GAIA_FILELINEBREAK_RN;
		s << "float type name = " << typeid(float).name() << GAIA_FILELINEBREAK_RN;
		s << "double type name = " << typeid(double).name() << GAIA_FILELINEBREAK_RN;
		s << "size_t type name = " << typeid(size_t).name() << GAIA_FILELINEBREAK_RN;

		GAIA::FSYS::File f;
		GAIA::CH* pszFileName = gnew GAIA::CH[GAIA::MAXPL];
		GAIA::ALGO::gstrcpy(pszFileName, g_gaia_appdocdir);
		GAIA::ALGO::gstrcat(pszFileName, _T("compilerinfo.txt"));
		if(!f.Open(pszFileName, GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS | GAIA::FSYS::File::OPEN_TYPE_WRITE))
		{
			TERROR;
			gdel[] pszFileName;
			return;
		}
		gdel[] pszFileName;
		if(!GAIA::ALGO::gstremp(s.getstring()))
		{
			GAIA::CH* psz = GAIA::ALGO::gstrnewex<GAIA::CH>(s.getstring());
			f.WriteText(psz);
			GAIA_DELETEARRAY_SAFE(psz);
		}

		{
			GAIA::BL b;
			GAIA::N8 n8;
			GAIA::U8 u8;
			GAIA::N16 n16;
			GAIA::U16 u16;
			GAIA::N32 n32;
			GAIA::U32 u32;
			GAIA::N64 n64;
			GAIA::U64 u64;
			GAIA::NM nm;
			GAIA::UM um;
			GAIA::F32 f32;
			GAIA::F64 f64;
			GAIA::WCH wch;
			GAIA::X128 x128;
			if(!GAIA::isbasetype(b) || 
				!GAIA::isbasetype(n8) || 
				!GAIA::isbasetype(u8) || 
				!GAIA::isbasetype(n16) ||
				!GAIA::isbasetype(u16) ||
				!GAIA::isbasetype(n32) ||
				!GAIA::isbasetype(u32) ||
				!GAIA::isbasetype(n64) ||
				!GAIA::isbasetype(u64) ||
				!GAIA::isbasetype(nm) ||
				!GAIA::isbasetype(um) ||
				!GAIA::isbasetype(f32) ||
				!GAIA::isbasetype(f64) ||
				!GAIA::isbasetype(wch) ||
				!GAIA::isbasetype(x128))
			{
				TERROR;
			}
			GAIA::CTN::TString str;
			if(GAIA::isbasetype(str))
				TERROR;
		}

		f.Close();
	}
}
