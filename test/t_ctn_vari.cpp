#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_vari(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::Vari __VariType;

		__VariType v1, v2;
		v1.reset();
		if(v1.check())
			TERROR;
		v2 = v1;
		if(v2.check())
			TERROR;

		/* NM. */
		v1 = GSCAST(GAIA::NM)(1);
		if(v1.type() != GAIA::TYPEID_NM || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_NM || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_NM || v2.type() != GAIA::TYPEID_NM)
			TERROR;
		if((GAIA::NM)v1 != 1 || (GAIA::NM)v2 != 1)
			TERROR;

		/* UM. */
		v1 = GSCAST(GAIA::UM)(1);
		if(v1.type() != GAIA::TYPEID_UM || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_UM || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_UM || v2.type() != GAIA::TYPEID_UM)
			TERROR;
		if((GAIA::UM)v1 != 1 || (GAIA::UM)v2 != 1)
			TERROR;

		/* BL. */
		v1 = GAIA::True;
		if(v1.type() != GAIA::TYPEID_BL || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_BL || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_BL || v2.type() != GAIA::TYPEID_BL)
			TERROR;
		if((GAIA::BL)v1 != GAIA::True || (GAIA::BL)v2 != GAIA::True)
			TERROR;

		/* N8. */
		v1 = GSCAST(GAIA::N8)(1);
		if(v1.type() != GAIA::TYPEID_N8 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_N8 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_N8 || v2.type() != GAIA::TYPEID_N8)
			TERROR;
		if((GAIA::N8)v1 != 1 || (GAIA::N8)v2 != 1)
			TERROR;

		/* N16. */
		v1 = GSCAST(GAIA::N16)(1);
		if(v1.type() != GAIA::TYPEID_N16 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_N16 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_N16 || v2.type() != GAIA::TYPEID_N16)
			TERROR;
		if((GAIA::N16)v1 != 1 || (GAIA::N16)v2 != 1)
			TERROR;

		/* N32. */
		v1 = GSCAST(GAIA::N32)(1);
		if(v1.type() != GAIA::TYPEID_N32 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_N32 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_N32 || v2.type() != GAIA::TYPEID_N32)
			TERROR;
		if((GAIA::N32)v1 != 1 || (GAIA::N32)v2 != 1)
			TERROR;

		/* N64. */
		v1 = GSCAST(GAIA::N64)(1);
		if(v1.type() != GAIA::TYPEID_N64 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_N64 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_N64 || v2.type() != GAIA::TYPEID_N64)
			TERROR;
		if((GAIA::N64)v1 != 1 || (GAIA::N64)v2 != 1)
			TERROR;

		/* U8. */
		v1 = GSCAST(GAIA::U8)(1);
		if(v1.type() != GAIA::TYPEID_U8 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_U8 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_U8 || v2.type() != GAIA::TYPEID_U8)
			TERROR;
		if((GAIA::U8)v1 != 1 || (GAIA::U8)v2 != 1)
			TERROR;

		/* U16. */
		v1 = GSCAST(GAIA::U16)(1);
		if(v1.type() != GAIA::TYPEID_U16 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_U16 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_U16 || v2.type() != GAIA::TYPEID_U16)
			TERROR;
		if((GAIA::U16)v1 != 1 || (GAIA::U16)v2 != 1)
			TERROR;

		/* U32. */
		v1 = GSCAST(GAIA::U32)(1);
		if(v1.type() != GAIA::TYPEID_U32 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_U32 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_U32 || v2.type() != GAIA::TYPEID_U32)
			TERROR;
		if((GAIA::U32)v1 != 1 || (GAIA::U32)v2 != 1)
			TERROR;

		/* U64. */
		v1 = GSCAST(GAIA::U64)(1);
		if(v1.type() != GAIA::TYPEID_U64 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_U64 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_U64 || v2.type() != GAIA::TYPEID_U64)
			TERROR;
		if((GAIA::U64)v1 != 1 || (GAIA::U64)v2 != 1)
			TERROR;

		/* F32. */
		v1 = GSCAST(GAIA::F32)(1);
		if(v1.type() != GAIA::TYPEID_F32 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_F32 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_F32 || v2.type() != GAIA::TYPEID_F32)
			TERROR;
		if((GAIA::F32)v1 != 1 || (GAIA::F32)v2 != 1)
			TERROR;

		/* F64. */
		v1 = GSCAST(GAIA::F64)(1);
		if(v1.type() != GAIA::TYPEID_F64 || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_F64 || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_F64 || v2.type() != GAIA::TYPEID_F64)
			TERROR;
		if((GAIA::F64)v1 != 1 || (GAIA::F64)v2 != 1)
			TERROR;

		/* Pointer. */
		GAIA::GVOID* p = GNIL;
		v1 = p;
		if(v1.type() != GAIA::TYPEID_POINTER || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_POINTER || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_POINTER || v2.type() != GAIA::TYPEID_POINTER)
			TERROR;
		if((GAIA::GVOID*)v1 != GNIL || (GAIA::GVOID*)v2 != GNIL)
			TERROR;

		/* ConstPointer. */
		const GAIA::GVOID* cp = GNIL;
		v1 = cp;
		if(v1.type() != GAIA::TYPEID_CONSTPOINTER || !v1.check())
			TERROR;
		v2 = v1;
		if(v2.type() != GAIA::TYPEID_CONSTPOINTER || !v2.check())
			TERROR;
		if(v1.type() != GAIA::TYPEID_CONSTPOINTER || v2.type() != GAIA::TYPEID_CONSTPOINTER)
			TERROR;
		if((const GAIA::GVOID*)v1 != GNIL || (const GAIA::GVOID*)v2 != GNIL)
			TERROR;
	}
}
