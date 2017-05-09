#ifndef		__GAIA_DIGIT_BASE64_H__
#define		__GAIA_DIGIT_BASE64_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace DIGIT
	{
		/*!
		 	@brief Get base64 encode result size.
		 
		 	@param srclen [in] Specify the source length in bytes.
		 
		 	@return Return the size of the result in characters.
		*/
		template<typename _SizeType> _SizeType base64encode_resultsize(_SizeType srclen)
		{
			return ((srclen + 2) / 3) * 4;
		}
		
		/*!
		 	@brief Get base64 decode max result size.
		 
		 	@param srclen [in] Specify the source length in characters.
		 
		 	@return Return the size of the result in bytes.
		*/
		template<typename _SizeType> _SizeType base64decode_maxresultsize(_SizeType srclen)
		{
			return (srclen >> 2) * 3 + 2;
		}
		
		/*!
		 	@brief Base64 encode.
		 
		 	@param pDst [out] Used for saving the encoded result.
		
		 	@param dstlen [in] Specify the length of parameter pDst in characters.
		 
		 	@param pSrc [in] Specify the source binary buffer.
		 
		 	@param srclen [in] Specify the size of parameter pSrc in bytes.
		 
		 	@return Return practice length of the result in characters. If failed, return GINVALID.
		*/
		template<typename _DataType, typename _DstSizeType, typename _SrcSizeType> _DstSizeType base64encode(_DataType* pDst, _DstSizeType dstlen, const GAIA::U8* pSrc, _SrcSizeType srclen)
		{
			static const GAIA::U8 BIN2ASCII[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			GAST(pSrc != GNIL);
			GAST(srclen > 0);
			if(pSrc == GNIL || srclen <= 0)
				return GINVALID;
			if(pDst != GNIL)
			{
				GAST(dstlen >= ((srclen + 2) / 3) * 4);
				if(dstlen < ((srclen + 2) / 3) * 4)
					return GINVALID;
				for(GAIA::N32 i = srclen; i > 0; i -= 3)
				{
					GAIA::U8 b0, b1, b2;
					if(i >= 3)
					{
						b0 = *pSrc++;
						b1 = *pSrc++;
						b2 = *pSrc++;
						*pDst++ = BIN2ASCII[b0 >> 2];
						*pDst++ = BIN2ASCII[((b0 << 4) | (b1 >> 4)) & 0x3F];
						*pDst++ = BIN2ASCII[((b1 << 2) | (b2 >> 6)) & 0x3F];
						*pDst++ = BIN2ASCII[b2 & 0x3F];
					}
					else
					{
						b0 = *pSrc++;
						if(i == 2) b1 = *pSrc++; else b1 = 0;
						*pDst++ = BIN2ASCII[b0 >> 2];
						*pDst++ = BIN2ASCII[((b0 << 4) | (b1 >> 4)) & 0x3F];
						*pDst++ = (i == 1) ? '=' : BIN2ASCII[(b1 << 2) & 0x3F];
						*pDst++ = '=';
					}
				}
			}
			return ((srclen + 2) / 3) * 4;
		}

		/*!
		 	@brief Base64 decode.
		 
		 	@param pDst [out] Used for saving decoded result.
		 
		 	@param dstlen [in] Specify the size of parameter pDst in bytes.
		 
		 	@param pSrc [in] Specify the source base64 string.
		 
		 	@param srclen [in] Specify the length of parameter pSrc in characters.
		 
		 	@return Return the practice size of the result in bytes. If failed, return GINVALID.
		*/
		template<typename _DataType, typename _DstSizeType, typename _SrcSizeType> _DstSizeType base64decode(GAIA::U8* pDst, _DstSizeType dstlen, const _DataType* pSrc, _SrcSizeType srclen)
		{
			static const GAIA::U8 ASCII2BIN[128] = {
				0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0xF0,0xFF,0xFF,0xF1,0xFF,0xFF,
				0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
				0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3E,0xFF,0xF2,0xFF,0x3F,
				0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,
				0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,
				0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,
				0xFF,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
				0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF
			};
			GAST(pSrc != GNIL);
			GAST(srclen > 0);
			if(pSrc == GNIL || srclen <= 0)
				return GINVALID;
			GAIA::U8 b[4];
			_DataType t;
			GAIA::U8* pDstOld = pDst;
			while(srclen > 0)
			{
				t = *pSrc;
				if((t < 0) || (t >= 0x80))
					return GINVALID;
				else
				{
					if(ASCII2BIN[t] == 0xE0)
					{
						pSrc++;
						srclen--;
					}
					else
						break;
				}
			}
			while(srclen >= 4)
			{
				t = pSrc[srclen - 1];
				if((t < 0) || (t >= 0x80))
					return GINVALID;
				else
				{
					if((ASCII2BIN[t] | 0x13) == 0xF3)
						srclen--;
					else
						break;
				}
			}
			if((srclen % 4) != 0)
				return GINVALID;
			if(pDst != GNIL)
			{
				for(GAIA::N32 i = 0; i < srclen; i += 4)
				{
					GAIA::N32 j;
					for(j = 0; j < 4; j++)
					{
						t = *pSrc++;
						if((t < 0) || (t >= 0x80))
							return GINVALID;
						else
						{
							if(t == '=')
								break;
							else
							{
								b[j] = ASCII2BIN[t];
								if(b[j] & 0x80)
									return GINVALID;
							}
						}
					}
					if(j == 4)
					{
						GAST(dstlen - (pDst - pDstOld) >= 3);
						if(dstlen - (pDst - pDstOld) < 3)
							return GINVALID;
						*pDst++ = (b[0] << 2) | (b[1] >> 4);
						*pDst++ = (b[1] << 4) | (b[2] >> 2);
						*pDst++ = (b[2] << 6) | b[3];
					}
					else if(j == 3)
					{
						GAST(dstlen - (pDst - pDstOld) >= 2);
						if(dstlen - (pDst - pDstOld) < 2)
							return GINVALID;
						*pDst++ = (b[0] << 2) | (b[1] >> 4);
						*pDst++ = (b[1] << 4) | (b[2] >> 2);
						return (i >> 2) * 3 + 2;
					}
					else if(j == 2)
					{
						GAST(dstlen - (pDst - pDstOld) >= 1);
						if(dstlen - (pDst - pDstOld) < 1)
							return GINVALID;
						*pDst++ = (b[0] << 2) | (b[1] >> 4);
						return (i >> 2) * 3 + 1;
					}
					else
						return GINVALID;
				}
			}
			return (srclen >> 2) * 3;
		}
	}
}

#endif
