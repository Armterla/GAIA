#ifndef		__GAIA_MSYS_HEAPCRT_H__
#define		__GAIA_MSYS_HEAPCRT_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_msys_base.h"
#include "gaia_sync_atomic.h"

#include <stdlib.h>

namespace GAIA
{
	namespace MSYS
	{
		class HeapCRT : public GAIA::MSYS::Heap
		{
		public:
			GINL HeapCRT(){this->init();}
			GINL virtual GAIA::GVOID* memory_alloc(const GAIA::UM& uSize)
			{
				GAST(uSize > 0);
				GAIA::GVOID* pRet = malloc(uSize + sizeof(GAIA::UM));
				*(GSCAST(GAIA::UM*)(pRet)) = uSize;
				m_capacity.Add(uSize + sizeof(GAIA::UM));
				m_usesize.Add(uSize);
				m_piecesize.Increase();
				m_alloctimes.Increase();
				return (GSCAST(GAIA::UM*)(pRet)) + 1;
			}
			GINL virtual GAIA::GVOID memory_release(GAIA::GVOID* p)
			{
				GAST(!!p);
				m_capacity.Add(-(GAIA::N64)this->memory_size(p) - (GAIA::N64)sizeof(GAIA::UM));
				m_usesize.Add(-(GAIA::N64)this->memory_size(p));
				m_piecesize.Decrease();
				free(GSCAST(GAIA::UM*)(p) - 1);
			}
			GINL virtual GAIA::UM memory_size(GAIA::GVOID* p)
			{
				GAST(!!p);
				return *(GSCAST(GAIA::UM*)(p) - 1);
			}
			GINL virtual GAIA::UM capacity()
			{
				return (GAIA::UM)m_capacity;
			}
			GINL virtual GAIA::UM size()
			{
				return (GAIA::UM)m_capacity;
			}
			GINL virtual GAIA::UM use_size()
			{
				return (GAIA::UM)m_usesize;
			}
			GINL virtual GAIA::UM piece_size()
			{
				return (GAIA::UM)m_piecesize;
			}
			GINL virtual GAIA::U64 alloc_times()
			{
				return m_alloctimes;
			}
		private:
			GINL GAIA::GVOID init(){}
		private:
			GAIA::SYNC::Atomic m_capacity;
			GAIA::SYNC::Atomic m_usesize;
			GAIA::SYNC::Atomic m_piecesize;
			GAIA::SYNC::Atomic m_alloctimes;
		};
	}
}

#endif
