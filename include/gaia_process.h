#ifndef		__GAIA_PROCESS_H__
#define		__GAIA_PROCESS_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_string.h"
#include "gaia_ctn_vector.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#	include <psapi.h>
#else
#	include <unistd.h>
#endif

namespace GAIA
{
	namespace PROCESS
	{
		class ProcessInfo : public GAIA::Base
		{
		public:
			GAIA::UM uProcessID;
			GAIA::CTN::TString strProcessName;
			GAIA::CTN::TString strProcessUrl;
			GAIA::CTN::Vector<GAIA::CTN::TString> listDynamicLib;
		};

		GINL GAIA::UM processid()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			return (GAIA::UM)::GetCurrentProcessId();
		#else
			return (GAIA::UM)getpid();
		#endif
		}

		GINL GAIA::NUM GetProcessList(GAIA::CTN::Vector<ProcessInfo>& res, GAIA::BL bCollectDynamicLibrary = GAIA::False)
		{
			res.clear();
		#if GAIA_OS == GAIA_OS_WINDOWS
			DWORD pidlist[1024];
			DWORD uProcessCount;
			if(!EnumProcesses(pidlist, sizeof(pidlist), &uProcessCount))
				return GINVALID;
			uProcessCount = uProcessCount / sizeof(DWORD);
			if(res.capacity() < (GAIA::NUM)uProcessCount)
				res.reserve(uProcessCount);
			for(GAIA::UM x = 0; x < uProcessCount; ++x)
			{
				DWORD pid = pidlist[x];
				if(pid == 0)
					continue;
				HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, GAIA::False, pid);
				if(hProcess == GNIL)
					continue;
				HMODULE mods[1024];
				DWORD uModCount;
				if(EnumProcessModulesEx(hProcess, mods, sizeof(mods), &uModCount, LIST_MODULES_ALL))
				{
					uModCount = uModCount / sizeof(DWORD);

					ProcessInfo pi;

					GAIA::TCH szTemp[MAX_PATH];
					if(GetModuleBaseName(hProcess, mods[0], szTemp, sizeofarray(szTemp)) != 0)
					{
						pi.strProcessName = szTemp;
						pi.strProcessName.replace('\\', '/');
					}

					for(GAIA::UM y = 0; y < uModCount; ++y)
					{
						if(GetModuleFileNameEx(hProcess, mods[y], szTemp, sizeofarray(szTemp)) != 0)
						{
							if(y == 0)
							{
								pi.strProcessUrl = szTemp;
								pi.strProcessUrl.replace('\\', '/');
								if(bCollectDynamicLibrary)
									pi.listDynamicLib.push_back(pi.strProcessUrl);
							}
							else
							{
								if(bCollectDynamicLibrary)
								{
									pi.listDynamicLib.push_back(szTemp);
									pi.listDynamicLib.back().replace('\\', '/');
								}
							}
						}
						if(!bCollectDynamicLibrary)
							break;
					}

					if(!pi.strProcessName.empty())
					{
						pi.uProcessID = pid;
						res.push_back(pi);
					}
				}
				::CloseHandle(hProcess);
			}
			return res.size();
		#else
			return GINVALID;
		#endif
		}
		class Process : public GAIA::Base
		{
		public:
			GINL static Process CurrentProcess();
			GINL Process()
			{
				m_uProcessID = 0;
			}
			GINL Process(const GAIA::CH* pszFileName)
			{
				m_uProcessID = 0;
				m_strBinaryFileName = pszFileName;
			}
			GINL Process(GAIA::UM uProcessID)
			{
				m_uProcessID = uProcessID;
			}
			GINL ~Process()
			{
			}
			GINL GAIA::UM GetID() const
			{
				return m_uProcessID;
			}
			GINL const GAIA::CH* GetBinaryFileName() const
			{
				return m_strBinaryFileName.fptr();
			}
			GINL GAIA::BL Run()
			{
				if(m_strBinaryFileName.empty())
					return GAIA::False;
			#if GAIA_OS == GAIA_OS_WINDOWS
				STARTUPINFOA si;
				PROCESS_INFORMATION pi;
				zeromem(&si);
				zeromem(&pi);
				GAIA::CTN::AString strTempBinaryFileName = m_strBinaryFileName;
				strTempBinaryFileName.replace('/', '\\');
				if(!::CreateProcessA(strTempBinaryFileName.fptr(), GNIL, GNIL, GNIL, False, CREATE_NEW_CONSOLE, GNIL, GNIL, &si, &pi))
					return GAIA::False;
				m_uProcessID = pi.dwProcessId;
				if(si.hStdError != INVALID_HANDLE_VALUE && si.hStdError != GNIL)
					::CloseHandle(si.hStdError);
				if(si.hStdInput != INVALID_HANDLE_VALUE && si.hStdInput != GNIL)
					::CloseHandle(si.hStdInput);
				if(si.hStdOutput != INVALID_HANDLE_VALUE && si.hStdOutput != GNIL)
					::CloseHandle(si.hStdOutput);
				if(pi.hProcess != INVALID_HANDLE_VALUE && pi.hProcess != GNIL)
					::CloseHandle(pi.hProcess);
				return GAIA::True;
			#else
				return GAIA::True;
			#endif
			}
			GINL GAIA::BL Wait()
			{
				return GAIA::True;
			}
			GINL GAIA::BL Suspend()
			{
				return GAIA::True;
			}
			GINL GAIA::BL Resume()
			{
				return GAIA::True;
			}
			GINL GAIA::BL Terminate()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(m_uProcessID == 0)
					return GAIA::False;
				HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, GAIA::False, m_uProcessID);
				if(hProcess == GNIL)
					return GAIA::False;
				GAIA::BL bResult = ::TerminateProcess(hProcess, 0);
				::CloseHandle(hProcess);
				return bResult;
			#else
				return GAIA::False;
			#endif
			}
		private:
			GAIA::UM m_uProcessID;
			GAIA::CTN::AString m_strBinaryFileName;
		};
		GAIA::PROCESS::Process Process::CurrentProcess()
		{
			Process ret;
			ret.m_uProcessID = GAIA::PROCESS::processid();
			return ret;
		}
	}
}

#endif
