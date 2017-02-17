#ifndef		__GAIA_FSYS_DIR_H__
#define		__GAIA_FSYS_DIR_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_time.h"
#include "gaia_fsys_filebase.h"
#include "gaia_fsys_dirbase.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#else
#	include <stdio.h>
#	include <sys/stat.h>
#	include <dirent.h>
#	include <unistd.h>
#endif

namespace GAIA
{
	namespace FSYS
	{
		class FSFinder : public FSFinderBase
		{
			friend class Dir;
		public:
			virtual const FileInfo& GetInfo() const
			{
				return fi;
			}
			GINL GAIA::GVOID UpdateFileInfo()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAST(hFF != (HANDLE)GINVALID);
				if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					GAIA::TCH* pEnd = GAIA::ALGO::gstrend(fdata.cFileName);
					GAST(pEnd - fdata.cFileName < GAIA::MAXPL - 1);
					if(pEnd[-1] != '/' && pEnd[-1] != '\\')
					{
						pEnd[0] = '/';
						pEnd[1] = '\0';
					}
				}
				fi.pszName = fdata.cFileName;
				fi.bDirectory = (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
				GAIA::TIME::FileTimeToTime(fdata.ftCreationTime, fi.uCreateTime);
				GAIA::TIME::FileTimeToTime(fdata.ftLastAccessTime, fi.uLastReadTime);
				GAIA::TIME::FileTimeToTime(fdata.ftLastWriteTime, fi.uLastWriteTime);
			#else
				// TODO:
			#endif
			}
		private:
			GINL FSFinder(){this->init();}
		private:
			GINL GAIA::GVOID init()
			{
		#if GAIA_OS == GAIA_OS_WINDOWS
				hFF = (HANDLE)GINVALID;
				zeromem(&fdata);
				fi.reset();
		#else
		#endif
			}
		private:
		#if GAIA_OS == GAIA_OS_WINDOWS
			HANDLE hFF;
		#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
				WIN32_FIND_DATAA fdata;
		#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
				WIN32_FIND_DATAW fdata;
		#	endif
		#else
			// TODO:
		#endif
			FileInfo fi;
		};

		class Dir : public GAIA::FSYS::DirBase
		{
		public:
			GINL virtual GAIA::GVOID GetBinaryDir(GAIA::CTN::TString& result)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::TCH szPath[MAXPL];
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					::GetModuleFileNameA(GNIL, szPath, MAXPL);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					::GetModuleFileNameW(GNIL, szPath, MAXPL);
			#	endif
				GAIA::TCH* p = GAIA::ALGO::gstrdropr(szPath, _T("/\\"));
				result = szPath;
			#else
				// TODO:
			#endif
			}
			GINL virtual GAIA::BL SetWorkingDir(const GAIA::TCH* pszPath)
			{
				if(GAIA::ALGO::gstremp(pszPath))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(::SetCurrentDirectoryA(pszPath))
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					if(::SetCurrentDirectoryW(pszPath))
			#	endif
			#else
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(chdir(pszPath) == 0)
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempPath[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszPath, GINVALID, szTempPath, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempPath[sLen] = '\0';
					if(chdir(szTempPath) == 0)
			#	endif
			#endif
					return GAIA::True;
				return GAIA::False;
			}
			GINL virtual GAIA::GVOID GetWorkingDir(GAIA::CTN::TString& result)
			{
				GAIA::TCH szPath[MAXPL];
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					::GetCurrentDirectoryA(MAXPL, szPath);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					::GetCurrentDirectoryW(MAXPL, szPath);
			#	endif
			#else
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					::getcwd(szPath, MAXPL);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempPath[GAIA::MAXPL];
					::getcwd(szTempPath, MAXPL);
					GAIA::NUM sLen = GAIA::LOCALE::m2w(szTempPath, GINVALID, szPath, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szPath[sLen] = '\0';
			#	endif
			#endif
				result = szPath;
			}
			GINL virtual GAIA::BL Create(const GAIA::TCH* pszName, GAIA::BL bOverlapped)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
				if(this->Exist(pszName))
					return GAIA::False;
				/* Generate szFind for recursive file collection. */
				GAIA::TCH szFind[MAXPL];
				GAIA::ALGO::gstrcpy(szFind, pszName);
				GAIA::TCH* p = GAIA::ALGO::gstrend(szFind);
				--p;
				if(*p != '\\' && *p != '/')
					GAIA::ALGO::gstrcat(p, _T("/"));
				if(bOverlapped)
				{
					const GAIA::TCH* pCursor = szFind;
				#if GAIA_OS == GAIA_OS_WINDOWS
					/* Jump after Windows-OS disk name. */
					if(GAIA::ALGO::gstrch(szFind, ':') != GNIL)
					{
						pCursor = GAIA::ALGO::gstrdrop(pCursor, _T("/\\"));
						if(pCursor == GNIL)
							return GAIA::False;
						++pCursor;
					}
				#endif
					while((pCursor = GAIA::ALGO::gstrdrop(pCursor, _T("/\\\0"))) != GNIL)
					{
						GAIA::TCH sz[MAXPL];
						GAIA::ALGO::gmemcpy(sz, szFind, (pCursor - szFind + 1) * sizeof(GAIA::TCH));
						sz[pCursor - szFind + 1] = 0;
						if(!this->Exist(sz))
						{
						#if GAIA_OS == GAIA_OS_WINDOWS
						#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
								if(!::CreateDirectoryA(sz, GNIL))
						#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
								if(!::CreateDirectoryW(sz, GNIL))
						#	endif
						#else
						#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
								if(mkdir(sz, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
						#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
								GAIA::CH szTemp[GAIA::MAXPL];
								GAIA::NUM sLen = GAIA::LOCALE::w2m(sz, GINVALID, szTemp, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
								szTemp[sLen] = '\0';
								if(mkdir(szTemp, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
						#	endif
						#endif
								return GAIA::False;
						}
						if(*pCursor == 0)
							break;
						else
							++pCursor;
					}
					return GAIA::True;
				}
				else
				{
				#if GAIA_OS == GAIA_OS_WINDOWS
				#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
						if(::CreateDirectoryA(pszName, GNIL))
				#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
						if(::CreateDirectoryW(pszName, GNIL))
				#	endif
				#else
				#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
						if(mkdir(pszName, S_IRWXU | S_IRWXG | S_IRWXO) == 0)
				#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
						GAIA::CH szTempName[GAIA::MAXPL];
						GAIA::NUM sLen = GAIA::LOCALE::w2m(pszName, GINVALID, szTempName, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
						szTempName[sLen] = '\0';
						if(mkdir(szTempName, S_IRWXU | S_IRWXG | S_IRWXO) == 0)
				#	endif
				#endif
						return GAIA::True;
					return GAIA::False;
				}
			}
			GINL virtual GAIA::BL Remove(const GAIA::TCH* pszName, GAIA::BL bOverlapped)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(bOverlapped)
				{
					/* Generate szFind for recursive file collection. */
					GAIA::TCH szFind[MAXPL];
					GAIA::ALGO::gstrcpy(szFind, pszName);
					GAIA::TCH* p = GAIA::ALGO::gstrend(szFind);
					--p;
					if(*p != '\\' && *p != '/')
						GAIA::ALGO::gstrcat(p, _T("/"));
					/* Generate szTarget for FindFirstFile. */
					GAIA::TCH szTarget[MAXPL];
					GAIA::ALGO::gstrcpy(szTarget, szFind);
					GAIA::ALGO::gstrcat(szTarget + (p - szFind), _T("*.*"));
				#if GAIA_CHARSET == GAIA_CHARSET_ANSI
						WIN32_FIND_DATAA fdata;
						HANDLE hFF = ::FindFirstFileA(szTarget, &fdata);
				#elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
						WIN32_FIND_DATAW fdata;
						HANDLE hFF = ::FindFirstFileW(szTarget, &fdata);
				#endif
					if(hFF == (HANDLE)GINVALID)
						return GAIA::False;
					GAIA::BL bFinded = GAIA::True;
					while(bFinded)
					{
						if(GAIA::ALGO::gstrcmp(fdata.cFileName, _T(".")) == 0 ||
							GAIA::ALGO::gstrcmp(fdata.cFileName, _T("..")) == 0){}
						else
						{
							GAIA::TCH sz[MAXPL];
							GAIA::ALGO::gstrcpy(sz, szFind);
							GAIA::ALGO::gstrcat(sz, fdata.cFileName);
							if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{
								if(!this->Remove(sz, bOverlapped))
								{
									::FindClose(hFF);
									return GAIA::False;
								}
							}
							else
							{
								if(!this->RemoveFile(sz))
								{
									::FindClose(hFF);
									return GAIA::False;
								}
							}
						}
					#if GAIA_CHARSET == GAIA_CHARSET_ANSI
						bFinded = ::FindNextFileA(hFF, &fdata) != 0;
					#elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
						bFinded = ::FindNextFileW(hFF, &fdata) != 0;
					#endif
					}
					::FindClose(hFF);
					return this->Remove(pszName, GAIA::False);
				}
				else
				{
				#if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(::RemoveDirectoryA(pszName))
				#elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					if(::RemoveDirectoryW(pszName))
				#endif
						return GAIA::True;
					return GAIA::False;
				}
			#else
				if(bOverlapped)
				{
					/* Generate szFind for recursive file collection. */
					GAIA::TCH szFind[MAXPL];
					GAIA::ALGO::gstrcpy(szFind, pszName);
					GAIA::TCH* p = GAIA::ALGO::gstrend(szFind);
					--p;
					if(*p != '/')
						GAIA::ALGO::gstrcat(p, _T("/"));
					/* find */
				#if GAIA_CHARSET == GAIA_CHARSET_ANSI
					DIR* pdir = opendir(pszName);
				#elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempName[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszName, GINVALID, szTempName, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempName[sLen] = '\0';
					DIR* pdir = opendir(szTempName);
				#endif
					if(pdir == GNIL)
						return GAIA::False;
					dirent* pdirent;
					while((pdirent = readdir(pdir)) != GNIL)
					{
						if(GAIA::ALGO::gstrcmp(pdirent->d_name, _T(".")) == 0 ||
							GAIA::ALGO::gstrcmp(pdirent->d_name, _T("..")) == 0){}
						else
						{
							struct stat s;
							GAIA::TCH sz[MAXPL];
							GAIA::ALGO::gstrcpy(sz, szFind);
							GAIA::ALGO::gstrcat(sz, pdirent->d_name);
						#if GAIA_CHARSET == GAIA_CHARSET_ANSI
							if(lstat(sz, &s) >= 0)
						#elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
							GAIA::CH szTemp[GAIA::MAXPL];
							GAIA::NUM sLen = GAIA::LOCALE::w2m(sz, GINVALID, szTemp, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
							szTemp[sLen] = '\0';
							if(lstat(szTemp, &s) >= 0)
						#endif
							{
								if(S_ISDIR(s.st_mode))
								{
									if(!this->Remove(sz, bOverlapped))
									{
										closedir(pdir);
										return GAIA::False;
									}
								}
								else
								{
									if(!this->RemoveFile(sz))
									{
										closedir(pdir);
										return GAIA::False;
									}
								}
							}
						}
					}
					closedir(pdir);
					return this->Remove(pszName, GAIA::False);
				}
				else
				{
				#if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(rmdir(pszName) == 0)
				#elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempName[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszName, GINVALID, szTempName, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempName[sLen] = '\0';
					if(rmdir(szTempName) == 0)
				#endif
						return GAIA::True;
					return GAIA::False;
				}
			#endif
			}
			GINL virtual GAIA::BL Copy(const GAIA::TCH* pszSrcName, const GAIA::TCH* pszDstName, GAIA::BL bOverlapped)
			{
				if(GAIA::ALGO::gstremp(pszSrcName))
					return GAIA::False;
				if(GAIA::ALGO::gstremp(pszDstName))
					return GAIA::False;
				GAIA::FSYS::FileInfo fi;
				if(!this->GetInfo(pszSrcName, fi))
					return GAIA::False;
				if(!fi.bDirectory)
					return GAIA::False;
				if(!this->Exist(pszDstName))
				{
					if(!this->Create(pszDstName, bOverlapped))
						return GAIA::False;
				}
				if(!this->GetInfo(pszDstName, fi))
					return GAIA::False;
				if(!fi.bDirectory)
					return GAIA::False;
				GAIA::FSYS::FSFinderBase* pFinder = this->Find(pszSrcName);
				if(pFinder == GNIL)
					return GAIA::True;
				GAIA::BL bExistError = GAIA::False;
				while(pFinder != NULL)
				{
					const GAIA::FSYS::FileInfo& fifinded = pFinder->GetInfo();
					GAIA::TCH szTempSrc[GAIA::MAXPL];
					GAIA::TCH szTempDst[GAIA::MAXPL];
					GAIA::ALGO::gstrcpy(szTempSrc, pszSrcName);
					GAIA::ALGO::gstrcpy(szTempDst, pszDstName);
					GAIA::ALGO::gstrcat(szTempSrc, fifinded.pszName);
					GAIA::ALGO::gstrcat(szTempDst, fifinded.pszName);
					if(fifinded.bDirectory)
					{
						if(!this->Copy(szTempSrc, szTempDst, bOverlapped))
						{
							bExistError = GAIA::True;
							break;
						}
					}
					else
					{
						if(!this->CopyFile(szTempSrc, szTempDst))
						{
							bExistError = GAIA::True;
							break;
						}
					}

					if(!this->FindNext(*pFinder))
						break;
				}
				this->FindEnd(*pFinder);
				if(bExistError)
					return GAIA::False;
				return GAIA::True;
			}
			GINL virtual GAIA::BL Move(const GAIA::TCH* pszSrcName, const GAIA::TCH* pszDstName)
			{
				if(GAIA::ALGO::gstremp(pszSrcName))
					return GAIA::False;
				if(GAIA::ALGO::gstremp(pszDstName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(!::MoveFileA(pszSrcName, pszDstName))
						return GAIA::False;
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					if(!::MoveFileW(pszSrcName, pszDstName))
						return GAIA::False;
			#	endif
			#else
				// TODO:
			#endif
				return GAIA::True;
			}
			GINL virtual GAIA::BL Exist(const GAIA::TCH* pszName)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					DWORD dwFileAttribute = ::GetFileAttributesA(pszName);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					DWORD dwFileAttribute = ::GetFileAttributesW(pszName);
			#	endif
				if(dwFileAttribute == INVALID_FILE_ATTRIBUTES)
					return GAIA::False;
				if(dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY)
					return GAIA::True;
				return GAIA::False;
			#else
				struct stat s;
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(lstat(pszName, &s) >= 0)
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempName[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszName, GINVALID, szTempName, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempName[sLen] = '\0';
					if(lstat(szTempName, &s) >= 0)
			#	endif
				{
					if(S_ISDIR(s.st_mode))
						return GAIA::True;
					return GAIA::False;
				}
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL Empty(const GAIA::TCH* pszName, GAIA::BL& bEmpty)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
				GAIA::FSYS::FileInfo fi;
				if(!this->GetInfo(pszName, fi))
					return GAIA::False;
				if(!fi.bDirectory)
					return GAIA::False;
				GAIA::FSYS::FSFinderBase* pFinder = this->Find(pszName);
				if(pFinder == GNIL)
					return GAIA::True;
				this->FindEnd(*pFinder);
				return GAIA::False;
			}
			GINL virtual GAIA::BL RemoveFile(const GAIA::TCH* pszName)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(::DeleteFileA(pszName))
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					if(::DeleteFileW(pszName))
			#	endif
					return GAIA::True;
				return GAIA::False;
			#else
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(unlink(pszName) == 0)
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempName[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszName, GINVALID, szTempName, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempName[sLen] = '\0';
					if(unlink(szTempName) == 0)
			#	endif
					return GAIA::True;
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL CopyFile(const GAIA::TCH* pszSrcName, const GAIA::TCH* pszDstName)
			{
				if(GAIA::ALGO::gstremp(pszSrcName))
					return GAIA::False;
				if(GAIA::ALGO::gstremp(pszDstName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(::CopyFileA(pszSrcName, pszDstName, GAIA::False))
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					if(::CopyFileW(pszSrcName, pszDstName, GAIA::False))
			#	endif
					return GAIA::True;
				return GAIA::False;
			#else
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(link(pszSrcName, pszDstName) == 0)
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempSrc[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszSrcName, GINVALID, szTempSrc, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempSrc[sLen] = '\0';
					GAIA::CH szTempDst[GAIA::MAXPL];
					sLen = GAIA::LOCALE::w2m(pszDstName, GINVALID, szTempDst, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempDst[sLen] = '\0';
					if(link(szTempSrc, szTempDst) == 0)
			#	endif
					return GAIA::True;
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL MoveFile(const GAIA::TCH* pszSrcName, const GAIA::TCH* pszDstName)
			{
				if(GAIA::ALGO::gstremp(pszSrcName))
					return GAIA::False;
				if(GAIA::ALGO::gstremp(pszDstName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(::MoveFileA(pszSrcName, pszDstName))
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					if(::MoveFileW(pszSrcName, pszDstName))
			#	endif
					return GAIA::True;
				return GAIA::False;
			#else
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(rename(pszSrcName, pszDstName) == 0)
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempSrc[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszSrcName, GINVALID, szTempSrc, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempSrc[sLen] = '\0';
					GAIA::CH szTempDst[GAIA::MAXPL];
					sLen = GAIA::LOCALE::w2m(pszDstName, GINVALID, szTempDst, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempDst[sLen] = '\0';
					if(rename(szTempSrc, szTempDst) == 0)
			#	endif
					return GAIA::True;
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL ExistFile(const GAIA::TCH* pszName)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					GAIA::UM uFlag = ::GetFileAttributesA(pszName);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::UM uFlag = ::GetFileAttributesW(pszName);
			#	endif
				if(uFlag == GINVALID)
					return GAIA::False;
				else if(uFlag & FILE_ATTRIBUTE_DIRECTORY)
					return GAIA::False;
				return GAIA::True;
			#else
				struct stat s;
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					if(lstat(pszName, &s) >= 0)
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempName[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszName, GINVALID, szTempName, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempName[sLen] = '\0';
					if(lstat(szTempName, &s) >= 0)
			#	endif
				{
					if(S_ISDIR(s.st_mode))
						return GAIA::False;
					return GAIA::True;
				}
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL GetInfo(const GAIA::TCH* pszName, FileInfo& fi)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					DWORD dwFileAttr = ::GetFileAttributesA(pszName);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					DWORD dwFileAttr = ::GetFileAttributesW(pszName);
			#	endif
				if(dwFileAttr == INVALID_FILE_ATTRIBUTES)
					return GAIA::False;
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					HANDLE hFile = ::CreateFileA(pszName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, GNIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, GNIL);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					HANDLE hFile = ::CreateFileW(pszName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, GNIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, GNIL);
			#	endif
				if(hFile == INVALID_HANDLE_VALUE)
					return GAIA::False;
				fi.pszName = pszName;
				fi.bDirectory = dwFileAttr & FILE_ATTRIBUTE_DIRECTORY;
				FILETIME tCreateTime, tLastWriteTime, tLastReadTime;
				if(!::GetFileTime(hFile, &tCreateTime, &tLastReadTime, &tLastWriteTime))
					return GAIA::False;
				if(!::CloseHandle(hFile))
					return GAIA::False;
				GAIA::TIME::FileTimeToTime(tCreateTime, fi.uCreateTime);
				GAIA::TIME::FileTimeToTime(tLastReadTime, fi.uLastReadTime);
				GAIA::TIME::FileTimeToTime(tLastWriteTime, fi.uLastWriteTime);
			#else
				// TODO:
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
			#	endif
			#endif
				return GAIA::True;
			}
			GINL virtual GAIA::FSYS::FSFinderBase* Find(const GAIA::TCH* pszDirName)
			{
				if(GAIA::ALGO::gstremp(pszDirName))
					return GNIL;
				FSFinder* pRet = gnew FSFinder;
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::TCH szFind[GAIA::MAXPL];
				GAIA::ALGO::gstrcpy(szFind, pszDirName);
				GAIA::TCH* p = GAIA::ALGO::gstrend(szFind);
				--p;
				if(*p != '\\' && *p != '/')
					GAIA::ALGO::gstrcat(p, _T("/"));

				GAIA::TCH szTarget[GAIA::MAXPL];
				GAIA::ALGO::gstrcpy(szTarget, szFind);
				GAIA::ALGO::gstrcat(szTarget + (p - szFind), _T("*.*"));

			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					pRet->hFF = ::FindFirstFileA(szTarget, &pRet->fdata);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					pRet->hFF = ::FindFirstFileW(szTarget, &pRet->fdata);
			#	endif

				while(GAIA::ALGO::gstrcmp(pRet->fdata.cFileName, _T(".")) == 0 ||
					GAIA::ALGO::gstrcmp(pRet->fdata.cFileName, _T("..")) == 0)
				{
					if(!::FindNextFileW(pRet->hFF, &pRet->fdata))
					{
						::FindClose(pRet->hFF);
						gdel pRet;
						return GNIL;
					}
				}

				if(pRet->hFF == (HANDLE)GINVALID)
				{
					gdel pRet;
					return GNIL;
				}

				pRet->UpdateFileInfo();
			#else
				// TODO:
			#endif
				return pRet;
			}
			GINL virtual GAIA::BL FindNext(GAIA::FSYS::FSFinderBase& df)
			{
				FSFinder& f = (FSFinder&)df;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(!::FindNextFile(f.hFF, &f.fdata))
					return GAIA::False;
				f.UpdateFileInfo();
				return GAIA::True;
			#else
				// TODO:
				f.UpdateFileInfo();
				return GAIA::True;
			#endif
			}
			GINL virtual GAIA::BL FindEnd(GAIA::FSYS::FSFinderBase& df)
			{
				FSFinder& f = (FSFinder&)df;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(f.hFF == (HANDLE)GINVALID)
					return GAIA::False;
				if(!::FindClose(f.hFF))
				{
					gdel &f;
					return GAIA::False;
				}
				gdel &f;
				return GAIA::True;
			#else
				// TODO:
				gdel &f;
				return GAIA::True;
			#endif
			}
			GINL virtual GAIA::BL CollectFile(const GAIA::TCH* pszName, const GAIA::TCH* pszFilter, GAIA::BL bOverlapped, __ResultTree& treeResult)
			{
				GAST(!GAIA::ALGO::gstremp(pszName));
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				/* Generate szFind for recursive file collection. */
				GAIA::TCH szFind[MAXPL];
				GAIA::ALGO::gstrcpy(szFind, pszName);
				GAIA::TCH* p = GAIA::ALGO::gstrend(szFind);
				--p;
				if(*p != '\\' && *p != '/')
					GAIA::ALGO::gstrcat(p, _T("/"));
				/* Generate szTarget for FindFirstFile. */
				GAIA::TCH szTarget[MAXPL];
				GAIA::ALGO::gstrcpy(szTarget, szFind);
				GAIA::ALGO::gstrcat(szTarget + (p - szFind), _T("*.*"));
				/* Find. */
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					WIN32_FIND_DATAA fdata;
					HANDLE hFF = ::FindFirstFileA(szTarget, &fdata);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					WIN32_FIND_DATAW fdata;
					HANDLE hFF = ::FindFirstFileW(szTarget, &fdata);
			#	endif
				if(hFF == (HANDLE)GINVALID)
					return GAIA::False;
				GAIA::BL bFinded = GAIA::True;
				while(bFinded)
				{
					if(GAIA::ALGO::gstrcmp(fdata.cFileName, _T(".")) == 0 ||
						GAIA::ALGO::gstrcmp(fdata.cFileName, _T("..")) == 0){}
					else if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && bOverlapped)
					{
						GAIA::TCH sz[MAXPL];
						GAIA::ALGO::gstrcpy(sz, szFind);
						GAIA::ALGO::gstrcat(sz, fdata.cFileName);
						this->CollectFile(sz, pszFilter, bOverlapped, treeResult);
					}
					else
					{
						GAIA::BL bExtMatch = GAIA::False;
						if(GAIA::ALGO::gstremp(pszFilter))
							bExtMatch = GAIA::True;
						else
						{
							const GAIA::TCH* pExt = GAIA::ALGO::gstrext(fdata.cFileName);
							if(pExt != GNIL && GAIA::ALGO::gstriwrd(pszFilter, pExt) != GNIL)
								bExtMatch = GAIA::True;
						}
						if(bExtMatch)
						{
							GAIA::TCH szFinal[MAXPL];
							GAIA::ALGO::gstrcpy(szFinal, szFind);
							GAIA::ALGO::gstrcat(szFinal, fdata.cFileName);
							GAIA::CTN::BasicVector<GAIA::CTN::TString, GAIA::N16, GAIA::ALGO::ExtendGold<GAIA::N32> > listResult;
							const GAIA::TCH* pFinal = szFinal;
							for(;;)
							{
								const GAIA::TCH* pNew = GAIA::ALGO::gstrdrop(pFinal, _T("/\\\0"));
								if(pNew != pFinal)
								{
									if(pNew == GNIL || *pNew == 0)
									{
										GAIA::TCH szTemp[MAXPL];
										GAIA::ALGO::gstrcpy(szTemp, pFinal);
										if(szTemp[0] != 0)
											listResult.push_back(szTemp);
										break;
									}
									else
									{
										GAIA::TCH szTemp[MAXPL];
										GAIA::ALGO::gmemcpy(szTemp, pFinal, (pNew - pFinal) * sizeof(GAIA::TCH));
										szTemp[pNew - pFinal] = 0;
										if(szTemp[0] != 0)
											listResult.push_back(szTemp);
									}
								}
								pFinal = pNew + 1;
							}
							treeResult.insert(listResult.fptr(), listResult.size());
						}
					}
				#if GAIA_CHARSET == GAIA_CHARSET_ANSI
					bFinded = ::FindNextFileA(hFF, &fdata) != 0;
				#elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					bFinded = ::FindNextFileW(hFF, &fdata) != 0;
				#endif
				}
				::FindClose(hFF);
				return GAIA::True;
			#else
				/* Generate szFind for recursive file collection. */
				GAIA::TCH szFind[MAXPL];
				GAIA::ALGO::gstrcpy(szFind, pszName);
				GAIA::TCH* p = GAIA::ALGO::gstrend(szFind);
				--p;
				if(*p != '/')
					GAIA::ALGO::gstrcat(p, _T("/"));
				/* find */
			#	if GAIA_CHARSET == GAIA_CHARSET_ANSI
					DIR* pdir = opendir(pszName);
			#	elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
					GAIA::CH szTempName[GAIA::MAXPL];
					GAIA::NUM sLen = GAIA::LOCALE::w2m(pszName, GINVALID, szTempName, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
					szTempName[sLen] = '\0';
					DIR* pdir = opendir(szTempName);
			#	endif
				if(pdir == GNIL)
					return GAIA::False;
				dirent* pdirent;
				while((pdirent = readdir(pdir)) != GNIL)
				{
					if(GAIA::ALGO::gstrcmp(pdirent->d_name, _T(".")) == 0 ||
						GAIA::ALGO::gstrcmp(pdirent->d_name, _T("..")) == 0){}
					else
					{
						struct stat s;
						GAIA::TCH sz[MAXPL];
						GAIA::ALGO::gstrcpy(sz, szFind);
						GAIA::ALGO::gstrcat(sz, pdirent->d_name);
					#if GAIA_CHARSET == GAIA_CHARSET_ANSI
						if(lstat(sz, &s) >= 0)
					#elif GAIA_CHARSET == GAIA_CHARSET_UNICODE
						GAIA::CH szTemp[GAIA::MAXPL];
						GAIA::NUM sLen = GAIA::LOCALE::w2m(sz, GINVALID, szTemp, GAIA::MAXPL, GAIA::CHARSET_TYPE_ASCII);
						szTemp[sLen] = '\0';
						if(lstat(szTemp, &s) >= 0)
					#endif
						{
							if(S_ISDIR(s.st_mode))
							{
								if(bOverlapped)
									this->CollectFile(sz, pszFilter, bOverlapped, treeResult);
							}
							else
							{
								GAIA::BL bExtMatch = GAIA::False;
								if(GAIA::ALGO::gstremp(pszFilter))
									bExtMatch = GAIA::True;
								else
								{
									const GAIA::CH* pExt = GAIA::ALGO::gstrext(pdirent->d_name);
									if(pExt != GNIL && GAIA::ALGO::gstriwrd(pszFilter, pExt) != GNIL)
										bExtMatch = GAIA::True;
								}
								if(bExtMatch)
								{
									GAIA::CTN::BasicVector<GAIA::CTN::TString, GAIA::N16, GAIA::ALGO::ExtendGold<GAIA::N32> > listResult;
									const GAIA::TCH* pFinal = sz;
									for(;;)
									{
										const GAIA::TCH* pNew = GAIA::ALGO::gstrdrop(pFinal, _T("/\\\0"));
										if(pNew != pFinal)
										{
											if(pNew == GNIL || *pNew == 0)
											{
												GAIA::TCH szTemp[MAXPL];
												GAIA::ALGO::gstrcpy(szTemp, pFinal);
												if(szTemp[0] != 0)
													listResult.push_back(szTemp);
												break;
											}
											else
											{
												GAIA::TCH szTemp[MAXPL];
												GAIA::ALGO::gmemcpy(szTemp, pFinal, (pNew - pFinal) * sizeof(GAIA::TCH));
												szTemp[pNew - pFinal] = 0;
												if(szTemp[0] != 0)
													listResult.push_back(szTemp);
											}
										}
										pFinal = pNew + 1;
									}
									treeResult.insert(listResult.fptr(), listResult.size());
								}
							}
						}
					}
				}
				closedir(pdir);
				return GAIA::True;
			#endif
			}
		};
	}
}

#endif
