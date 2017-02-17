#ifndef		__GAIA_JSON_JSONFACTORYDESC_H__
#define		__GAIA_JSON_JSONFACTORYDESC_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace JSON
	{
		class JsonFactoryDesc : public GAIA::Base
		{
		public:
			GINL GAIA::GVOID reset()
			{
				bThreadSafe = GAIA::True;
				bObjectPoolEnable = GAIA::True;
				bStringPoolEnable = GAIA::True;
				bStaticStringPoolEnable = GAIA::True;
			}

			GINL GAIA::BL check() const
			{
				return GAIA::True;
			}
		public:
			GAIA::U8 bThreadSafe : 1;
			GAIA::U8 bObjectPoolEnable : 1;
			GAIA::U8 bStringPoolEnable : 1;
			GAIA::U8 bStaticStringPoolEnable : 1;
		};
	}
}

#endif
