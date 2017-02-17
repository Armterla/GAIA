#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_type(GAIA::LOG::Log& logobj)
	{
		GAIA::RefObject ro;

		if(GAIA::datatotype(GAIA::True) != GAIA::TYPEID_BL)
			TERROR;
		if(GAIA::datatotype((GAIA::N8)10) != GAIA::TYPEID_N8)
			TERROR;
		if(GAIA::datatotype((GAIA::N16)10) != GAIA::TYPEID_N16)
			TERROR;
		if(GAIA::datatotype((GAIA::N32)10) != GAIA::TYPEID_N32)
			TERROR;
		if(GAIA::datatotype((GAIA::N64)10) != GAIA::TYPEID_N64)
			TERROR;
		if(GAIA::datatotype((GAIA::NM)10) != GAIA::TYPEID_NM)
			TERROR;
		if(GAIA::datatotype((GAIA::U8)10) != GAIA::TYPEID_U8)
			TERROR;
		if(GAIA::datatotype((GAIA::U16)10) != GAIA::TYPEID_U16)
			TERROR;
		if(GAIA::datatotype((GAIA::U32)10) != GAIA::TYPEID_U32)
			TERROR;
		if(GAIA::datatotype((GAIA::U64)10) != GAIA::TYPEID_U64)
			TERROR;
		if(GAIA::datatotype((GAIA::UM)10) != GAIA::TYPEID_UM)
			TERROR;
		if(GAIA::datatotype(1.0F) != GAIA::TYPEID_F32)
			TERROR;
		if(GAIA::datatotype(1.0) != GAIA::TYPEID_F64)
			TERROR;
		if(GAIA::datatotype("hello") != GAIA::TYPEID_CONSTCHARPOINTER)
			TERROR;
		if(GAIA::datatotype(L"hello") != GAIA::TYPEID_CONSTWCHARPOINTER)
			TERROR;
		if(GAIA::datatotype((GAIA::N8*)"hello") != GAIA::TYPEID_CHARPOINTER)
			TERROR;
		if(GAIA::datatotype((GAIA::WCH*)L"hello") != GAIA::TYPEID_WCHARPOINTER)
			TERROR;
		if(GAIA::datatotype((GAIA::GVOID*)GNIL) != GAIA::TYPEID_POINTER)
			TERROR;
		if(GAIA::datatotype((const GAIA::GVOID*)GNIL) != GAIA::TYPEID_CONSTPOINTER)
			TERROR;
	}
}
