#ifndef		__GAIA_TYPE_IMPL_H__
#define		__GAIA_TYPE_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_global_decl.h"

namespace GAIA
{
	template<typename _ParamDataType> GAIA::BL X128::check(const _ParamDataType* psz) const
	{
		if(psz == GNIL)
			return GAIA::False;
		if(GAIA_INTERNAL_NAMESPACE::gstrlen(psz) != 32)
			return GAIA::False;
		for(GAIA::NUM x = 0; x < 32; ++x)
		{
			if((psz[x] >= '0' && psz[x] <= '9') || 
				(psz[x] >= 'a' && psz[x] <= 'f') || 
				(psz[x] >= 'A' && psz[x] <= 'F')){}
			else
				return GAIA::False;
		}
		return GAIA::True;
	}
	template<typename _ParamDataType> GAIA::GVOID X128::fromstring(const _ParamDataType* psz)
	{
		GAIA_INTERNAL_NAMESPACE::str2hex(psz, sizeofarray(u8), u8);
	}
	template<typename _ParamDataType> _ParamDataType* X128::tostring(_ParamDataType* psz) const
	{
		GAIA_INTERNAL_NAMESPACE::hex2str(u8, sizeofarray(u8), psz);
		return psz;
	}

	GINL TYPEID nametotype(const GAIA::CH* psz)
	{
		for(GAIA::N32 x = 0; x < sizeofarray(TYPEID_ANAME); ++x)
		{
			if(GAIA_INTERNAL_NAMESPACE::gstrcmp(TYPEID_ANAME[x], psz) == 0)
				return GSCAST(TYPEID)(x);
		}
		return TYPEID_INVALID;
	}
	GINL TYPEID nametotype(const GAIA::WCH* psz)
	{
		for(GAIA::N32 x = 0; x < sizeofarray(TYPEID_WNAME); ++x)
		{
			if(GAIA_INTERNAL_NAMESPACE::gstrcmp(TYPEID_WNAME[x], psz) == 0)
				return GSCAST(TYPEID)(x);
		}
		return TYPEID_INVALID;
	}
}

#endif
