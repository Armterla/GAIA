#ifndef		__GAIA_STREAM_STRINGSTREAM_H__
#define		__GAIA_STREAM_STRINGSTREAM_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_ctn_string.h"
#include "gaia_stream_format.h"
#include "gaia_stream_streambase.h"

namespace GAIA
{
	namespace STREAM
	{
		/*!
			@brief String stream.

			@remarks This class is NOT thread safe.
		*/
		class StringStream : public StreamBase
		{
		public:
			typedef GAIA::TCH __CharType;
			typedef GAIA::CTN::TString __StringType;

		public:
			GINL StringStream(){this->init();}

			GINL virtual GAIA::GVOID enable_write(GAIA::BL bEnable){m_bEnableWrite = bEnable;}
			GINL virtual GAIA::BL enable_write() const{return m_bEnableWrite;}
			GINL virtual GAIA::GVOID enable_read(GAIA::BL bEnable){m_bEnableRead = bEnable;}
			GINL virtual GAIA::BL enable_read() const{return m_bEnableRead;}
			GINL virtual StringStream& operator << (GAIA::BL t)
			{
				if(!this->enable_write())
					return *this;
				if(t)
					m_str += _T("1");
				else
					m_str += _T("0");
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::NM t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::UM t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::N8 t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::N16 t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::N32 t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (const GAIA::N64& t)
			{
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::U8 t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::U16 t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::U32 t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (const GAIA::U64& t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (GAIA::F32 t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (const GAIA::F64& t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv(t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (const GAIA::WCH& t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				GAIA::ALGO::castv((GAIA::N64)t, szTemp, sizeofarray(szTemp));
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator << (const GAIA::CH* p)
			{
				if(!this->enable_write())
					return *this;
				m_str += p;
				return *this;
			}
			GINL virtual StringStream& operator << (const GAIA::WCH* p)
			{
				if(!this->enable_write())
					return *this;
				m_str += p;
				return *this;
			}
			GINL virtual StringStream& operator << (const GAIA::X128& t)
			{
				if(!this->enable_write())
					return *this;
				__CharType szTemp[64];
				t.tostring(szTemp);
				m_str += szTemp;
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::BL& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::NM& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::UM& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::N8& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::N16& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::N32& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::N64& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::U8& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::U16& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::U32& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::U64& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::F32& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::F64& t)
			{
				this->readvalue(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::WCH& t)
			{
				this->readchar(t);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::CH* p)
			{
				this->readchars(p);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::WCH* p)
			{
				this->readchars(p);
				return *this;
			}
			GINL virtual StringStream& operator >> (GAIA::X128& t)
			{
				if(!this->enable_read())
					return *this;
				if(!t.check(m_str.fptr() + m_sReadIndex))
					return *this;
				t.fromstring(m_str.fptr() + m_sReadIndex);
				m_sReadIndex += 32;
				return *this;
			}
			GINL virtual StringStream& operator >> (StreamFormat& pf)
			{
				if(!this->enable_read())
					return *this;
				pf = m_pf;
				return *this;
			}
			GINL virtual StringStream& operator << (const StreamFormat& pf)
			{
				m_pf = pf;
				return *this;
			}
			GINL virtual GAIA::GVOID flush()
			{
				if(!this->enable_write())
					return;
			}

			GINL GAIA::GVOID setstring(const __CharType* p){m_str = p;}
			GINL const __CharType* getstring() const{return m_str.fptr();}
			GINL GAIA::GVOID clear(){m_str.clear(); m_sReadIndex = 0;}
			GINL GAIA::GVOID destroy(){m_str.destroy(); m_sReadIndex = 0;}
			GINL GAIA::NUM tell() const{return m_sReadIndex;}
			GINL GAIA::GVOID seek(GAIA::NUM sReadIndex){m_sReadIndex = sReadIndex;}

		private:
			GINL GAIA::GVOID init()
			{
				m_pf.reset();
				m_bEnableWrite = GAIA::True;
				m_bEnableRead = GAIA::True;
				m_sReadIndex = 0;
			}
			
			template<typename _ParamDataType> GAIA::BL readvalue(_ParamDataType& t)
			{
				if(!this->enable_read())
					return GAIA::False;
				if(m_sReadIndex >= m_str.size())
					return GAIA::False;
				m_sReadIndex = GSCAST(GAIA::NUM)(GAIA::ALGO::gstrtrim(m_str.fptr() + m_sReadIndex, " \t\n\r") - m_str.fptr());
				__CharType* pBegin = m_str.fptr() + m_sReadIndex;
				__CharType szTemp[40];
				__CharType* pFinded = GAIA::ALGO::gstrchs(pBegin, " \t\n\r");
				if(pFinded == GNIL)
				{
					if(m_str.size() - m_sReadIndex > sizeofarray(szTemp))
						return GAIA::False;
					GAIA::ALGO::gstrcpy(szTemp, pBegin);
					m_sReadIndex += m_str.size() - m_sReadIndex;
				}
				else
				{
					if(pFinded - pBegin >= sizeofarray(szTemp))
						return GAIA::False;
					GAIA::ALGO::gstrcpy(szTemp, pBegin, pFinded - pBegin);
					m_sReadIndex += pFinded - pBegin;
				}
				t = GAIA::ALGO::acasts(szTemp);
				return GAIA::True;
			}

			template<typename _ParamDataType> GAIA::BL readchar(_ParamDataType& t)
			{
				if(!this->enable_read())
					return GAIA::False;
				if(m_sReadIndex >= m_str.size())
					return GAIA::False;
				t = *(m_str.fptr() + m_sReadIndex);
				++m_sReadIndex;
				return GAIA::True;
			}

			template<typename _ParamDataType> GAIA::BL readchars(_ParamDataType* p)
			{
				if(!this->enable_read())
					return GAIA::False;
				if(m_sReadIndex >= m_str.size())
					return GAIA::False;
				if(p == GNIL)
					return GAIA::False;
				GAIA::ALGO::gstrcpy(p, m_str.fptr() + m_sReadIndex);
				m_sReadIndex = m_str.size();
				return GAIA::True;
			}

		private:
			StreamFormat m_pf;
			GAIA::U8 m_bEnableWrite : 1;
			GAIA::U8 m_bEnableRead : 1;
			__StringType m_str;
			GAIA::NUM m_sReadIndex;
		};
	}
}

#endif
