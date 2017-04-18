#ifndef		__T_COMMON_H__
#define		__T_COMMON_H__

extern GAIA::SYNC::Atomic g_error_cnt;

#define TTEXT(sz) \
	do\
	{\
		logobj << logobj.Type(GAIA::LOG::Log::TYPE_LOG);\
		logobj << sz;\
		logobj << logobj.End();\
	}\
	while(0)

#define TITEM(sz) \
	do\
	{\
		if(!bOutputTime)\
			uTimeBegin = GAIA::TIME::tick_time();\
		else\
		{\
			uTimeEnd = GAIA::TIME::tick_time();\
			logobj << logobj.Type(GAIA::LOG::Log::TYPE_LOG);\
			logobj << "\t\tTimelost ";\
			logobj << GSCAST(GAIA::F64)(uTimeEnd - uTimeBegin) / 1000.0;\
			logobj << "(MS)";\
			logobj << logobj.End();\
		}\
		bOutputTime = !bOutputTime;\
		logobj << "\t" << (sz) << logobj.End();\
	}\
	while(0)

#define TERROR \
	do\
	{\
		logobj << logobj.Type(GAIA::LOG::Log::TYPE_ERROR);\
		logobj << "\t\tTErr(";\
		logobj << (GAIA::N64)g_error_cnt;\
		logobj << ") at : ";\
		logobj << __FILE__;\
		logobj << "(";\
		logobj << __LINE__;\
		logobj << ")";\
		logobj << "\n";\
		logobj << logobj.End();\
		g_error_cnt++;\
	}\
	while(0)

#define TLOG(t) \
	do\
	{\
		logobj << logobj.Type(GAIA::LOG::Log::TYPE_LOG);\
		logobj << "\t\t";\
		logobj << (t);\
		logobj << "\n";\
		logobj << logobj.End();\
	}\
	while(0)

#define TAST(e) \
	do\
	{\
		if(!(e))\
			TERROR;\
	}\
	while(0)

#define TMODULE_LOG_ENABLED GAIA::True

#endif
