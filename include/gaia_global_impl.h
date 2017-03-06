#ifndef		__GAIA_GLOBAL_IMPL_H__
#define		__GAIA_GLOBAL_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_platform.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_msys_heapcrt.h"
#include "gaia_msys_heapesg.h"
#include "gaia_msys_leakdetector.h"
#include "gaia_ctn_staticstringpool.h"
#include "gaia_ctn_stringpool.h"
#include "gaia_ctn_staticstringptrpool.h"
#include "gaia_ctn_stringptrpool.h"
#include "gaia_ctn_staticbufferpool.h"
#include "gaia_ctn_bufferpool.h"
#include "gaia_log.h"
#include "gaia_global_decl.h"

#include "gaia_type_impl.h"
#include "gaia_timer_timer_impl.h"
#include "gaia_thread_base_impl.h"
#include "gaia_assert_impl.h"
#include "gaia_xml_xmlnode_impl.h"
#include "gaia_xml_xml_impl.h"
#include "gaia_xml_xmlfactory_impl.h"
#include "gaia_html_htmlnode_impl.h"
#include "gaia_html_html_impl.h"
#include "gaia_html_htmlfactory_impl.h"
#include "gaia_json_jsonnode_impl.h"
#include "gaia_json_json_impl.h"
#include "gaia_json_jsonfactory_impl.h"
#include "gaia_network_base_impl.h"
#include "gaia_network_socket_impl.h"
#include "gaia_test_unit_impl.h"
#include "gaia_test_case_impl.h"
#include "gaia_math_rid_impl.h"

/* Global std stream. */
GAIA::STREAM::STDStream g_gaia_stdstream;
GAIA::STREAM::InvalidStream g_gaia_invalidstream;

/* Global assert mode. */
GAIA::BL g_gaia_assertconfirm = GAIA::True;

/* Global memory system(msys). */
GAIA::MSYS::HeapESG g_gaia_globalmsys;

#ifdef GAIA_DEBUG_MEMORYLEAK
	GAIA::MSYS::LeakDetector g_gaia_leakdetector;
#endif

/* Global directory. */
GAIA::CH g_gaia_appdocdir[GAIA::MAXPL] = {0};
GAIA::CH g_gaia_apptempdir[GAIA::MAXPL] = {0};

/* Platform. */
GAIA::Platform g_gaia_platform;

/* Log. */
GAIA::LOG::Log g_gaia_log;
GAIA::LOG::InvalidLog g_gaia_invalidlog;
DefaultGAIALogCallBack g_gaia_log_callback;

/* Global cache pool */
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Lock g_gaia_lr_staticstringptrpool;
	GAIA::SYNC::Lock g_gaia_lr_staticwstringptrpool;
	GAIA::SYNC::Lock g_gaia_lr_staticbufferpool;
	GAIA::SYNC::Lock g_gaia_lr_stringptrpool;
	GAIA::SYNC::Lock g_gaia_lr_wstringptrpool;
	GAIA::SYNC::Lock g_gaia_lr_bufferpool;
#endif
GAIA::CTN::StaticStringPtrPool<GAIA::CH> g_gaia_staticstringptrpool;
GAIA::CTN::StaticStringPtrPool<GAIA::WCH> g_gaia_staticwstringptrpool;
GAIA::CTN::StaticBufferPool g_gaia_staticbufferpool;
GAIA::CTN::StringPtrPool<GAIA::CH> g_gaia_stringptrpool;
GAIA::CTN::StringPtrPool<GAIA::WCH> g_gaia_wstringptrpool;
GAIA::CTN::BufferPool g_gaia_bufferpool;
const GAIA::CH* gaia_alloc_staticstring(const GAIA::CH* psz)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_staticstringptrpool);
#endif
	return g_gaia_staticstringptrpool.alloc(psz);
}
const GAIA::WCH* gaia_alloc_staticwstring(const GAIA::WCH* psz)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_staticwstringptrpool);
#endif
	return g_gaia_staticwstringptrpool.alloc(psz);
}
const GAIA::U8* gaia_alloc_staticbuffer(const GAIA::GVOID* p, GAIA::NUM sSize)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_staticbufferpool);
#endif
	return g_gaia_staticbufferpool.alloc(p, sSize);
}
const GAIA::CH* gaia_alloc_string(const GAIA::CH* psz)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_stringptrpool);
#endif
	return g_gaia_stringptrpool.alloc(psz);
}
GAIA::BL gaia_release_string(const GAIA::CH* psz)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_stringptrpool);
#endif
	return g_gaia_stringptrpool.release(psz);
}
const GAIA::WCH* gaia_alloc_wstring(const GAIA::WCH* psz)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_wstringptrpool);
#endif
	return g_gaia_wstringptrpool.alloc(psz);
}
GAIA::BL gaia_release_wstring(const GAIA::WCH* psz)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_wstringptrpool);
#endif
	return g_gaia_wstringptrpool.release(psz);
}
const GAIA::U8* gaia_alloc_buffer(const GAIA::GVOID* p, GAIA::NUM sSize)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_bufferpool);
#endif
	return g_gaia_bufferpool.alloc(p, sSize);
}
GAIA::BL gaia_release_buffer(const GAIA::GVOID* p, GAIA::NUM sSize)
{
#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
	GAIA::SYNC::Autolock al(g_gaia_lr_bufferpool);
#endif
	return g_gaia_bufferpool.release(p, sSize);
}

/* PerfCollector. */
GAIA::DBG::PerfCollector g_gaia_perf;

GAIA::GVOID gaia_reset_global_variables()
{
	g_gaia_log.Destroy();
	g_gaia_log_callback.reset();
	g_gaia_perf.Reset();

	// Release global pool.
	{
		// Static string ptr pool.
		{
		#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
			GAIA::SYNC::Autolock al(g_gaia_lr_staticstringptrpool);
		#endif
			g_gaia_staticstringptrpool.destroy();
		}

		// Static wstring ptr pool.
		{
		#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
			GAIA::SYNC::Autolock al(g_gaia_lr_staticwstringptrpool);
		#endif
			g_gaia_staticwstringptrpool.destroy();
		}

		// Static buffer pool.
		{
		#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
			GAIA::SYNC::Autolock al(g_gaia_lr_staticbufferpool);
		#endif
			g_gaia_staticbufferpool.destroy();
		}

		// Dynamic string ptr pool.
		{
		#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
			GAIA::SYNC::Autolock al(g_gaia_lr_stringptrpool);
		#endif
			g_gaia_stringptrpool.destroy();
		}

		// Dynamic wstring ptr pool.
		{
		#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
			GAIA::SYNC::Autolock al(g_gaia_lr_wstringptrpool);
		#endif
			g_gaia_wstringptrpool.destroy();
		}

		// Dynamic buffer pool.
		{
		#if GAIA_THREADSAFE == GAIA_THREADSAFE_ON
			GAIA::SYNC::Autolock al(g_gaia_lr_bufferpool);
		#endif
			g_gaia_bufferpool.destroy();
		}
	}
}

#ifndef GAIA_DEBUG_CATCHEXCEPTION
	GAIA::ECT::Ect e(GNIL, 0, GNIL);
#endif

#endif
