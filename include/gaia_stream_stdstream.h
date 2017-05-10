#ifndef		__GAIA_STREAM_STREAM_H__
#define		__GAIA_STREAM_STREAM_H__

#include "gaia_type.h"
#include "gaia_stream_format.h"
#include "gaia_stream_streambase.h"

namespace GAIA
{
	namespace STREAM
	{
		/*!
			@brief Standard stream.

			It implement by STL cin & cout.

			@remarks This class is thread safe.
		*/
		class STDStream : public StreamBase
		{
		public:
			STDStream(){this->init();}
			~STDStream()
			{
				if(m_pszPrepareRead != GNIL)
					gdel[] m_pszPrepareRead;
			}
			
			virtual GAIA::GVOID lock_read();
			virtual GAIA::GVOID unlock_read();
			virtual GAIA::GVOID lock_write();
			virtual GAIA::GVOID unlock_write();

			virtual GAIA::GVOID enable_write(GAIA::BL bEnable){m_bEnableWrite = bEnable;}
			virtual GAIA::BL enable_write() const{return m_bEnableWrite;}
			virtual GAIA::GVOID enable_read(GAIA::BL bEnable){m_bEnableRead = bEnable;}
			virtual GAIA::BL enable_read() const{return m_bEnableRead;}

			virtual STDStream& operator << (GAIA::BL t);
			virtual STDStream& operator << (GAIA::NM t);
			virtual STDStream& operator << (GAIA::UM t);
			virtual STDStream& operator << (GAIA::N8 t);
			virtual STDStream& operator << (GAIA::N16 t);
			virtual STDStream& operator << (GAIA::N32 t);
			virtual STDStream& operator << (const GAIA::N64& t);
			virtual STDStream& operator << (GAIA::U8 t);
			virtual STDStream& operator << (GAIA::U16 t);
			virtual STDStream& operator << (GAIA::U32 t);
			virtual STDStream& operator << (const GAIA::U64& t);
			virtual STDStream& operator << (GAIA::F32 t);
			virtual STDStream& operator << (const GAIA::F64& t);
			virtual STDStream& operator << (const GAIA::WCH& t);
			virtual STDStream& operator << (const GAIA::CH* p);
			virtual STDStream& operator << (const GAIA::WCH* p);
			virtual STDStream& operator << (const GAIA::X128& t);

			virtual STDStream& operator >> (GAIA::BL& t);
			virtual STDStream& operator >> (GAIA::NM& t);
			virtual STDStream& operator >> (GAIA::UM& t);
			virtual STDStream& operator >> (GAIA::N8& t);
			virtual STDStream& operator >> (GAIA::N16& t);
			virtual STDStream& operator >> (GAIA::N32& t);
			virtual STDStream& operator >> (GAIA::N64& t);
			virtual STDStream& operator >> (GAIA::U8& t);
			virtual STDStream& operator >> (GAIA::U16& t);
			virtual STDStream& operator >> (GAIA::U32& t);
			virtual STDStream& operator >> (GAIA::U64& t);
			virtual STDStream& operator >> (GAIA::F32& t);
			virtual STDStream& operator >> (GAIA::F64& t);
			virtual STDStream& operator >> (GAIA::WCH& t);
			virtual STDStream& operator >> (GAIA::CH* p);
			virtual STDStream& operator >> (GAIA::WCH* p);
			virtual STDStream& operator >> (GAIA::X128& t);

			virtual STDStream& operator >> (StreamFormat& pf);
			virtual STDStream& operator << (const StreamFormat& pf);

			virtual GAIA::GVOID flush();

			template<typename _ParamDataType> GAIA::GVOID prepare_read(const _ParamDataType* psz)
			{
				if(m_pszPrepareRead != GNIL)
				{
					gdel[] m_pszPrepareRead;
					m_pszPrepareRead = GNIL;
				}

				if(psz != GNIL && psz[0] != '\0')
				{
					GAIA::NUM sLen;
					for(sLen = 0; psz[sLen] != '\0'; ++sLen){}
					sLen++;
					m_pszPrepareRead = gnew GAIA::WCH[sLen];
					for(GAIA::NUM x = 0; x < sLen; ++x)
						m_pszPrepareRead[x] = psz[x];
					m_pszPrepareReadCursor = m_pszPrepareRead;
				}
			}

		private:
			GAIA::GVOID init()
			{
				m_pszPrepareRead = GNIL;
				m_pszPrepareReadCursor = GNIL;
				m_pf.reset();
				m_bEnableWrite = GAIA::True;
				m_bEnableRead = GAIA::True;
			}

		private:
			GAIA::WCH* m_pszPrepareRead;
			GAIA::WCH* m_pszPrepareReadCursor;
			StreamFormat m_pf;
			GAIA::U8 m_bEnableWrite : 1;
			GAIA::U8 m_bEnableRead : 1;
		};
	}
}

#endif
