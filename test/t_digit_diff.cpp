#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class DiffEx : public GAIA::DIGIT::Diff
	{
	public:
		GINL DiffEx(){this->init();}
		GINL GAIA::GVOID SetData(GAIA::CTN::Vector<GAIA::NUM>* pData){m_pData = pData;}
	private:
		virtual GAIA::N64 request_source(GAIA::GVOID* p, const GAIA::N64& nOffset, const GAIA::N64& nSize)
		{
			GAST(p != GNIL);
			if(nOffset < 0 || nSize < 0 || nOffset > m_pData->datasize())
				return 0;
			GAIA::NUM sAvailSize = (GAIA::NUM)GAIA::ALGO::gmin(nSize, (GAIA::N64)(m_pData->datasize() - nOffset));
			GAIA::ALGO::gmemcpy(p, ((GAIA::U8*)m_pData->fptr()) + nOffset, sAvailSize);
			return sAvailSize;
		}
	private:
		GINL GAIA::GVOID init()
		{
			m_pData = GNIL;
		}
	private:
		GAIA::CTN::Vector<GAIA::NUM>* m_pData;
	};

	extern GAIA::GVOID t_digit_diff(GAIA::LOG::Log& logobj)
	{
		GAIA::CH szSrc[1024];
		GAIA::CH szDst[1024];
		GAIA::CH szDiff[1024];

		GAIA::ALGO::gstrcpy(szSrc, "Hello World! This is diff test source! Diff By XDelta3!Diff By XDelta3!Diff By XDelta3!");
		GAIA::ALGO::gstrcpy(szDst, "Hello Kitty! This is diff test destination! Diff By XDelta3!Diff By XDelta3!Diff By XDelta3!");

		GAIA::NUM sSrcLen = GAIA::ALGO::gstrlen(szSrc);
		GAIA::NUM sDstLen = GAIA::ALGO::gstrlen(szDst);

		GAIA::NUM sRes = (GAIA::NUM)GAIA::DIGIT::diff(
			szDiff, sizeof(szDiff),
			szSrc, sSrcLen + 1,
			szDst, sDstLen + 1,
			GAIA::DIGIT::DIFF_MODE_NOCOMPRESS);
		if(sRes == -1)
			TERROR;
		sRes = (GAIA::NUM)GAIA::DIGIT::undiff(
			szDst, sizeof(szDst),
			szSrc, GAIA::ALGO::gstrlen(szSrc) + 1,
			szDiff, sRes,
			GAIA::DIGIT::DIFF_MODE_DEFAULT);
		if(sRes == -1)
			TERROR;
		if(sRes != sDstLen + 1)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szDst, "Hello Kitty! This is diff test destination! Diff By XDelta3!Diff By XDelta3!Diff By XDelta3!") != 0)
			TERROR;

		GAIA::CTN::Vector<GAIA::NUM> listDataSrc;
		for(GAIA::NUM x = 0; x < 100000; ++x)
			listDataSrc.push_back(x);
		GAIA::CTN::Vector<GAIA::NUM> listDataDst = listDataSrc;
		for(GAIA::NUM x = listDataSrc.size() / 1000 - 1; x >= 0; --x)
			listDataDst.erase(x * 1000);

		DiffEx diff;
		diff.SetData(&listDataSrc);
		if(diff.isbegin())
			TERROR;
		TAST(diff.begin(GAIA::DIGIT::DIFF_MODE_DEFAULT));
		TAST(diff.empty());
		if(diff.read_size() != 0)
			TERROR;
		if(diff.write_size() != 0)
			TERROR;
		if(diff.remain() != 0)
			TERROR;
		if(diff.total() != 0)
			TERROR;
		TAST(diff.isbegin());
		if(diff.write((GAIA::GVOID*)listDataDst.fptr(), listDataDst.size() * sizeof(GAIA::NUM)) != listDataDst.size() * sizeof(GAIA::NUM))
			TERROR;
		TAST(diff.flush());
		if(diff.read_size() != 0)
			TERROR;
		if(diff.write_size() != listDataDst.size() * sizeof(GAIA::NUM))
			TERROR;
		if(diff.total() == 0)
			TERROR;
		if(diff.remain() != diff.total())
			TERROR;
		GAIA::NUM sDiffBufSize = (GAIA::NUM)diff.total();
		GAIA::U8* pDiffBuf = gnew GAIA::U8[sDiffBufSize];
		if(diff.read(pDiffBuf, sDiffBufSize) != sDiffBufSize)
			TERROR;
		TAST(diff.end());
		
		TAST(diff.begin(GAIA::DIGIT::DIFF_MODE_UNDIFF));
		if(diff.write(pDiffBuf, sDiffBufSize) != sDiffBufSize)
			TERROR;
		GAIA::NUM sDstBufSize = (GAIA::NUM)diff.total();
		GAIA::U8* pDstBuf = gnew GAIA::U8[sDstBufSize];
		if(diff.read(pDstBuf, sDstBufSize) != sDstBufSize)
			TERROR;
		TAST(diff.end());
		if(sDstBufSize != listDataDst.datasize())
			TERROR;
		if(GAIA::ALGO::gmemcmp(pDstBuf, listDataDst.fptr(), listDataDst.datasize()) != 0)
			TERROR;

		gdel[] pDiffBuf;
		pDiffBuf = GNIL;
		gdel[] pDstBuf;
		pDstBuf = GNIL;
	}
}
