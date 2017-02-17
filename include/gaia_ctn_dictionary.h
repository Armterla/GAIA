#ifndef		__GAIA_CTN_DIRTIONARY_H__
#define		__GAIA_CTN_DIRTIONARY_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicDictionary : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicDictionary<_DataType, _SizeType, _ExtendType> __MyType;
		public:
			GINL BasicDictionary(){}
			GINL BasicDictionary(const __MyType& src){}
			GINL GAIA::BL empty() const;
			GINL const _SizeType& size() const;
			GINL const _SizeType& capacity() const;
			GINL GAIA::GVOID resize(const _SizeType& size);
			GINL GAIA::GVOID reserve(const _SizeType& size);
			GINL GAIA::GVOID clear();
			GINL GAIA::GVOID destroy();
		private:
			GINL GAIA::GVOID init(){}
		private:
		};
	}
}

#endif
