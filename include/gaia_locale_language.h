#ifndef 	__GAIA_LOCALE_LANGUAGE_H__
#define 	__GAIA_LOCALE_LANGUAGE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace LOCALE
	{
		class LanguageCtx;
		class Language : public GAIA::Base
		{
		public:
			Language();
			~Language();

			GAIA::BL Load(const GAIA::CH* pszLanguageName);
			GAIA::BL Unload(const GAIA::CH* pszLanguageName);
			GAIA::BL IsLoaded(const GAIA::CH* pszLanguageName);
			GAIA::NUM GetLoadedCount() const;
			const GAIA::CH* GetLoaded(GAIA::NUM sIndex);

			GAIA::BL Select(GAIA::LOCALE::LanguageCtx& ctx, const GAIA::CH* pszLanguageName);
			GAIA::BL Unselect(GAIA::LOCALE::LanguageCtx& ctx, const GAIA::CH* pszLanguageName);

			GAIA::NUM GetStringPinyin(GAIA::LOCALE::LanguageCtx& ctx, const GAIA::CH* pszSrc, GAIA::NUM sSrcSize, GAIA::CH* pszDst, GAIA::NUM sDstSize) const;
			GAIA::NUM CheckString(GAIA::LOCALE::LanguageCtx& ctx, const GAIA::CH* psz, GAIA::NUM sSize) const;
			GAIA::GVOID MatchPinyinList(GAIA::LOCALE::LanguageCtx& ctx, const GAIA::CH* pszSrc, GAIA::NUM sSrcSize, GAIA::CTN::Vector<GAIA::NUM>& listResult) const;
			
			GAIA::NUM GetWordCount(GAIA::LOCALE::LanguageCtx& ctx) const;
			const GAIA::CH* GetWord(GAIA::LOCALE::LanguageCtx& ctx, GAIA::NUM sIndex) const;
		};
	}
}

#endif
