#ifndef		__GAIA_ALGO_REPLACE_H__
#define		__GAIA_ALGO_REPLACE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _BufferType, typename _SizeType, typename _OldDataType, typename _NewDataType> _SizeType
			replace(_BufferType* p, const _SizeType& size,
					const _OldDataType& old_element, const _NewDataType& new_element)
		{
			_SizeType ret = 0;
			_SizeType index = 0;
			while(index < size)
			{
				if(p[index] == old_element)
				{
					p[index] = new_element;
					++ret;
				}
				++index;
			}
			return ret;
		}
	}
}

#endif
