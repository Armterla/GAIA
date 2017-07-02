#ifndef		__GAIA_FSYS_DIRBASE_H__
#define		__GAIA_FSYS_DIRBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_trietree.h"

namespace GAIA
{
	namespace FSYS
	{
		/*!
			@brief File finder base class.
		*/
		class FSFinderBase : public GAIA::Entity
		{
		public:
			
			/*!
				@brief Get file's information.
			 
				@see GAIA::FSYS::FileInfo.
			*/
			virtual const FileInfo& GetInfo() const = 0;
		};

		/*!
			@brief File system base class.
		*/
		class DirBase : public GAIA::Entity
		{
		public:
			typedef GAIA::CTN::BasicTrieTree<GAIA::CTN::AString, GAIA::NM, GAIA::ALGO::ExtendGold<GAIA::NM> > __ResultTree;

		public:
			/*!
				@brief Get process's binary directory.
			*/
			virtual GAIA::GVOID GetBinaryDir(GAIA::CTN::AString& result) = 0;

			/*!
				@brief Set process's work directory.(current directory)
			*/
			virtual GAIA::BL SetWorkingDir(const GAIA::CH* pszPath) = 0;

			/*!
				@brief Get process's work directory.(current directory)
			*/
			virtual GAIA::GVOID GetWorkingDir(GAIA::CTN::AString& result) = 0;

			/*!
				@brief Create a directory.
			*/
			virtual GAIA::BL Create(const GAIA::CH* pszName, GAIA::BL bOverlapped) = 0;

			/*!
				@brief Remove a directory.
			*/
			virtual GAIA::BL Remove(const GAIA::CH* pszName, GAIA::BL bOverlapped) = 0;

			/*!
				@brief Copy directory.
			*/
			virtual GAIA::BL Copy(const GAIA::CH* pszSrcName, const GAIA::CH* pszDstName, GAIA::BL bOverlapped) = 0;

			/*!
				@brief Move directory.
			*/
			virtual GAIA::BL Move(const GAIA::CH* pszSrcName, const GAIA::CH* pszDstName) = 0;

			/*!
				@brief Check the directory exist or not.
			*/
			virtual GAIA::BL Exist(const GAIA::CH* pszName) = 0;

			/*!
				@brief Check a directory is empty or not.
			 
				@return If the directory is empty, return GAIA::True, or will return GAIA::False.
			*/
			virtual GAIA::BL Empty(const GAIA::CH* pszName, GAIA::BL& bEmpty) = 0;

			/*!
				@brief Remove a file.
			*/
			virtual GAIA::BL RemoveFile(const GAIA::CH* pszName) = 0;

			/*!
				@brief Copy file.
			*/
			virtual GAIA::BL CopyFile(const GAIA::CH* pszSrcName, const GAIA::CH* pszDstName) = 0;

			/*!
				@brief Move file.
			*/
			virtual GAIA::BL MoveFile(const GAIA::CH* pszSrcName, const GAIA::CH* pszDstName) = 0;

			/*!
				@brief Check the file exist or not.
			*/
			virtual GAIA::BL ExistFile(const GAIA::CH* pszName) = 0;

			/*!
				@brief Get file's information.
			 
				@see GAIA:FSYS::FileInfo.
			*/
			virtual GAIA::BL GetInfo(const GAIA::CH* pszName, FileInfo& fi) = 0;

			/*!
				@brief Begin search file directory.
			*/
			virtual GAIA::FSYS::FSFinderBase* Find(const GAIA::CH* pszDirName) = 0;

			/*!
				@brief Find next file.
			*/
			virtual GAIA::BL FindNext(GAIA::FSYS::FSFinderBase& df) = 0;

			/*!
				@brief End find file.
			*/
			virtual GAIA::BL FindEnd(GAIA::FSYS::FSFinderBase& df) = 0;

			/*!
				@brief Collect files in a directory.
			*/
			virtual GAIA::BL CollectFile(const GAIA::CH* pszName, const GAIA::CH* pszFilter, GAIA::BL bOverlapped, __ResultTree& treeResult) = 0;

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
