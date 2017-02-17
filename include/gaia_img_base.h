#ifndef		__GAIA_IMG_BASE_H__
#define		__GAIA_IMG_BASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_buffer.h"

namespace GAIA
{
	namespace IMG
	{
		GAIA_ENUM_BEGIN(IMAGE_FORMAT)
			IMAGE_FORMAT_PNG,
			IMAGE_FORMAT_JPEG,
		GAIA_ENUM_END(IMAGE_FORMAT)

		GAIA_ENUM_BEGIN(IMAGE_CHANNEL_FORMAT)
			IMAGE_CHANNEL_FORMAT_RGB,
			IMAGE_CHANNEL_FORMAT_RGBA,
		GAIA_ENUM_END(IMAGE_CHANNEL_FORMAT)

		GAIA_ENUM_BEGIN(IMAGE_SAMPLER_TYPE)
			IMAGE_SAMPLER_TYPE_NEAREST,
			IMAGE_SAMPLER_TYPE_LINEAR,
		GAIA_ENUM_END(IMAGE_SAMPLER_TYPE)

		class ImageInfo : public GAIA::Base
		{
		public:
			GINL GAIA::GVOID reset()
			{
				imgfmt = GAIA::IMG::IMAGE_FORMAT_INVALID;
				chnfmt = GAIA::IMG::IMAGE_CHANNEL_FORMAT_INVALID;
				sChannelCount = 0;
				sWidth = 0;
				sHeight = 0;
				sDepth = 0;
			}
			GINL GAIA::BL check() const
			{
				if(imgfmt == GAIA::IMG::IMAGE_FORMAT_INVALID || imgfmt >= GAIA::IMG::IMAGE_FORMAT_MAXENUMCOUNT)
					return GAIA::False;
				if(chnfmt == GAIA::IMG::IMAGE_CHANNEL_FORMAT_INVALID || chnfmt >= GAIA::IMG::IMAGE_CHANNEL_FORMAT_MAXENUMCOUNT)
					return GAIA::False;
				if(sChannelCount <= 0)
					return GAIA::False;
				if(sWidth <= 0)
					return GAIA::False;
				if(sHeight <= 0)
					return GAIA::False;
				if(sDepth <= 0)
					return GAIA::False;
				return GAIA::True;
			}
			GINL GAIA::BL existalpha() const
			{
				if(chnfmt == GAIA::IMG::IMAGE_CHANNEL_FORMAT_RGBA)
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::NUM pixelsize() const
			{
				if(sDepth <= 0 || sChannelCount <= 0)
					return GINVALID;
				return sDepth * sChannelCount / 8;
			}
			GINL GAIA::NUM imagesize() const
			{
				if(sWidth <= 0 || sHeight <= 0 || sDepth <= 0 || sChannelCount <= 0)
					return GINVALID;
				return sWidth * sHeight * sDepth * sChannelCount / 8;
			}
		public:
			GAIA::IMG::IMAGE_FORMAT imgfmt;
			GAIA::IMG::IMAGE_CHANNEL_FORMAT chnfmt;
			GAIA::NUM sChannelCount;
			GAIA::NUM sWidth;
			GAIA::NUM sHeight;
			GAIA::NUM sDepth;
		};

		class ImageBase : public GAIA::Entity
		{
		public:

			// Static get info.
			virtual GAIA::BL GetInfo(const GAIA::CH* pszFileName, GAIA::IMG::ImageInfo& imginfo) const = 0;
			virtual GAIA::BL GetInfo(const GAIA::GVOID* pBuf, GAIA::NUM sSize, GAIA::IMG::ImageInfo& imginfo) const = 0;

			// For streamed read write.
			virtual GAIA::BL Open(const GAIA::CH* pszFileName) = 0;
			virtual GAIA::BL Open(const GAIA::GVOID* pBuf, GAIA::NUM sSize) = 0;
			virtual GAIA::BL Close() = 0;
			virtual GAIA::BL IsOpenned() const = 0;

			// For no-streamed read write.
			virtual GAIA::BL Load(const GAIA::CH* pszFileName, GAIA::NUM sWidth = GINVALID, GAIA::NUM sHeight = GINVALID, GAIA::IMG::IMAGE_SAMPLER_TYPE sam = GAIA::IMG::IMAGE_SAMPLER_TYPE_LINEAR) = 0;
			virtual GAIA::BL Load(const GAIA::GVOID* pBuf, GAIA::NUM sSize, GAIA::NUM sWidth = GINVALID, GAIA::NUM sHeight = GINVALID, GAIA::IMG::IMAGE_SAMPLER_TYPE sam = GAIA::IMG::IMAGE_SAMPLER_TYPE_LINEAR) = 0;
			virtual GAIA::BL Save(const GAIA::CH* pszFileName, GAIA::N32 nCompressLevel = 50) = 0;
			virtual GAIA::BL Save(GAIA::GVOID* pBuf, GAIA::NUM sSize, GAIA::NUM& sResultSize, GAIA::N32 nCompressLevel = 50) = 0;
			virtual const GAIA::IMG::ImageInfo& GetImageInfo() const = 0;

			// Read.
			virtual const GAIA::GVOID* Read(GAIA::NUM x, GAIA::NUM y) = 0;
			virtual const GAIA::GVOID* ReadLine(GAIA::NUM y) = 0;

			// Write.
			virtual const GAIA::BL Write(GAIA::NUM x, GAIA::NUM y) = 0;
			virtual const GAIA::BL WriteLine(GAIA::NUM y, const GAIA::GVOID* pBuf, GAIA::NUM sSize = GINVALID) = 0;

		protected:
			GAIA::IMG::ImageInfo m_info;
			GAIA::CTN::Buffer m_buf;
		};
	}
}

#endif
