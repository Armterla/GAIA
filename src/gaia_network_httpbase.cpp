#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_algo_search.h>
#include <gaia_network_httpbase.h>
#include <gaia_global_decl.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		GINL const GAIA::CH* HttpHead::requestname(const GAIA::CH* pszName)
		{
			GAST(GAIA::ALGO::gstremp(pszName));

			// Allocate from statiac name list.
			for(GAIA::NUM x = 0; x < sizeofarray(HTTP_HEADNAME_LIST); ++x)
			{
				if(GAIA::ALGO::gstrequal(HTTP_HEADNAME_LIST[x], pszName))
					return HTTP_HEADNAME_LIST[x];
			}

			// Allocate from global static pool.
			return gaia_alloc_staticstring(pszName);
		}
		GINL const GAIA::CH* requestvalue(const GAIA::CH* pszValue)
		{
			GAST(pszValue != GNIL);

			// Allocate from global pool.
			return gaia_alloc_string(pszValue);
		}
		GINL GAIA::BL releasevalue(const GAIA::CH* pszValue)
		{
			GAST(pszValue != GNIL);

			// Release to global pool.
			return gaia_release_string(pszValue);
		}
	}
}