#ifndef		__GAIA_CTN_COOPERATION_H__
#define		__GAIA_CTN_COOPERATION_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_chars.h"
#include "gaia_ctn_string.h"

namespace GAIA
{
	namespace CTN
	{
		/* BasicChars and BasicString's convert. */
		template<typename _DataType, typename _SizeType, _SizeType _Size>
		template<typename _ParamSizeType>
		BasicChars<_DataType, _SizeType, _Size>&
			BasicChars<_DataType, _SizeType, _Size>::operator =
			(const GAIA::CTN::BasicString<_DataType, _ParamSizeType>& src)
		{
			if(src.empty())
				this->clear();
			else
				(*this) = src.fptr();
			return *this;
		}

		template<typename _DataType, typename _SizeType>
		template<typename _ParamSizeType, _ParamSizeType _Size>
		BasicString<_DataType, _SizeType>&
			BasicString<_DataType, _SizeType>::operator =
			(const GAIA::CTN::BasicChars<_DataType, _ParamSizeType, _Size>& src)
		{
			if(src.empty())
				this->clear();
			else
				(*this) = src.fptr();
			return *this;
		}

		template<typename _DataType, typename _StringSizeType, typename _CharsSizeType, _CharsSizeType size>
		GAIA::CTN::BasicChars<_DataType, _CharsSizeType, size> operator + (
			const GAIA::CTN::BasicChars<_DataType, _CharsSizeType, size>& p1,
			const GAIA::CTN::BasicString<_DataType, _StringSizeType>& p2)
		{
			GAIA::CTN::BasicChars<_DataType, _CharsSizeType, size> ret = p1;
			ret += p2.fptr();
			return ret;
		}

		template<typename _DataType, typename _StringSizeType, typename _CharsSizeType, _CharsSizeType size>
		GAIA::CTN::BasicString<_DataType, _StringSizeType> operator + (
			const GAIA::CTN::BasicString<_DataType, _StringSizeType>& p1,
			const GAIA::CTN::BasicChars<_DataType, _CharsSizeType, size>& p2)
		{
			GAIA::CTN::BasicString<_DataType, _StringSizeType> ret = p1;
			ret += p2.fptr();
			return ret;
		}

		template<typename _DataType, typename _StringSizeType, typename _CharsSizeType, _CharsSizeType size>
		GAIA::CTN::BasicChars<_DataType, _CharsSizeType, size>& operator += (
			GAIA::CTN::BasicChars<_DataType, _CharsSizeType, size>& p1,
			const GAIA::CTN::BasicString<_DataType, _StringSizeType>& p2)
		{
			p1 += p2.fptr();
			return p1;
		}

		template<typename _DataType, typename _StringSizeType, typename _CharsSizeType, _CharsSizeType size>
		GAIA::CTN::BasicString<_DataType, _StringSizeType>& operator += (
			GAIA::CTN::BasicString<_DataType, _StringSizeType>& p1,
			const GAIA::CTN::BasicChars<_DataType, _CharsSizeType, size>& p2)
		{
			p1 += p2.fptr();
			return p1;
		}
	}
}

#endif
