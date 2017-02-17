#ifndef		__GAIA_TEST_CASE_IMPL_H__
#define		__GAIA_TEST_CASE_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_test_case.h"
#include "gaia_test_controller.h"

namespace GAIA
{
	namespace TEST
	{
		GINL GAIA::BL Case::Error(const GAIA::CH* pszFileName, GAIA::N32 nCodeLine)
		{
			if(m_pParent != GNIL)
				return m_pParent->Error(pszFileName, nCodeLine);
			else if(m_pController != GNIL)
				return m_pController->Error(pszFileName, nCodeLine);
			return GAIA::False;
		}
	}
}

#endif
