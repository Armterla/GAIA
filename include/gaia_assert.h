#ifndef		__GAIA_ASSERT_H__
#define		__GAIA_ASSERT_H__

#include "gaia_type.h"
#include "gaia_exception.h"
#include "gaia_stream_format.h"
#include "gaia_stream_streambase.h"
#include "gaia_stream_stdstream.h"

extern GAIA::BL g_gaia_assertconfirm;

namespace GAIA
{
	namespace DBG
	{
		GINL GAIA::STREAM::StreamBase& debugstream();
		GINL GAIA::GVOID debuglog(const GAIA::CH* pszHead, const GAIA::CH* pszFileName, GAIA::NUM sCodeLine);
		
#ifdef GAIA_DEBUG_AST
#	if GAIA_APPMODE == GAIA_APPMODE_CONSOLE
#		define GAST(e)	do\
						{\
							if(!(e))\
							{\
								GAIA::CH szTempDebugLogHead[1024] = "GAST Failed:";\
								const GAIA::CH* pszE = #e;\
								GAIA::CH* pszTempDebugLogHeadDst = szTempDebugLogHead + sizeof("GAST Failed:") - 1;\
								while(*pszE != '\0')\
									*pszTempDebugLogHeadDst++ = *pszE++;\
								*pszTempDebugLogHeadDst = ' ';\
								*pszTempDebugLogHeadDst++ = '\0';\
								GAIA::DBG::debuglog(szTempDebugLogHead, __FILE__, __LINE__);\
								if(g_gaia_assertconfirm)\
								{\
									GAIA::STREAM::StreamBase& stm = GAIA::DBG::debugstream();\
									stm.lock_write();\
									stm << "GAST Failed:\n\t" << #e << "\n\t" << __FILE__ << "(" << __LINE__ << ")\n\tInput 1 to break and other to continue:";\
									stm.unlock_write();\
									GAIA::CH ch[32];\
									stm >> ch;\
									if(ch[0] == '1' && ch[1] == '\0')\
										GTHROW(Ast);\
								}\
								else\
								{\
									GAIA::STREAM::StreamBase& stm = GAIA::DBG::debugstream();\
									stm.lock_write();\
									stm << "GAST Failed:\n\t" << #e << "\n\t" << __FILE__ << "(" << __LINE__ << ")\n";\
									stm.unlock_write();\
									GTHROW(Ast);\
								}\
							}\
						}while(0)
#	elif GAIA_APPMODE == GAIA_APPMODE_GUI
#		define GAST(e)	do\
						{\
							if(!(e))\
							{\
								GAIA::DBG::debuglog("GAST Failed:", __FILE__, __LINE__);\
								GTHROW(Ast);\
							}\
						}while(0)
#	endif
#else
#	define GAST(e) do{}while(0)
#endif
#define GASTFALSE GAST(GAIA::ALWAYSFALSE)

#ifdef GAIA_DEBUG_ASTDEBUG
#	if GAIA_APPMODE == GAIA_APPMODE_CONSOLE
#		define GASTDBG(e)	do\
							{\
								if(!(e))\
								{\
									GAIA::CH szTempDebugLogHead[1024] = "GAST Failed:";\
									const GAIA::CH* pszE = #e;\
									GAIA::CH* pszTempDebugLogHeadDst = szTempDebugLogHead + sizeof("GAST Failed:") - 1;\
									while(*pszE != '\0')\
										*pszTempDebugLogHeadDst++ = *pszE++;\
									*pszTempDebugLogHeadDst = ' ';\
									*pszTempDebugLogHeadDst = '\0';\
									GAIA::DBG::debuglog(szTempDebugLogHead, __FILE__, __LINE__);\
									if(g_gaia_assertconfirm)\
									{\
										GAIA::STREAM::StreamBase& stm = GAIA::DBG::debugstream();\
										stm.lock_write();\
										stm << "GASTDBG Failed:\n\t" << #e << "\n\t"  << __FILE__ << "(" << __LINE__ << ")\n\tInput 1 to break and other to continue(Debug):";\
										stm.unlock_write();\
										GAIA::CH ch;\
										stm >> ch;\
										if(ch == '1')\
											GTHROW(AstDebug);\
									}\
									else\
									{\
										GAIA::STREAM::StreamBase& stm = GAIA::DBG::debugstream();\
										stm.lock_write();\
										stm << "GAST Failed:\n\t" << #e << "\n\t" << __FILE__ << "(" << __LINE__ << ")\n";\
										stm.unlock_write();\
										GTHROW(AstDebug);\
									}\
								}\
							}while(0)
#	elif GAIA_APPMODE == GAIA_APPMODE_GUI
#		define GASTDBG(e)	do\
							{\
								if(!(e))\
								{\
									GAIA::CH szTempDebugLogHead[1024] = "GASTDBG Failed:";\
									const GAIA::CH* pszE = #e;\
									GAIA::CH* pszTempDebugLogHeadDst = szTempDebugLogHead + sizeof("GAST Failed:") - 1;\
									while(*pszE != '\0')\
										*pszTempDebugLogHeadDst++ = *pszE++;\
									*pszTempDebugLogHeadDst = '\0';\
									GAIA::DBG::debuglog(szTempDebugLogHead, __FILE__, __LINE__);\
									GTHROW(AstDebug);\
								}\
							}while(0)
#	endif
#else
#	define GASTDBG(e) do{}while(0)
#endif

#ifdef GAIA_DEBUG_STATICAST
#	define GSTATICAST(e) do{typedef class GAIA_STATIC_ASSERT_STRUCT GAIA_STATIC_ASSERT_FAILED[(e) ? 1 : -1];}while(GAIA::ALWAYSFALSE)
#endif
	}
}
#endif
