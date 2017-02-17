#ifndef		__GAIA_MSYS_BASE_H__
#define		__GAIA_MSYS_BASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	if GAIA_PROFILE == GAIA_PROFILE_DEBUG
#		include <crtdbg.h>
#	endif
#endif

namespace GAIA
{
	namespace MSYS
	{
	#if GAIA_OS == GAIA_OS_WINDOWS
	#	if GAIA_PROFILE == GAIA_PROFILE_DEBUG
		inline GAIA::N32 crt_alloc_hook(
			GAIA::N32 nAllocType,
			GAIA::GVOID* pUserData,
		#if GAIA_MACHINE == GAIA_MACHINE32
			GAIA::U32 size,
		#elif GAIA_MACHINE == GAIA_MACHINE64
			GAIA::U64 size,
		#endif
			GAIA::N32 nBlockType,
			GAIA::NM nRequestNum,
			const GAIA::U8* pszFileName,
			GAIA::N32 nCodeLine)
		{
			static GAIA::U64 s_uAllocCount = 0;
			static GAIA::U64 s_uReallocCount = 0;
			static GAIA::U64 s_uFreeCount = 0;
			if(nAllocType == _HOOK_ALLOC)
				++s_uAllocCount;
			else if(nAllocType == _HOOK_REALLOC)
				++s_uReallocCount;
			else if(nAllocType == _HOOK_FREE)
				++s_uFreeCount;
			return GAIA::True;
		}
	#	endif
	#endif

		class Heap : public GAIA::RefObject
		{
		public:
			/*!
				@brief Constructor.
			*/
			GINL Heap()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_PROFILE == GAIA_PROFILE_DEBUG
				_CrtSetAllocHook(crt_alloc_hook);
			#	endif
			#endif
			}

			/*!
				@brief Allocate memory.
			*/
			virtual GAIA::GVOID* memory_alloc(const GAIA::UM& size) = 0;

			/*!
				@brief Release memory.
			*/
			virtual GAIA::GVOID memory_release(GAIA::GVOID* p) = 0;

			/*!
				@brief Get allocated memory's size(unit in bytes).
			*/
			virtual GAIA::UM memory_size(GAIA::GVOID* p) = 0;

			/*!
				@brief Get all cached(for later allocate) and allocated memory size.
			*/
			virtual GAIA::UM capacity() = 0;

			/*!
				@brief Get all allocated memory size.

				@remarks This memory size include the part of allocated memory
					which holding some index or debug information by Heap(Current module) internal.
			*/
			virtual GAIA::UM size() = 0;

			/*!
				@brief Get all allocated memory size which user could take usage.
			*/
			virtual GAIA::UM use_size() = 0;

			/*!
				@brief Get memory piece size.
			*/
			virtual GAIA::UM piece_size() = 0;

			/*!
				@brief Get allocated times.
			*/
			virtual GAIA::U64 alloc_times() = 0;
		private:
		};
	}
}

#endif
