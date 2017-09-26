#ifndef		__GAIA_FSYS_DIR_H__
#define		__GAIA_FSYS_DIR_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"
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
			virtual const FileDesc& GetDesc() const
			{
				return fdesc;
			}
			GINL GAIA::GVOID UpdateFileInfo()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAST(hFF != (HANDLE)GINVALID);
				if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					GAIA::CH* pEnd = GAIA::ALGO::gstrend(fdata.cFileName);
					GAST(pEnd - fdata.cFileName < GAIA::MAXPL - 1);
					if(pEnd[-1] != '/' && pEnd[-1] != '\\')
					{
						pEnd[0] = '/';
						pEnd[1] = '\0';
					}
				}
				fdesc.pszName = fdata.cFileName;
				fdesc.bDirectory = (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
				GAIA::TIME::FileTimeToTime(fdata.ftCreationTime, fdesc.uCreateTime);
				GAIA::TIME::FileTimeToTime(fdata.ftLastAccessTime, fdesc.uLastReadTime);
				GAIA::TIME::FileTimeToTime(fdata.ftLastWriteTime, fdesc.uLastWriteTime);
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
				fdesc.reset();
		#else
		#endif
			}
		private:
		#if GAIA_OS == GAIA_OS_WINDOWS
			HANDLE hFF;
			WIN32_FIND_DATAA fdata;
		#else
			// TODO:
		#endif
			FileDesc fdesc;
		};

		class Dir : public GAIA::FSYS::DirBase
		{
		public:
			GINL virtual GAIA::GVOID GetBinaryDir(GAIA::CTN::AString& result)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::CH szPath[MAXPL];
				::GetModuleFileNameA(GNIL, szPath, MAXPL);
				GAIA::CH* p = GAIA::ALGO::gstrdropr(szPath, _T("/\\"));
				result = szPath;
			#else
				// TODO:
			#endif
			}
			GINL virtual GAIA::BL SetWorkingDir(const GAIA::CH* pszPath)
			{
				if(GAIA::ALGO::gstremp(pszPath))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(::SetCurrentDirectoryA(pszPath))
			#else
				if(chdir(pszPath) == 0)
			#endif
					return GAIA::True;
				return GAIA::False;
			}
			GINL virtual GAIA::GVOID GetWorkingDir(GAIA::CTN::AString& result)
			{
				GAIA::CH szPath[MAXPL];
			#if GAIA_OS == GAIA_OS_WINDOWS
				::GetCurrentDirectoryA(MAXPL, szPath);
			#else
				::getcwd(szPath, MAXPL);
			#endif
				result = szPath;
			}
			GINL virtual GAIA::BL Create(const GAIA::CH* pszName, GAIA::BL bOverlapped)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
				if(this->Exist(pszName))
					return GAIA::False;
				/* Generate szFind for recursive file collection. */
				GAIA::CH szFind[MAXPL];
				GAIA::ALGO::gstrcpy(szFind, pszName);
				GAIA::CH* p = GAIA::ALGO::gstrend(szFind);
				--p;
				if(*p != '\\' && *p != '/')
					GAIA::ALGO::gstrcat(p, _T("/"));
				if(bOverlapped)
				{
					const GAIA::CH* pCursor = szFind;
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
						GAIA::CH sz[MAXPL];
						GAIA::ALGO::gmemcpy(sz, szFind, (pCursor - szFind + 1) * sizeof(GAIA::CH));
						sz[pCursor - szFind + 1] = 0;
						if(!this->Exist(sz))
						{
						#if GAIA_OS == GAIA_OS_WINDOWS
							if(!::CreateDirectoryA(sz, GNIL))
						#else
							if(mkdir(sz, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
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
					if(::CreateDirectoryA(pszName, GNIL))
				#else
					if(mkdir(pszName, S_IRWXU | S_IRWXG | S_IRWXO) == 0)
				#endif
						return GAIA::True;
					return GAIA::False;
				}
			}
			GINL virtual GAIA::BL Remove(const GAIA::CH* pszName, GAIA::BL bOverlapped)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(bOverlapped)
				{
					/* Generate szFind for recursive file collection. */
					GAIA::CH szFind[MAXPL];
					GAIA::ALGO::gstrcpy(szFind, pszName);
					GAIA::CH* p = GAIA::ALGO::gstrend(szFind);
					--p;
					if(*p != '\\' && *p != '/')
						GAIA::ALGO::gstrcat(p, _T("/"));
					/* Generate szTarget for FindFirstFile. */
					GAIA::CH szTarget[MAXPL];
					GAIA::ALGO::gstrcpy(szTarget, szFind);
					GAIA::ALGO::gstrcat(szTarget + (p - szFind), _T("*.*"));
					WIN32_FIND_DATAA fdata;
					HANDLE hFF = ::FindFirstFileA(szTarget, &fdata);
					if(hFF == (HANDLE)GINVALID)
						return GAIA::False;
					GAIA::BL bFinded = GAIA::True;
					while(bFinded)
					{
						if(GAIA::ALGO::gstrcmp(fdata.cFileName, _T(".")) == 0 ||
							GAIA::ALGO::gstrcmp(fdata.cFileName, _T("..")) == 0){}
						else
						{
							GAIA::CH sz[MAXPL];
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
						bFinded = ::FindNextFileA(hFF, &fdata) != 0;
					}
					::FindClose(hFF);
					return this->Remove(pszName, GAIA::False);
				}
				else
				{
					if(::RemoveDirectoryA(pszName))
						return GAIA::True;
					return GAIA::False;
				}
			#else
				if(bOverlapped)
				{
					/* Generate szFind for recursive file collection. */
					GAIA::CH szFind[MAXPL];
					GAIA::ALGO::gstrcpy(szFind, pszName);
					GAIA::CH* p = GAIA::ALGO::gstrend(szFind);
					--p;
					if(*p != '/')
						GAIA::ALGO::gstrcat(p, _T("/"));
					/* find */
					DIR* pdir = opendir(pszName);
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
							GAIA::CH sz[MAXPL];
							GAIA::ALGO::gstrcpy(sz, szFind);
							GAIA::ALGO::gstrcat(sz, pdirent->d_name);
							if(lstat(sz, &s) >= 0)
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
					if(rmdir(pszName) == 0)
						return GAIA::True;
					return GAIA::False;
				}
			#endif
			}
			GINL virtual GAIA::BL Copy(const GAIA::CH* pszSrcName, const GAIA::CH* pszDstName, GAIA::BL bOverlapped)
			{
				if(GAIA::ALGO::gstremp(pszSrcName))
					return GAIA::False;
				if(GAIA::ALGO::gstremp(pszDstName))
					return GAIA::False;
				GAIA::FSYS::FileDesc fdesc;
				if(!this->GetDesc(pszSrcName, fdesc))
					return GAIA::False;
				if(!fdesc.bDirectory)
					return GAIA::False;
				if(!this->Exist(pszDstName))
				{
					if(!this->Create(pszDstName, bOverlapped))
						return GAIA::False;
				}
				if(!this->GetDesc(pszDstName, fdesc))
					return GAIA::False;
				if(!fdesc.bDirectory)
					return GAIA::False;
				GAIA::FSYS::FSFinderBase* pFinder = this->Find(pszSrcName);
				if(pFinder == GNIL)
					return GAIA::True;
				GAIA::BL bExistError = GAIA::False;
				while(pFinder != NULL)
				{
					const GAIA::FSYS::FileDesc& fifinded = pFinder->GetDesc();
					GAIA::CH szTempSrc[GAIA::MAXPL];
					GAIA::CH szTempDst[GAIA::MAXPL];
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
			GINL virtual GAIA::BL Move(const GAIA::CH* pszSrcName, const GAIA::CH* pszDstName)
			{
				if(GAIA::ALGO::gstremp(pszSrcName))
					return GAIA::False;
				if(GAIA::ALGO::gstremp(pszDstName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(!::MoveFileA(pszSrcName, pszDstName))
					return GAIA::False;
			#else
				// TODO:
			#endif
				return GAIA::True;
			}
			GINL virtual GAIA::BL Exist(const GAIA::CH* pszName)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				DWORD dwFileAttribute = ::GetFileAttributesA(pszName);
				if(dwFileAttribute == INVALID_FILE_ATTRIBUTES)
					return GAIA::False;
				if(dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY)
					return GAIA::True;
				return GAIA::False;
			#else
				struct stat s;
				if(lstat(pszName, &s) >= 0)
				{
					if(S_ISDIR(s.st_mode))
						return GAIA::True;
					return GAIA::False;
				}
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL Empty(const GAIA::CH* pszName, GAIA::BL& bEmpty)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
				GAIA::FSYS::FileDesc fdesc;
				if(!this->GetDesc(pszName, fdesc))
					return GAIA::False;
				if(!fdesc.bDirectory)
					return GAIA::False;
				GAIA::FSYS::FSFinderBase* pFinder = this->Find(pszName);
				if(pFinder == GNIL)
					return GAIA::True;
				this->FindEnd(*pFinder);
				return GAIA::False;
			}
			GINL virtual GAIA::BL RemoveFile(const GAIA::CH* pszName)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(::DeleteFileA(pszName))
					return GAIA::True;
				return GAIA::False;
			#else
				if(unlink(pszName) == 0)
					return GAIA::True;
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL CopyFile(const GAIA::CH* pszSrcName, const GAIA::CH* pszDstName)
			{
				if(GAIA::ALGO::gstremp(pszSrcName))
					return GAIA::False;
				if(GAIA::ALGO::gstremp(pszDstName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(::CopyFileA(pszSrcName, pszDstName, GAIA::False))
					return GAIA::True;
				return GAIA::False;
			#else
				if(link(pszSrcName, pszDstName) == 0)
					return GAIA::True;
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL MoveFile(const GAIA::CH* pszSrcName, const GAIA::CH* pszDstName)
			{
				if(GAIA::ALGO::gstremp(pszSrcName))
					return GAIA::False;
				if(GAIA::ALGO::gstremp(pszDstName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(::MoveFileA(pszSrcName, pszDstName))
					return GAIA::True;
				return GAIA::False;
			#else
				if(rename(pszSrcName, pszDstName) == 0)
					return GAIA::True;
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL ExistFile(const GAIA::CH* pszName)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::UM uFlag = ::GetFileAttributesA(pszName);
				if(uFlag == GINVALID)
					return GAIA::False;
				else if(uFlag & FILE_ATTRIBUTE_DIRECTORY)
					return GAIA::False;
				return GAIA::True;
			#else
				struct stat s;
				if(lstat(pszName, &s) >= 0)
				{
					if(S_ISDIR(s.st_mode))
						return GAIA::False;
					return GAIA::True;
				}
				return GAIA::False;
			#endif
			}
			GINL virtual GAIA::BL GetDesc(const GAIA::CH* pszName, FileDesc& fdesc)
			{
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				DWORD dwFileAttr = ::GetFileAttributesA(pszName);
				if(dwFileAttr == INVALID_FILE_ATTRIBUTES)
					return GAIA::False;
				HANDLE hFile = ::CreateFileA(pszName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, GNIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, GNIL);
				if(hFile == INVALID_HANDLE_VALUE)
					return GAIA::False;
				fdesc.pszName = pszName;
				fdesc.bDirectory = dwFileAttr & FILE_ATTRIBUTE_DIRECTORY;
				FILETIME tCreateTime, tLastWriteTime, tLastReadTime;
				if(!::GetFileTime(hFile, &tCreateTime, &tLastReadTime, &tLastWriteTime))
					return GAIA::False;
				if(!::CloseHandle(hFile))
					return GAIA::False;
				GAIA::TIME::FileTimeToTime(tCreateTime, fdesc.uCreateTime);
				GAIA::TIME::FileTimeToTime(tLastReadTime, fdesc.uLastReadTime);
				GAIA::TIME::FileTimeToTime(tLastWriteTime, fdesc.uLastWriteTime);
			#else
				// TODO:
			#endif
				return GAIA::True;
			}
			GINL virtual GAIA::FSYS::FSFinderBase* Find(const GAIA::CH* pszDirName)
			{
				if(GAIA::ALGO::gstremp(pszDirName))
					return GNIL;
				FSFinder* pRet = gnew FSFinder;
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::CH szFind[GAIA::MAXPL];
				GAIA::ALGO::gstrcpy(szFind, pszDirName);
				GAIA::CH* p = GAIA::ALGO::gstrend(szFind);
				--p;
				if(*p != '\\' && *p != '/')
					GAIA::ALGO::gstrcat(p, _T("/"));

				GAIA::CH szTarget[GAIA::MAXPL];
				GAIA::ALGO::gstrcpy(szTarget, szFind);
				GAIA::ALGO::gstrcat(szTarget + (p - szFind), _T("*.*"));

				pRet->hFF = ::FindFirstFileA(szTarget, &pRet->fdata);

				while(GAIA::ALGO::gstrcmp(pRet->fdata.cFileName, _T(".")) == 0 ||
					GAIA::ALGO::gstrcmp(pRet->fdata.cFileName, _T("..")) == 0)
				{
					if(!::FindNextFileA(pRet->hFF, &pRet->fdata))
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
				if(!::FindNextFileA(f.hFF, &f.fdata))
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
			GINL virtual GAIA::BL CollectFile(const GAIA::CH* pszName, const GAIA::CH* pszFilter, GAIA::BL bOverlapped, __ResultTree& treeResult)
			{
				GAST(!GAIA::ALGO::gstremp(pszName));
				if(GAIA::ALGO::gstremp(pszName))
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				/* Generate szFind for recursive file collection. */
				GAIA::CH szFind[MAXPL];
				GAIA::ALGO::gstrcpy(szFind, pszName);
				GAIA::CH* p = GAIA::ALGO::gstrend(szFind);
				--p;
				if(*p != '\\' && *p != '/')
					GAIA::ALGO::gstrcat(p, _T("/"));
				/* Generate szTarget for FindFirstFile. */
				GAIA::CH szTarget[MAXPL];
				GAIA::ALGO::gstrcpy(szTarget, szFind);
				GAIA::ALGO::gstrcat(szTarget + (p - szFind), _T("*.*"));
				/* Find. */
				WIN32_FIND_DATAA fdata;
				HANDLE hFF = ::FindFirstFileA(szTarget, &fdata);
				if(hFF == (HANDLE)GINVALID)
					return GAIA::False;
				GAIA::BL bFinded = GAIA::True;
				while(bFinded)
				{
					if(GAIA::ALGO::gstrcmp(fdata.cFileName, _T(".")) == 0 ||
						GAIA::ALGO::gstrcmp(fdata.cFileName, _T("..")) == 0){}
					else if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && bOverlapped)
					{
						GAIA::CH sz[MAXPL];
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
							const GAIA::CH* pExt = GAIA::ALGO::gstrext(fdata.cFileName);
							if(pExt != GNIL && GAIA::ALGO::gstriwrd(pszFilter, pExt) != GNIL)
								bExtMatch = GAIA::True;
						}
						if(bExtMatch)
						{
							GAIA::CH szFinal[MAXPL];
							GAIA::ALGO::gstrcpy(szFinal, szFind);
							GAIA::ALGO::gstrcat(szFinal, fdata.cFileName);
							GAIA::CTN::BasicVector<GAIA::CTN::AString, GAIA::N16, GAIA::ALGO::ExtendGold<GAIA::N32> > listResult;
							const GAIA::CH* pFinal = szFinal;
							for(;;)
							{
								const GAIA::CH* pNew = GAIA::ALGO::gstrdrop(pFinal, _T("/\\\0"));
								if(pNew != pFinal)
								{
									if(pNew == GNIL || *pNew == 0)
									{
										GAIA::CH szTemp[MAXPL];
										GAIA::ALGO::gstrcpy(szTemp, pFinal);
										if(szTemp[0] != 0)
											listResult.push_back(szTemp);
										break;
									}
									else
									{
										GAIA::CH szTemp[MAXPL];
										GAIA::ALGO::gmemcpy(szTemp, pFinal, (pNew - pFinal) * sizeof(GAIA::CH));
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
				bFinded = ::FindNextFileA(hFF, &fdata) != 0;
				}
				::FindClose(hFF);
				return GAIA::True;
			#else
				/* Generate szFind for recursive file collection. */
				GAIA::CH szFind[MAXPL];
				GAIA::ALGO::gstrcpy(szFind, pszName);
				GAIA::CH* p = GAIA::ALGO::gstrend(szFind);
				--p;
				if(*p != '/')
					GAIA::ALGO::gstrcat(p, _T("/"));
				/* find */
				DIR* pdir = opendir(pszName);
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
						GAIA::CH sz[MAXPL];
						GAIA::ALGO::gstrcpy(sz, szFind);
						GAIA::ALGO::gstrcat(sz, pdirent->d_name);
						if(lstat(sz, &s) >= 0)
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
									GAIA::CTN::BasicVector<GAIA::CTN::AString, GAIA::N16, GAIA::ALGO::ExtendGold<GAIA::N32> > listResult;
									const GAIA::CH* pFinal = sz;
									for(;;)
									{
										const GAIA::CH* pNew = GAIA::ALGO::gstrdrop(pFinal, _T("/\\\0"));
										if(pNew != pFinal)
										{
											if(pNew == GNIL || *pNew == 0)
											{
												GAIA::CH szTemp[MAXPL];
												GAIA::ALGO::gstrcpy(szTemp, pFinal);
												if(szTemp[0] != 0)
													listResult.push_back(szTemp);
												break;
											}
											else
											{
												GAIA::CH szTemp[MAXPL];
												GAIA::ALGO::gmemcpy(szTemp, pFinal, (pNew - pFinal) * sizeof(GAIA::CH));
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
