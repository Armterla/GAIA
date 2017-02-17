#ifndef		__GAIA_ALGO_EXTEND_H__
#define		__GAIA_ALGO_EXTEND_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_constant.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _SizeType> class Extend1
		{
		public:
			typedef _SizeType _sizetype;
		public:
			typedef Extend1<_SizeType> __MyType;
		public:
			GINL _SizeType Increase(const _SizeType& size) const
			{
				GAST(size >= 0);
				return GSCAST(_SizeType)(size + 1);
			}
		};
		template<typename _SizeType> class Extend2
		{
		public:
			typedef _SizeType _sizetype;
		public:
			typedef Extend1<_SizeType> __MyType;
		public:
			GINL _SizeType Increase(const _SizeType& size) const
			{
				GAST(size >= 0);
				return GSCAST(_SizeType)(GAIA::ALGO::gmin(GSCAST(GAIA::NMAXTYPE)(size * 2 + 1), GSCAST(GAIA::NMAXTYPE)(1024 * 1024 * 10)));
			}
		};
		template<typename _SizeType> class ExtendGold
		{
		public:
			typedef _SizeType _sizetype;
		public:
			typedef Extend1<_SizeType> __MyType;
		public:
			GINL _SizeType Increase(const _SizeType& size) const
			{
				GAST(size >= 0);
				_SizeType ret = (_SizeType)((GAIA::MATH::GOLD + R(1.0)) * (GAIA::REAL)size) + 1;
				return GSCAST(_SizeType)(GAIA::ALGO::gmin(GSCAST(GAIA::NMAXTYPE)(ret), GSCAST(GAIA::NMAXTYPE)(1024 * 1024 * 10)));
			}
		};
	}
}

#endif
