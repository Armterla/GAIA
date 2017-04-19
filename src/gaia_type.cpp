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

#if defined(GAIA_DEBUG_SELFCHECK) || defined(GAIA_DEBUG_SOLUTION)
	RefObject::~RefObject()
	{
		GAST(!m_bDestructingByDropRef || (this->get_ref() >= 0 && this->get_ref() <= 1));
	#ifdef GAIA_DEBUG_SOLUTION
		GWATCH_END(this, &m_uuid, 4);
	#	if GAIA_OS == GAIA_OS_WINDOWS
			::DeleteCriticalSection(&m_cs);
	#	else
			pthread_mutex_destroy(&m_mutex);
	#	endif
	#endif
	}
#endif

#ifdef GAIA_DEBUG_SOLUTION
	GAIA::GVOID RefObject::debug_constructor()
	{
		GWATCH_BEGIN(this, &m_uuid, 4);
	}
	GAIA::GVOID RefObject::debug_change_ref(GAIA::BL bRise, GAIA::NM nNewRef, const GAIA::CH* pszReason, GAIA::BL bDestructingByDropRef)
	{
		GAIA::CTN::ACharsString strTemp;
		GAIA::CH szNewRef[32];
		GAIA::ALGO::castv(nNewRef, szNewRef, sizeof(szNewRef));
		if(bRise)
		{
			strTemp = "RefObject refcnt rise to ";
			strTemp += szNewRef;
		}
		else
		{
			strTemp = "RefObject refcnt drop to ";
			strTemp += szNewRef;
		}
		if(pszReason != GNIL)
		{
			strTemp += ", Reason=";
			if(pszReason[0] == '\0')
				strTemp += "reason string is a empty(\\0) string";
			else
				strTemp += pszReason;
		}
		GWATCH_UPDATE(this, strTemp.fptr(), &m_uuid, 4);

		if(nNewRef < 0)
		{
			strTemp.clear();
			if(pszReason != GNIL)
			{
				strTemp = ", Reason=";
				if(pszReason[0] == '\0')
					strTemp += "reason string is a empty(\\0) string";
				else
					strTemp += pszReason;
			}

			GAIA::CH szUid[33];
			m_uuid.tostring(szUid);
			GAIA::UM uThreadID = GAIA::THREAD::threadid();
			if(bRise)
			{
				GERR << "GAIA::RefObject::debug_change_ref: "
					 << (GAIA::U64)this
					 << ", Uid=" << szUid
					 << ", Tid=" << uThreadID
					 << ", object reference count rise to " << szNewRef << strTemp.fptr()
					 << GEND;
			}
			else
			{
				GERR << "GAIA::RefObject::debug_change_ref: "
					 << (GAIA::U64)this
					 << ", Uid=" << szUid
					 << ", Tid=" << uThreadID
					 << ", object reference count drop to " << szNewRef << strTemp.fptr()
					 << GEND;
			}
		}

		if(bDestructingByDropRef)
		{
			strTemp.clear();
			if(pszReason != GNIL)
			{
				strTemp = ", Reason=";
				if(pszReason[0] == '\0')
					strTemp += "reason string is a empty(\\0) string";
				else
					strTemp += pszReason;
			}

			GAIA::CH szUid[33];
			m_uuid.tostring(szUid);
			GAIA::UM uThreadID = GAIA::THREAD::threadid();
			if(bRise)
			{
				GERR << "GAIA::RefObject::debug_change_ref: "
					 << (GAIA::U64)this
					 << ", Uid=" << szUid
					 << ", Tid=" << uThreadID
					 << ", rise reference count to " << szNewRef << " when destructing" << strTemp.fptr()
					 << GEND;
			}
			else
			{
				GERR << "GAIA::RefObject::debug_change_ref: "
					 << (GAIA::U64)this
					 << ", Uid=" << szUid
					 << ", Tid=" << uThreadID
					 << ", drop reference count to " << szNewRef << " when destructing" << strTemp.fptr()
					 << GEND;
			}
		}
		GAST(nNewRef >= 0);
		GAST(!bDestructingByDropRef);
		GAST(nNewRef <= 0xFF);
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
