#ifndef		__GAIA_CTN_H__
#define		__GAIA_CTN_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#define GAIA_CTN_PROXY \
			GINL GAIA::GVOID proxy(const _DataType* p, const _SizeType& size, const _SizeType& capacity)\
			{\
				m_pFront = GCCAST(_DataType*)(p);\
				m_size = size;\
				m_capacity = capacity;\
			}

#endif
