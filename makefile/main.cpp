#include "../test/preheader.h"

#include <gaia_sysconfig.h>
#if GAIA_COMPILER == GAIA_COMPILER_VC && GAIA_PROFILE == GAIA_PROFILE_DEBUG
#	define	_CRTDBG_MAP_ALLOC
#	include	<stdlib.h>
#	include	<stdio.h>
#	include <assert.h>
#	define HEAPCHECK assert(_CrtCheckMemory());
#else
#	define HEAPCHECK
#endif

#include <gaia.h>
#include <gaia_global_impl.h>
#include "../test/t_header.h"

int main()
{
	TEST::test();
	return 0;
}
