#ifndef		__T_COMMON_H__
#define		__T_COMMON_H__

#define TTEXT(sz) do{logobj << logobj.Type(GAIA::LOG::Log::TYPE_LOG) << sz << logobj.End();}while(0)
#define TITEM(sz) do{if(!bOutputTime)\
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
						logobj << "\t" << (sz) << logobj.End(); \
					}while(0)
#define TERROR do{logobj << logobj.Type(GAIA::LOG::Log::TYPE_ERROR) << "\t\tTErr at : " << __FILE__ << "(" << __LINE__ << ")" << "\n" << logobj.End();}while(0)
#define TLOG(t) do{logobj << logobj.Type(GAIA::LOG::Log::TYPE_LOG) << "\t\t" << (t) << "\n" << logobj.End();}while(0)
#define TAST(e) do{if(!(e)) TERROR;}while(0)

#define TMODULE_LOG_ENABLED GAIA::True

#endif
