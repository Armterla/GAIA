#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_algo_string.h>
#include <gaia_stream_format.h>
#include <gaia_stream_stdstream.h>
#include <gaia_sync_lock.h>
#include <gaia_sync_autolock.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

#include <stdlib.h>
#include <iostream>
#include <iomanip>

namespace GAIA
{
	namespace STREAM
	{
		GAIA::SYNC::Lock s_lrSTDStream;
		STDStream& STDStream::operator << (GAIA::BL t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::NM t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::UM t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::N8 t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << (GAIA::N32)t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::N16 t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << (GAIA::N32)t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::N32 t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (const GAIA::N64& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::U8 t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << (GAIA::U32)t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::U16 t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << (GAIA::U32)t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::U32 t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (const GAIA::U64& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (GAIA::F32 t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (const GAIA::F64& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << t;
			return *this;
		}
		STDStream& STDStream::operator << (const GAIA::WCH& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			std::cout << (GAIA::N64)t;
			return *this;
		}
		STDStream& STDStream::operator << (const GAIA::CH* p)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(p == GNIL)
				return *this;
			if(!this->enable_write())
				return *this;
			std::cout << p;
			return *this;
		}
		STDStream& STDStream::operator << (const GAIA::WCH* p)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(p == GNIL)
				return *this;
			if(!this->enable_write())
				return *this;
			std::wcout << p;
			return *this;
		}
		STDStream& STDStream::operator << (const GAIA::X128& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return *this;
			return *this;
		}
	#define BEGIN_PREPARE_READ if(m_pszPrepareReadCursor != GNIL)\
		{\
			GAIA::NUM sEnd;\
			for(sEnd = 0; m_pszPrepareReadCursor[sEnd] != '\0'; ++sEnd)\
			{\
				GAIA::WCH ch = m_pszPrepareReadCursor[sEnd];\
				if(ch == '\r' || ch == '\n' || ch == '\t' || ch == ' ')\
					break;\
			}\
			if(sEnd != 0)\
			{
				
	#define END_PREPARE_READ }\
			if(m_pszPrepareReadCursor[sEnd] == '\0')\
			{\
				gdel[] m_pszPrepareRead;\
				m_pszPrepareRead = m_pszPrepareReadCursor = GNIL;\
			}\
			else\
				m_pszPrepareReadCursor = m_pszPrepareReadCursor + sEnd + 1;\
			return *this;\
		}
	#define CIN_RESET \
		if(!std::cin)\
			std::cin.clear();
	#define WCIN_RESET \
		if(!std::wcin)\
			std::wcin.clear();

		STDStream& STDStream::operator >> (GAIA::BL& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			if(sEnd == 1 && m_pszPrepareReadCursor[0] == '1')
				t = GAIA::True;
			else
				t = GAIA::False;
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::NM& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::UM& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::N8& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			GAIA::N32 temp;
			std::cin >> temp;
			CIN_RESET
			t = GSCAST(GAIA::N8)(temp);
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::N16& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			GAIA::N32 temp;
			std::cin >> temp;
			CIN_RESET
			t = GSCAST(GAIA::N16)(temp);
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::N32& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::N64& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::U8& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			GAIA::U32 temp;
			std::cin >> temp;
			CIN_RESET
			t = GSCAST(GAIA::U8)(temp);
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::U16& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			GAIA::U32 temp;
			std::cin >> temp;
			CIN_RESET
			t = GSCAST(GAIA::U16)(temp);
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::U32& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::U64& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::F32& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::F64& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			std::cin >> t;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::WCH& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t = (GAIA::NUM)GAIA::ALGO::acasts(m_pszPrepareReadCursor);
			END_PREPARE_READ
			GAIA::N64 temp;
			std::cin >> temp;
			CIN_RESET
			t = GSCAST(GAIA::WCH)(temp);
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::CH* p)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(p == GNIL)
				return *this;
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			GAIA::ALGO::gstrcpy(p, m_pszPrepareReadCursor, sEnd);
			END_PREPARE_READ
			std::cin >> p;
			CIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::WCH* p)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(p == GNIL)
				return *this;
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			GAIA::ALGO::gstrcpy(p, m_pszPrepareReadCursor, sEnd);
			END_PREPARE_READ
			std::wcin >> p;
			WCIN_RESET
			return *this;
		}
		STDStream& STDStream::operator >> (GAIA::X128& t)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_read())
				return *this;
			BEGIN_PREPARE_READ
			t.fromstring(m_pszPrepareReadCursor);
			END_PREPARE_READ
			GAIA::CH sz[128];
			std::cin >> sz;
			CIN_RESET
			t = sz;
			return *this;
		}
		STDStream& STDStream::operator >> (StreamFormat& pf)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			pf = m_pf;
			return *this;
		}
		STDStream& STDStream::operator << (const StreamFormat& pf)
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(pf.align() != m_pf.align() || pf.align() != 0)
			{
				std::cout << std::setw(pf.align());
				std::wcout << std::setw(pf.align());
				m_pf.align(pf.align());
			}
			if(pf.precision() != m_pf.precision())
			{
				std::cout << std::setprecision(pf.precision());
				std::wcout << std::setprecision(pf.precision());
				m_pf.precision(pf.precision());
			}
			if(pf.radix() != m_pf.radix())
			{
				std::cout << std::setbase(pf.radix());
				std::wcout << std::setbase(pf.radix());
				m_pf.radix(pf.radix());
			}
			if(pf.showpoint() != m_pf.showpoint())
			{
				if(pf.showpoint())
				{
					std::cout << std::showpoint;
					std::wcout << std::showpoint;
				}
				else
				{
					std::cout << std::noshowpoint;
					std::wcout << std::noshowpoint;
				}
				m_pf.showpoint(pf.showpoint());
			}
			if(pf.showbase() != m_pf.showbase())
			{
				if(pf.showbase())
				{
					std::cout << std::showbase;
					std::wcout << std::showbase;
				}
				else
				{
					std::cout << std::noshowbase;
					std::wcout << std::noshowbase;
				}
				m_pf.showbase(pf.showbase());
			}
			if(pf.showsign() != m_pf.showsign())
			{
				if(pf.showsign())
				{
					std::cout << std::showpos;
					std::wcout << std::showpos;
				}
				else
				{
					std::cout << std::noshowpos;
					std::wcout << std::noshowpos;
				}
				m_pf.showsign(pf.showsign());
			}
			if(pf.scientific() != m_pf.scientific())
			{
				if(pf.scientific())
				{
					std::cout << std::scientific;
					std::wcout << std::scientific;
				}
				else
				{
					std::cout << std::fixed;
					std::wcout << std::fixed;
				}
				m_pf.scientific(pf.scientific());
			}
			if(pf.left() != m_pf.left())
			{
				if(pf.left())
				{
					std::cout << std::left;
					std::wcout << std::left;
				}
				else
				{
					std::cout << std::right;
					std::wcout << std::right;
				}
				m_pf.left(pf.left());
			}
			if(pf.uppercase() != m_pf.uppercase())
			{
				if(pf.uppercase())
				{
					std::cout << std::uppercase;
					std::wcout << std::uppercase;
				}
				else
				{
					std::cout << std::nouppercase;
					std::wcout << std::nouppercase;
				}
				m_pf.uppercase(pf.uppercase());
			}
			m_pf = pf;
			return *this;
		}
		GAIA::GVOID STDStream::flush()
		{
			GAIA::SYNC::Autolock al(s_lrSTDStream);
			if(!this->enable_write())
				return;
			std::cout << std::flush;
		}
	}
}
