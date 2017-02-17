#ifndef		__GAIA_FSYS_DIRBASE_H__
#define		__GAIA_FSYS_DIRBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_trietree.h"

namespace GAIA
{
	namespace FSYS
	{
		class FSFinderBase : public GAIA::Base
		{
		public:
			/*!
				@brief
			*/
			virtual const FileInfo& GetInfo() const = 0;
		};

		class DirBase : public GAIA::Entity
		{
		public:
			typedef GAIA::CTN::BasicTrieTree<GAIA::CTN::TString, GAIA::NM, GAIA::ALGO::ExtendGold<GAIA::NM> > __ResultTree;

		public:
			/*!
				@brief
			*/
			virtual GAIA::GVOID GetBinaryDir(GAIA::CTN::TString& result) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL SetWorkingDir(const GAIA::TCH* pszPath) = 0;

			/*!
				@brief
			*/
			virtual GAIA::GVOID GetWorkingDir(GAIA::CTN::TString& result) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Create(const GAIA::TCH* pszName, GAIA::BL bOverlapped) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Remove(const GAIA::TCH* pszName, GAIA::BL bOverlapped) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Copy(const GAIA::TCH* pszSrcName, const GAIA::TCH* pszDstName, GAIA::BL bOverlapped) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Move(const GAIA::TCH* pszSrcName, const GAIA::TCH* pszDstName) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Exist(const GAIA::TCH* pszName) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Empty(const GAIA::TCH* pszName, GAIA::BL& bEmpty) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL RemoveFile(const GAIA::TCH* pszName) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL CopyFile(const GAIA::TCH* pszSrcName, const GAIA::TCH* pszDstName) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL MoveFile(const GAIA::TCH* pszSrcName, const GAIA::TCH* pszDstName) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL ExistFile(const GAIA::TCH* pszName) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL GetInfo(const GAIA::TCH* pszName, FileInfo& fi) = 0;

			/*!
				@brief
			*/
			virtual GAIA::FSYS::FSFinderBase* Find(const GAIA::TCH* pszDirName) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL FindNext(GAIA::FSYS::FSFinderBase& df) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL FindEnd(GAIA::FSYS::FSFinderBase& df) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL CollectFile(const GAIA::TCH* pszName, const GAIA::TCH* pszFilter, GAIA::BL bOverlapped, __ResultTree& treeResult) = 0;

		private:
			/*!
				@brief Access denied overload function.
			*/
			GINL DirBase& operator = (const DirBase& src)
			{
				GAST(&src != this);
				return *this;
			}
		};
	}
}

#endif
