#ifndef		__GAIA_STREAM_STREAMFORMAT_H__
#define		__GAIA_STREAM_STREAMFORMAT_H__

#include "gaia_type.h"

namespace GAIA
{
	namespace STREAM
	{
		class StreamFormat : public GAIA::Base
		{
		public:
			GINL StreamFormat(){this->reset();}
			GINL GAIA::GVOID radix(GAIA::U8 uRadix){m_uRadix = uRadix;}
			GINL GAIA::U8 radix() const{return m_uRadix;}
			GINL GAIA::GVOID precision(GAIA::N8 nPrecision){m_nPrecision = nPrecision;}
			GINL GAIA::N8 precision() const{return m_nPrecision;}
			GINL GAIA::GVOID align(GAIA::U8 uAlign){m_uAlignCount = uAlign;}
			GINL GAIA::U8 align() const{return m_uAlignCount;}
			GINL GAIA::GVOID showpoint(GAIA::BL bEnable){m_bShowPoint = bEnable;}
			GINL GAIA::BL showpoint() const{return m_bShowPoint;}
			GINL GAIA::GVOID showbase(GAIA::BL bEnable){m_bShowBase = bEnable;}
			GINL GAIA::BL showbase() const{return m_bShowBase;}
			GINL GAIA::GVOID showsign(GAIA::BL bEnable){m_bShowSign = bEnable;}
			GINL GAIA::BL showsign() const{return m_bShowSign;}
			GINL GAIA::GVOID scientific(GAIA::BL bEnable){m_bScientific = bEnable;}
			GINL GAIA::BL scientific() const{return m_bScientific;}
			GINL GAIA::GVOID left(GAIA::BL bEnable){m_bLeft = bEnable;}
			GINL GAIA::BL left() const{return m_bLeft;}
			GINL GAIA::GVOID uppercase(GAIA::BL bEnable){m_bEnableUpperCase = bEnable;}
			GINL GAIA::BL uppercase() const{return m_bEnableUpperCase;}
			GINL StreamFormat& operator = (const StreamFormat& src)
			{
				m_uRadix = src.m_uRadix;
				m_nPrecision = src.m_nPrecision;
				m_uAlignCount = src.m_uAlignCount;
				m_bShowPoint = src.m_bShowPoint;
				m_bShowBase = src.m_bShowBase;
				m_bShowSign = src.m_bShowSign;
				m_bScientific = src.m_bScientific;
				m_bLeft = src.m_bLeft;
				m_bEnableUpperCase = src.m_bEnableUpperCase;
				return *this;
			}
		public:
			GINL GAIA::GVOID reset()
			{
				m_uRadix = 10;
				m_nPrecision = GINVALID; // -1 means %g, 0 means %f, >0 means %0nf.
				m_uAlignCount = 0;
				m_bShowPoint = GAIA::False;
				m_bShowBase = GAIA::False;
				m_bShowSign = GAIA::False;
				m_bScientific = GAIA::False;
				m_bLeft = GAIA::False;
				m_bEnableUpperCase = GAIA::False;
			}
		private:
			GAIA::U8 m_uRadix;
			GAIA::N8 m_nPrecision;
			GAIA::U8 m_uAlignCount;
			GAIA::U8 m_bShowPoint : 1;
			GAIA::U8 m_bShowBase : 1;
			GAIA::U8 m_bShowSign : 1;
			GAIA::U8 m_bScientific : 1;
			GAIA::U8 m_bLeft : 1;
			GAIA::U8 m_bEnableUpperCase : 1;
		};
	}
}

#endif
