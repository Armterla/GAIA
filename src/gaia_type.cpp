#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_global_decl.h>
#include <gaia_type_impl.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
#if defined(GAIA_HEAP)
#	if GAIA_OS == GAIA_OS_OSX
	    GAIA::GVOID* Base::operator new(GAIA::UM size GAIA_NEW_INFO)
		{
		#if defined(GAIA_HEAP_FORCLASS)
			GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
		#else
			GAIA::GVOID* ret = malloc(size);
		#endif
		#ifdef GAIA_DEBUG_MEMORYLEAK
			g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
		#endif
			return ret;
		}
		GAIA::GVOID* Base::operator new[](GAIA::UM size GAIA_NEW_INFO)
		{
		#if defined(GAIA_HEAP_FORCLASS)
			GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
		#else
			GAIA::GVOID* ret = malloc(size);
		#endif
		#ifdef GAIA_DEBUG_MEMORYLEAK
			g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
		#endif
			return ret;
		}
#	else
#		if GAIA_MACHINE == GAIA_MACHINE32
	        GAIA::GVOID* Base::operator new(GAIA::U32 size GAIA_NEW_INFO)
			{
			#if defined(GAIA_HEAP_FORCLASS)
				GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
			#else
				GAIA::GVOID* ret = malloc(size);
			#endif
			#ifdef GAIA_DEBUG_MEMORYLEAK
				g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
			#endif
				return ret;
			}
			GAIA::GVOID* Base::operator new[](GAIA::U32 size GAIA_NEW_INFO)
			{
			#if defined(GAIA_HEAP_FORCLASS)
				GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
			#else
				GAIA::GVOID* ret = malloc(size);
			#endif
			#ifdef GAIA_DEBUG_MEMORYLEAK
				g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
			#endif
				return ret;
			}
#		elif GAIA_MACHINE == GAIA_MACHINE64
	        GAIA::GVOID* Base::operator new(GAIA::U64 size GAIA_NEW_INFO)
			{
			#if defined(GAIA_HEAP_FORCLASS)
				GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
			#else
				GAIA::GVOID* ret = malloc(size);
			#endif
			#ifdef GAIA_DEBUG_MEMORYLEAK
				g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
			#endif
				return ret;
			}
			GAIA::GVOID* Base::operator new[](GAIA::U64 size GAIA_NEW_INFO)
			{
			#if defined(GAIA_HEAP_FORCLASS)
				GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
			#else
				GAIA::GVOID* ret = malloc(size);
			#endif
			#ifdef GAIA_DEBUG_MEMORYLEAK
				g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
			#endif
				return ret;
			}
#		endif
#	endif
		GAIA::GVOID Base::operator delete(GAIA::GVOID* p)
		{
		#ifdef GAIA_DEBUG_MEMORYLEAK
			g_gaia_leakdetector.release(p);
		#endif
		#if defined(GAIA_HEAP_FORCLASS)
			return g_gaia_globalmsys.memory_release(p);
		#else
			free(p);
		#endif
		}
		GAIA::GVOID Base::operator delete[](GAIA::GVOID* p)
		{
		#ifdef GAIA_DEBUG_MEMORYLEAK
			g_gaia_leakdetector.release(p);
		#endif
		#if defined(GAIA_HEAP_FORCLASS)
			return g_gaia_globalmsys.memory_release(p);
		#else
			free(p);
		#endif
		}
#endif

#ifdef GAIA_DEBUG_SELFCHECK
	RefObject::~RefObject()
	{
		GAST(!m_bDestructingByDropRef || (this->get_ref() >= 0 && this->get_ref() <= 1));
	}
#endif

#ifdef GAIA_DEBUG_SOLUTION
	GAIA::GVOID RefObject::debug_change_ref(GAIA::BL bRise, GAIA::NM nNewRef, const GAIA::CH* pszReason)
	{
		if(nNewRef < 0)
			GERR << "GAIA::RefObject::debug_change_ref: Object reference count drop to below zero!" << GEND;
		if(m_bDestructingByDropRef)
			GERR << "GAIA::RefObject::debug_change_ref: Change reference count when destructing!" << GEND;
		GAST(nNewRef >= 0);
		GAST(!m_bDestructingByDropRef);
		if(bRise)
		{
		}
		else
		{
		}
	}
#endif
}
#if defined(GAIA_HEAP)
#	if GAIA_OS == GAIA_OS_OSX
        GAIA::GVOID* operator new(GAIA::UM size GAIA_NEW_INFO)
		{
		#if defined(GAIA_HEAP_FORGLOBAL)
			GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
		#else
			GAIA::GVOID* ret = malloc(size);
		#endif
		#ifdef GAIA_DEBUG_MEMORYLEAK
			g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
		#endif
			return ret;
		}
		GAIA::GVOID* operator new[](GAIA::UM size GAIA_NEW_INFO)
		{
		#if defined(GAIA_HEAP_FORGLOBAL)
			GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
		#else
			GAIA::GVOID* ret = malloc(size);
		#endif
		#ifdef GAIA_DEBUG_MEMORYLEAK
			g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
		#endif
			return ret;
		}
#	else
#		if GAIA_MACHINE == GAIA_MACHINE32
            GAIA::GVOID* operator new(GAIA::U32 size GAIA_NEW_INFO)
			{
			#if defined(GAIA_HEAP_FORGLOBAL)
				GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
			#else
				GAIA::GVOID* ret = malloc(size);
			#endif
			#ifdef GAIA_DEBUG_MEMORYLEAK
				g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
			#endif
				return ret;
			}
			GAIA::GVOID* operator new[](GAIA::U32 size GAIA_NEW_INFO)
			{
			#if defined(GAIA_HEAP_FORGLOBAL)
				GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
			#else
				GAIA::GVOID* ret = malloc(size);
			#endif
			#ifdef GAIA_DEBUG_MEMORYLEAK
				g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
			#endif
				return ret;
			}
#		elif GAIA_MACHINE == GAIA_MACHINE64
            GAIA::GVOID* operator new(GAIA::U64 size GAIA_NEW_INFO)
			{
			#if defined(GAIA_HEAP_FORGLOBAL)
				GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
			#else
				GAIA::GVOID* ret = malloc(size);
			#endif
			#ifdef GAIA_DEBUG_MEMORYLEAK
				g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
			#endif
				return ret;
			}
			GAIA::GVOID* operator new[](GAIA::U64 size GAIA_NEW_INFO)
			{
			#if defined(GAIA_HEAP_FORGLOBAL)
				GAIA::GVOID* ret = g_gaia_globalmsys.memory_alloc(size);
			#else
				GAIA::GVOID* ret = malloc(size);
			#endif
			#ifdef GAIA_DEBUG_MEMORYLEAK
				g_gaia_leakdetector.alloc(ret, pszFileName, nCodeLine, size);
			#endif
				return ret;
			}
#		endif
#	endif
	GAIA::GVOID operator delete(GAIA::GVOID* p) throw()
	{
	#ifdef GAIA_DEBUG_MEMORYLEAK
		g_gaia_leakdetector.release(p);
	#endif
	#if defined(GAIA_HEAP_FORGLOBAL)
		return g_gaia_globalmsys.memory_release(p);
	#else
		free(p);
	#endif
	}
	GAIA::GVOID operator delete[](GAIA::GVOID* p) throw()
	{
	#ifdef GAIA_DEBUG_MEMORYLEAK
		g_gaia_leakdetector.release(p);
	#endif
	#if defined(GAIA_HEAP_FORGLOBAL)
		return g_gaia_globalmsys.memory_release(p);
	#else
		free(p);
	#endif
	}
#endif
