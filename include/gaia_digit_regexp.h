#ifndef		__GAIA_DIGIT_REGEXP_H__
#define		__GAIA_DIGIT_REGEXP_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_arrayvector.h"

namespace GAIA
{
	namespace DIGIT
	{
		class RegExp : public GAIA::Base
		{
		public:
			class CompileOption : public GAIA::Base
			{
			public:
				GAIA::GVOID reset()
				{
					bIgnoreCase = GAIA::False;
					bMultiLine = GAIA::False;
					bNewLineByCR = GAIA::False;
					bNewLineByLF = GAIA::False;
					bNewLineByCRLF = GAIA::False;
				}
				GAIA::BL check() const
				{
					GAIA::N32 n = bNewLineByCR ? 1 : 0;
					n += (bNewLineByLF ? 1 : 0);
					n += (bNewLineByCRLF ? 1 : 0);
					if(n != 0 && n != 1 && n != 3)
						return GAIA::False;
					return GAIA::True;
				}
			public:
				GAIA::U8 bIgnoreCase : 1;
				GAIA::U8 bMultiLine : 1;
				GAIA::U8 bNewLineByCR : 1;
				GAIA::U8 bNewLineByLF : 1;
				GAIA::U8 bNewLineByCRLF : 1;
			};
			class ExecuteOption : public GAIA::Base
			{
			public:
				GAIA::GVOID reset()
				{
					bNotBeginOfLine = GAIA::False;
					bNotEndOfLine = GAIA::False;
					bNewLineByCR = GAIA::False;
					bNewLineByLF = GAIA::False;
					bNewLineByCRLF = GAIA::False;
				}
				GAIA::BL check() const
				{
					GAIA::N32 n = bNewLineByCR ? 1 : 0;
					n += (bNewLineByLF ? 1 : 0);
					n += (bNewLineByCRLF ? 1 : 0);
					if(n != 0 && n != 1 && n != 3)
						return GAIA::False;
					return GAIA::True;
				}
			public:
				GAIA::U8 bNotBeginOfLine : 1;
				GAIA::U8 bNotEndOfLine : 1;
				GAIA::U8 bNewLineByCR : 1;
				GAIA::U8 bNewLineByLF : 1;
				GAIA::U8 bNewLineByCRLF : 1;
			};
			class ErrorInfo : public GAIA::Base
			{
			public:
				GAIA::GVOID reset(){m_pszError = GNIL;}
				const GAIA::CH* m_pszError;
			};
			class ResultInfo : public GAIA::Base
			{
			public:
				class Range
				{
				public:
					GAIA::N32 nBegin;
					GAIA::N32 nEnd;
				};
			public:
				GAIA::GVOID reset(){offsets.resize(0);}
			public:
				GAIA::CTN::ArrayVector<Range, 1> offsets;
			};
		public:
			RegExp(){this->init();}
			~RegExp(){this->Destroy();}
			GAIA::BL Compile(const GAIA::GVOID* pFinder, const CompileOption& co, GAIA::NUM sCharSize = sizeof(GAIA::CH));
			GAIA::BL SetResultMaxLen(const GAIA::NUM& sMaxLen)
			{
				if(sMaxLen <= 0)
					return GAIA::False;
				m_ri.offsets.resize(sMaxLen);
				return GAIA::True;
			}
			GAIA::BL Execute(const GAIA::GVOID* pFindee, const GAIA::NUM& sFindeeLen, const GAIA::NUM& sFindeeOffset, const ExecuteOption& eo);
			GAIA::GVOID Destroy();
			const ErrorInfo& GetLastErrorInfo() const{return m_ei;}
			const ResultInfo& GetResultInfo() const{return m_ri;}
		private:
			GAIA::GVOID init()
			{
				m_pContext = GNIL;
				m_sCharSize = sizeof(GAIA::CH);
				m_ei.reset();
				m_ri.offsets.resize(1);
			}
			RegExp(const RegExp& src){this->operator = (src);}
			RegExp& operator = (const RegExp& src)
			{
				m_pContext = src.m_pContext;
				m_ei = src.m_ei;
				m_ri = src.m_ri;
				return *this;
			}
		private:
			GAIA::GVOID* m_pContext;
			GAIA::NUM m_sCharSize;
			ErrorInfo m_ei;
			ResultInfo m_ri;
		};
	}
}

#endif
