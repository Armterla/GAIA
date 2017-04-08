#define PCRE_STATIC
#include <pcre.h>
#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_algo_string.h>
#include <gaia_digit_regexp.h>
#include <gaia_type_impl.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace DIGIT
	{
		GINL GAIA::GVOID* pcre_alloc_func(size_t size)
		{
			return gnew GAIA::U8[size];
		}
		GINL GAIA::GVOID pcre_free_func(GAIA::GVOID* address)
		{
			gdel[](GAIA::U8*)address;
		}

		GAIA::BL RegExp::Compile(const GAIA::GVOID* pFinder, const CompileOption& co, GAIA::NUM sCharSize)
		{
			pcre_malloc = pcre_stack_malloc = pcre_alloc_func;
			pcre_free = pcre_stack_free = pcre_free_func;
			pcre16_malloc = pcre16_stack_malloc = pcre_alloc_func;
			pcre16_free = pcre16_stack_free = pcre_free_func;
			pcre32_malloc = pcre32_stack_malloc = pcre_alloc_func;
			pcre32_free = pcre32_stack_free = pcre_free_func;

			if(sCharSize == 1)
			{
				if(GAIA::ALGO::gstremp((const GAIA::N8*)pFinder))
					return GAIA::False;
			}
			else if(sCharSize == 2)
			{
				if(GAIA::ALGO::gstremp((const GAIA::N16*)pFinder))
					return GAIA::False;
			}
			else if(sCharSize == 4)
			{
				if(GAIA::ALGO::gstremp((const GAIA::N32*)pFinder))
					return GAIA::False;
			}
			else
			{
				GASTFALSE;
				return GAIA::False;
			}

			if(!co.check())
				return GAIA::False;

			this->Destroy();

			GAIA::N32 nOption = 0;
			if(co.bIgnoreCase)
				nOption |= PCRE_CASELESS;
			if(co.bMultiLine)
				nOption |= PCRE_MULTILINE;
			if(co.bNewLineByCR && co.bNewLineByLF && co.bNewLineByCRLF)
				nOption |= PCRE_NEWLINE_ANYCRLF;
			else if(co.bNewLineByCR)
				nOption |= PCRE_NEWLINE_CR;
			else if(co.bNewLineByLF)
				nOption |= PCRE_NEWLINE_LF;
			else if(co.bNewLineByCRLF)
				nOption |= PCRE_NEWLINE_CRLF;

			GAIA::N32 nErrorOffset;
			if(sCharSize == 1)
				m_pContext = pcre_compile((const GAIA::CH*)pFinder, nOption, &m_ei.m_pszError, &nErrorOffset, GNIL);
			else if(sCharSize == 2)
				m_pContext = pcre16_compile((const GAIA::U16*)pFinder, nOption, &m_ei.m_pszError, &nErrorOffset, GNIL);
			else if(sCharSize == 4)
				m_pContext = pcre32_compile((const GAIA::U32*)pFinder, nOption, &m_ei.m_pszError, &nErrorOffset, GNIL);
			else
			{
				GASTFALSE;
				return GAIA::False;
			}
			if(m_pContext == GNIL)
				return GAIA::False;
			m_sCharSize = sCharSize;
			return GAIA::True;
		}
		GAIA::BL RegExp::Execute(const GAIA::GVOID* pFindee, const GAIA::NUM& sFindeeLen, const GAIA::NUM& sFindeeOffset, const ExecuteOption& eo)
		{
			if(m_sCharSize == 1)
			{
				if(GAIA::ALGO::gstremp((const GAIA::N8*)pFindee))
				{
					m_ri.reset();
					return GAIA::False;
				}
			}
			else if(m_sCharSize == 2)
			{
				if(GAIA::ALGO::gstremp((const GAIA::N16*)pFindee))
				{
					m_ri.reset();
					return GAIA::False;
				}
			}
			else if(m_sCharSize == 4)
			{
				if(GAIA::ALGO::gstremp((const GAIA::N32*)pFindee))
				{
					m_ri.reset();
					return GAIA::False;
				}
			}
			else
			{
				GASTFALSE;
				m_ri.reset();
				return GAIA::False;
			}
			if(sFindeeLen == 0)
			{
				m_ri.reset();
				return GAIA::False;
			}
			if(!eo.check())
			{
				m_ri.reset();
				return GAIA::False;
			}

			GAIA::NUM sFindeeLenPrac;
			if(sFindeeLen < 0)
			{
				if(m_sCharSize == 1)
					sFindeeLenPrac = GAIA::ALGO::gstrlen((const GAIA::N8*)pFindee);
				else if(m_sCharSize == 2)
					sFindeeLenPrac = GAIA::ALGO::gstrlen((const GAIA::N16*)pFindee);
				else if(m_sCharSize == 4)
					sFindeeLenPrac = GAIA::ALGO::gstrlen((const GAIA::N32*)pFindee);
				else
				{
					GASTFALSE;
					sFindeeLenPrac = 0;
				}
			}
			else
				sFindeeLenPrac = sFindeeLen;

			GAIA::N32 nOption = 0;
			if(eo.bNotBeginOfLine)
				nOption |= PCRE_NOTBOL;
			if(eo.bNotEndOfLine)
				nOption |= PCRE_NOTEOL;
			if(eo.bNewLineByCR && eo.bNewLineByLF && eo.bNewLineByCRLF)
				nOption |= PCRE_NEWLINE_ANYCRLF;
			else if(eo.bNewLineByCR)
				nOption |= PCRE_NEWLINE_CR;
			else if(eo.bNewLineByLF)
				nOption |= PCRE_NEWLINE_LF;
			else if(eo.bNewLineByCRLF)
				nOption |= PCRE_NEWLINE_CRLF;

			m_ri.offsets.resize(m_ri.offsets.capacity());
			GAIA::N32 nFinded = -1;
			if(m_sCharSize == 1)
			{
				nFinded = pcre_exec(
					(pcre*)m_pContext, 
					GNIL, 
					(const GAIA::CH*)pFindee, sFindeeLenPrac, sFindeeOffset, 
					nOption, (GAIA::N32*)m_ri.offsets.fptr(), m_ri.offsets.size() * 2);
			}
			else if(m_sCharSize == 2)
			{
				nFinded = pcre16_exec(
					(pcre16*)m_pContext,
					GNIL,
					(const GAIA::U16*)pFindee, sFindeeLenPrac, sFindeeOffset,
					nOption, (GAIA::N32*)m_ri.offsets.fptr(), m_ri.offsets.size() * 2);
			}
			else if(m_sCharSize == 4)
			{
				nFinded = pcre32_exec(
					(pcre32*)m_pContext,
					GNIL,
					(const GAIA::U32*)pFindee, sFindeeLenPrac, sFindeeOffset,
					nOption, (GAIA::N32*)m_ri.offsets.fptr(), m_ri.offsets.size() * 2);
			}
			else
			{
				GASTFALSE;
				m_ri.reset();
				return GAIA::False;
			}
			if(nFinded <= 0)
			{
				m_ri.reset();
				return GAIA::False;
			}
			m_ri.offsets.resize(nFinded);

			return GAIA::True;
		}
		GAIA::GVOID RegExp::Destroy()
		{
			if(m_pContext == GNIL)
				return;
			if(m_sCharSize == 1)
				pcre_free(m_pContext);
			else if(m_sCharSize == 2)
				pcre16_free(m_pContext);
			else if(m_sCharSize == 4)
				pcre32_free(m_pContext);
			else
				GASTFALSE;
			m_pContext = GNIL;
			m_sCharSize = sizeof(GAIA::CH);
			m_ei.reset();
			m_ri.reset();
		}
	}
}
