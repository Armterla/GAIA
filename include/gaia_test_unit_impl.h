#ifndef		__GAIA_TEST_UNIT_IMPL_H__
#define		__GAIA_TEST_UNIT_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_test_unit.h"
#include "gaia_test_case.h"

namespace GAIA
{
	namespace TEST
	{
		GINL GAIA::BL Unit::Error(const GAIA::CH* pszFileName, GAIA::N32 nCodeLine)
		{
			if(m_pCase != GNIL)
				return m_pCase->Error(pszFileName, nCodeLine);
			return GAIA::False;
		}
	}
}

#endif
