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
			typedef Extend2<_SizeType> __MyType;
		public:
			GINL _SizeType Increase(const _SizeType& size) const
			{
				GAST(size >= 0);
				_SizeType s = GSCAST(GAIA::NMAXTYPE)(size + 1);
				s = GSCAST(_SizeType)(GAIA::ALGO::gmin(GSCAST(GAIA::NMAXTYPE)(s), GSCAST(GAIA::NMAXTYPE)(1024L * 1024L * 10L)));
				return GSCAST(_SizeType)(size + s);
			}
		};
		template<typename _SizeType> class ExtendGold
		{
		public:
			typedef _SizeType _sizetype;
		public:
			typedef ExtendGold<_SizeType> __MyType;
		public:
			GINL _SizeType Increase(const _SizeType& size) const
			{
				GAST(size >= 0);
				_SizeType s = GSCAST(_SizeType)(GAIA::MATH::GOLD * (GAIA::REAL)size) + 1;
				s = GSCAST(_SizeType)(GAIA::ALGO::gmin(GSCAST(GAIA::NMAXTYPE)(s), GSCAST(GAIA::NMAXTYPE)(1024L * 1024L * 10L)));
				return GSCAST(_SizeType)(size + s);
			}
		};
	}
}

#endif
