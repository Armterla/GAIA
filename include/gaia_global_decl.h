#ifndef		__GAIA_GLOBAL_DECL_H__
#define		__GAIA_GLOBAL_DECL_H__

#include "gaia_type.h"
#include "gaia_stream_stdstream.h"
#include "gaia_assert.h"
#include "gaia_platform.h"
#include "gaia_msys_heapesg.h"
#include "gaia_msys_leakdetector.h"
#include "gaia_algo_string.h"
#include "gaia_ctn_charsstring.h"
#include "gaia_time.h"
#include "gaia_fsys_file.h"
#include "gaia_log.h"
#include "gaia_dbg_perf.h"
#include "gaia_dbg_objwatcher.h"

/* Process time. */
extern GAIA::U64 g_gaia_processlaunchtime;

/* Global std stream. */
extern GAIA::STREAM::STDStream g_gaia_stdstream;
extern GAIA::STREAM::InvalidStream g_gaia_invalidstream;

/* Global assert mode. */
extern GAIA::BL g_gaia_assertconfirm;
extern GAIA::BL g_gaia_assertinterrupt;
extern GAIA::BL g_gaia_assertprintconsole;
extern GAIA::BL g_gaia_assertlogout;
extern GAIA::BL g_gaia_assertdebugconfirm;
extern GAIA::BL g_gaia_assertdebuginterrupt;
extern GAIA::BL g_gaia_assertdebugprintconsole;
extern GAIA::BL g_gaia_assertdebuglogout;

/* Global memory system(msys). */
extern GAIA::MSYS::HeapESG g_gaia_globalmsys;
#ifdef GAIA_DEBUG_MEMORYLEAK
	extern GAIA::MSYS::LeakDetector g_gaia_leakdetector;
#endif

/* Global directory. */
extern GAIA::CH g_gaia_appdocdir[GAIA::MAXPL];
extern GAIA::CH g_gaia_apptempdir[GAIA::MAXPL];

/* Platform. */
extern GAIA::Platform g_gaia_platform;

/* Log. */
extern GAIA::LOG::Log g_gaia_log;
extern GAIA::LOG::InvalidLog g_gaia_invalidlog;
class DefaultGAIALogCallBack : public GAIA::LOG::Log::CallBack
{
public:
	GINL DefaultGAIALogCallBack()
	{
		this->init();
		g_gaia_log.SetCallBack(this);
	}
	GINL ~DefaultGAIALogCallBack()
	{
		this->reset();
	}
	GINL GAIA::GVOID reset()
	{
		if(m_pFile != GNIL)
		{
			if(m_pFile->IsOpen())
				m_pFile->Close();
			GAIA_DELETE_SAFE(m_pFile);
		}
	}
	GINL GAIA::GVOID SetLogFileName(const GAIA::CH* pszFileName){m_strLogFilePathName = pszFileName;}
	GINL const GAIA::CH* GetLogFileName() const{return m_strLogFilePathName.fptr();}
	virtual GAIA::BL WriteLog(
		GAIA::LOG::Log& logobj,
		const GAIA::TIME::Time& logtime,
		GAIA::LOG::Log::TYPE type,
		GAIA::LOG::Log::__FilterType userfilter,
		const GAIA::TCH* pszLog)
	{
		/* Parameter checkup. */
		if(GAIA::ALGO::gstremp(pszLog))
			return GAIA::False;
		if(type >= GAIA::LOG::Log::TYPE_MAXENUMCOUNT)
			return GAIA::False;

		/* If the file is not openned, open it. */
		if(m_pFile == GNIL)
		{
			m_pFile = gnew GAIA::FSYS::File;
			GAIA::CH szFileName[GAIA::MAXPL];
			if(m_strLogFilePathName.empty() || m_strLogFilePathName.size() >= GAIA::MAXPL)
			{
				GAIA::ALGO::gstrcpy(szFileName, g_gaia_appdocdir);
				GAIA::ALGO::gstrcat(szFileName, _T("last.log"));
			}
			else
				GAIA::ALGO::gstrcpy(szFileName, m_strLogFilePathName.fptr());
			if(!m_pFile->Open(szFileName, GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS | GAIA::FSYS::File::OPEN_TYPE_WRITE))
				return GAIA::False;
		}

		/* Generate the string. */
		GAIA::TCH szLogTime[64];
		logtime.to(szLogTime);
		GAIA::TCH szLogTimeAux[64];
		GAIA::TIME::timemkaux(szLogTime, szLogTimeAux);
		GAIA::TCH szFilter[32];
		GAIA::ALGO::hex2str(GRCAST(const GAIA::U8*)(&userfilter), sizeof(userfilter), szFilter);
		GAIA::TCH szIndex[32];
		GAIA::ALGO::int2str(m_sIndex, szIndex);

		/* Write log to file. */
		GAIA::CTN::BasicCharsString<GAIA::TCH, GAIA::NUM, 1024> strTemp;
		GAIA::CTN::BasicCharsString<GAIA::CH, GAIA::NUM, 1024> strTemp1;
		strTemp += szLogTimeAux + 2; // +2 means not need year's thousand place and hundred place.
		strTemp += " ";
		strTemp += logobj.GetLogTypeString(type);
		strTemp += " ";
		strTemp += szFilter;
		strTemp += " ";
		strTemp += szIndex;
		strTemp += " ";
		strTemp += pszLog;
		strTemp += " ";
		strTemp += logobj.GetLineBreak();
		strTemp1 = strTemp.toUtf8();
		if(strTemp1.empty())
		{
			strTemp1.reserve(strTemp.size() * 3 + 1);
			GAIA::NUM srclen = strTemp.size();
			const GAIA::TCH* p = strTemp.fptr();
			for(GAIA::NUM x = 0; x < srclen; ++x)
			{
				if(p[x] < 0 || p[x] >= 128)
				{
					strTemp1 += "$W";
					strTemp1 += (GAIA::N32)p[x];
				}
				else
				{
					GAIA::CH szTemp[2];
					szTemp[0] = p[x];
					szTemp[1] = '\0';
					strTemp1 += szTemp;
				}
			}
		}
		m_pFile->Write(strTemp1.fptr(), strTemp1.size());

		/* Increase index. */
		++m_sIndex;

		return GAIA::True;
	}
	virtual GAIA::BL FlushLog(GAIA::LOG::Log& logobj)
	{
		if(m_pFile == GNIL)
			return GAIA::False;
		m_pFile->Flush();
		return GAIA::True;
	}
private:
	GINL GAIA::GVOID init()
	{
		m_pFile = GNIL;
		m_sIndex = 0;
	}
private:
	GAIA::FSYS::File* m_pFile;
	GAIA::NUM m_sIndex;
	GAIA::CTN::AString m_strLogFilePathName;
};
extern DefaultGAIALogCallBack g_gaia_log_callback;

/* Global cache pool */
extern const GAIA::CH* gaia_alloc_staticstring(const GAIA::CH* psz);
extern const GAIA::WCH* gaia_alloc_staticwstring(const GAIA::WCH* psz);
extern const GAIA::U8* gaia_alloc_staticbuffer(const GAIA::GVOID* p, GAIA::NUM sSize);
extern const GAIA::CH* gaia_alloc_string(const GAIA::CH* psz);
extern GAIA::BL gaia_release_string(const GAIA::CH* psz);
extern const GAIA::WCH* gaia_alloc_wstring(const GAIA::WCH* psz);
extern GAIA::BL gaia_release_wstring(const GAIA::WCH* psz);
extern const GAIA::U8* gaia_alloc_buffer(const GAIA::GVOID* p, GAIA::NUM sSize);
extern GAIA::BL gaia_release_buffer(const GAIA::GVOID* p, GAIA::NUM sSize);

/* PerfCollector. */
extern GAIA::DBG::PerfCollector g_gaia_perf;

/* ObjWatcher. */
extern GAIA::DBG::ObjWatcher g_gaia_objwatcher;

/* Global variable management. */
extern GAIA::GVOID gaia_reset_global_variables();

#endif
