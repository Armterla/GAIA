#ifndef		__GAIA_EXCEPTION_H__
#define		__GAIA_EXCEPTION_H__

#include "gaia_type.h"

namespace GAIA
{
	namespace ECT
	{
		GAIA_ENUM_BEGIN(GERROR)
			// Base Error
			GERROR_NOERROR,
			GERROR_UNKNOWN,
			// Network error, see POSIX standard declaration
			GERROR_NETDOWN,
			GERROR_NETRESET,
			GERROR_ACCES,
			GERROR_AFNOSUPPORT,
			GERROR_MFILE,
			GERROR_NOBUFS,
			GERROR_PROTONOSUPPORT,
			GERROR_PROTOTYPE,
			GERROR_ADDRINUSE,
			GERROR_ADDRNOTAVAIL,
			GERROR_BADF,
			GERROR_DESTADDRREQ,
			GERROR_FAULT,
			GERROR_INVAL,
			GERROR_NOTSOCK,
			GERROR_OPNOTSUPP,
			GERROR_LOOP,
			GERROR_NAMETOOLONG,
			GERROR_CONNABORTED,
			GERROR_INTR,
			GERROR_WOULDBLOCK,
			GERROR_ALREADY,
			GERROR_CONNREFUSED,
			GERROR_HOSTUNREACH,
			GERROR_INPROGRESS,
			GERROR_ISCONN,
			GERROR_NETUNREACH,
			GERROR_TIMEDOUT,
			GERROR_CONNRESET,
			GERROR_NOPROTOOPT,
			GERROR_MSGSIZE,
			GERROR_NOTCONN,
		GAIA_ENUM_END(GERROR)
		static const GAIA::CH* GERROR_TEXTA[] =
		{
			"NoError",
			"Unknown",
			"NetDown",
			"NetReset",
			"Acces",
			"AFNoSupport",
			"MFile",
			"NoBufs",
			"ProtoNoSupport",
			"Prototype",
			"AddrInUse",
			"AddrNotAvail",
			"BadF",
			"DestAddrReq",
			"Fault",
			"InVal",
			"NotSock",
			"OpNotSupp",
			"Loop",
			"NameTooLong",
			"ConnAborted",
			"IntR",
			"WouldBlock",
			"Already",
			"ConnRefused",
			"HostUnreach",
			"InProgress",
			"IsConn",
			"NetUnreach",
			"TimedOut",
			"ConnReset",
			"NoProtoOpt",
			"MsgSize",
			"NotConn",
		};
		static const GAIA::WCH* GERROR_TEXTW[] =
		{
			L"NoError",
			L"Unknown",
			L"NetDown",
			L"NetReset",
			L"Acces",
			L"AFNoSupport",
			L"MFile",
			L"NoBufs",
			L"ProtoNoSupport",
			L"Prototype",
			L"AddrInUse",
			L"AddrNotAvail",
			L"BadF",
			L"DestAddrReq",
			L"Fault",
			L"InVal",
			L"NotSock",
			L"OpNotSupp",
			L"Loop",
			L"NameTooLong",
			L"ConnAborted",
			L"IntR",
			L"WouldBlock",
			L"Already",
			L"ConnRefused",
			L"HostUnreach",
			L"InProgress",
			L"IsConn",
			L"NetUnreach",
			L"TimedOut",
			L"ConnReset",
			L"NoProtoOpt",
			L"MsgSize",
			L"NotConn",
		};

		GINL const GAIA::CH* GetErrorTextA(GERROR err)
		{
			if(err >= sizeofarray(GAIA::ECT::GERROR_TEXTA))
				return GAIA::ECT::GERROR_TEXTA[GAIA::ECT::GERROR_UNKNOWN];
			return GAIA::ECT::GERROR_TEXTA[err];
		}

		GINL const GAIA::WCH* GetErrorTextW(GERROR err)
		{
			if(err >= sizeofarray(GAIA::ECT::GERROR_TEXTW))
				return GAIA::ECT::GERROR_TEXTW[GAIA::ECT::GERROR_UNKNOWN];
			return GAIA::ECT::GERROR_TEXTW[err];
		}

		GINL const GAIA::TCH* GetErrorText(GERROR err)
		{
		#if GAIA_CHARSET == GAIA_CHARSET_UNICODE
			return GAIA::ECT::GetErrorTextW(err);
		#elif GAIA_CHARSET == GAIA_CHARSET_ANSI
			return GAIA::ECT::GetErrorTextA(err);
		#endif
		}

		/*!
			Exception base class.
		*/
		class Ect : public GAIA::Base
		{
		public:
			GINL Ect(const GAIA::CH* pszFile, GAIA::N32 nCodeLine, const GAIA::CH* pszMsg)
			{
				this->init();
				m_pszFile = pszFile;
				m_nCodeLine = nCodeLine;
				m_pszMsg = pszMsg;
			}
			GINL Ect(const GAIA::CH* pszFile, GAIA::N32 nCodeLine, const GAIA::CH* pszMsg, GAIA::N32 nError, GAIA::N32 nOSError)
			{
				this->init();
				m_pszFile = pszFile;
				m_nCodeLine = nCodeLine;
				m_pszMsg = pszMsg;
				m_nError = nError;
				m_nOSError = nOSError;
			}
			GINL const GAIA::CH* GetFileName() const{return m_pszFile;}
			GINL GAIA::N32 GetCodeLine() const{return m_nCodeLine;}
			GINL const GAIA::CH* GetMessage() const{return m_pszMsg;}
			GINL GAIA::N32 GetError() const{return m_nError;}
			GINL const GAIA::CH* GetErrorTextA() const{return GAIA::ECT::GetErrorTextA((GAIA::ECT::GERROR)m_nError);}
			GINL const GAIA::WCH* GetErrorTextW() const{return GAIA::ECT::GetErrorTextW((GAIA::ECT::GERROR)m_nError);}
			GINL const GAIA::TCH* GetErrorText() const{return GAIA::ECT::GetErrorText((GAIA::ECT::GERROR)m_nError);}
			GINL GAIA::N32 GetOSError() const{return m_nOSError;}
			GINL GAIA::GVOID SetDispatched(GAIA::BL bDispatched){m_bDispatched = bDispatched;}
			GINL GAIA::BL GetDispatched() const{return m_bDispatched;}
		private:
			GINL GAIA::GVOID init()
			{
				m_pszFile = GNIL;
				m_nCodeLine = GINVALID;
				m_nError = 0;
				m_nOSError = 0;
				m_pszMsg = GNIL;
				m_bDispatched = GAIA::False;
			}
		private:
			const GAIA::CH* m_pszFile;
			GAIA::N32 m_nCodeLine;
			GAIA::N32 m_nError;
			GAIA::N32 m_nOSError;
			const GAIA::CH* m_pszMsg;
			GAIA::BL m_bDispatched;
		};

		/*!
			When some not support function be try, the exception will occurred.
		*/
		GAIA_EXCEPTION(NotSupport);

		/*!
			When unknown exception raised, the exception will occurred.
		*/
		GAIA_EXCEPTION(Unknown);

		/*!
			When the GAST failed, the exception will occurred.
		*/
		GAIA_EXCEPTION(Ast);

		/*!
			When the GASTDEBUG failed, the exception will occurred.
		*/
		GAIA_EXCEPTION_FROM(AstDebug, Ast);

		/*!
			When caller input a invalid param, the exception will occurred.
		*/
		GAIA_EXCEPTION(InvalidParam);

		/*!
			When caller cause a illegal operation, the exception will occurred.
		*/
		GAIA_EXCEPTION(Illegal);

		/*!
			When caller cause a null pointer access, the exception will occurred.
		*/
		GAIA_EXCEPTION(NullPointer);

		/*!
			When access overflow memory, the exception will occurred.
		*/
		GAIA_EXCEPTION(MemOverflow);

		/*!
			When leak memory, the exception will occurred.
		*/
		GAIA_EXCEPTION(MemLeak);

		/*!
			When access overflow index, the exception will occurred.
		*/
		GAIA_EXCEPTION(IndexOverflow);

		/*!
			When the memory is not enough, the exception will occurred.
		*/
		GAIA_EXCEPTION(MemNotEnough);

		/*!
			When the buffer is not enough, the exception will occurred.
		*/
		GAIA_EXCEPTION(BufNotEnough);

		/*!
			When IO read write seek tell(and etc) can't work corrected, the exception will occurred.
		*/
		GAIA_EXCEPTION(IO);

		/*!
			When read write exceed content, the exception will occurred.
		*/
		GAIA_EXCEPTION(ExceedContent);

		/*!
			When the constructor execute with some error, the exception will occurred.
		*/
		GAIA_EXCEPTION(Constructor);

		/*!
			When the destructor execute with some error, the exception will occurred.
		*/
		GAIA_EXCEPTION(Destructor);

		/*!
			When the copy constructor execute with some error, the exception will occurred.
		*/
		GAIA_EXCEPTION(CopyConstructor);

		/*!
			When the c++ operator execute with some error, the exception will occurred.
		*/
		GAIA_EXCEPTION(Operator);

		/*!
			When the convert from a to b failed, the exception will occurred.
		*/
		GAIA_EXCEPTION(Convert);

		/*!
			When div by zero, the exception will occurred.
		*/
		GAIA_EXCEPTION(DivZero);

		/*!
			When network error, this exception will occurred.
		*/
		GAIA_EXCEPTION(Network);

		/*!
			When data error, this exception will occurred.
		*/
		GAIA_EXCEPTION(DataError);
	}
}

#ifndef GAIA_DEBUG_CATCHEXCEPTION
extern GAIA::ECT::Ect e;
#endif

#endif
