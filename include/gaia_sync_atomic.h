#ifndef 	__GAIA_SYNC_ATOMIC_H__
#define 	__GAIA_SYNC_ATOMIC_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
#	include <libkern/OSAtomic.h>
#else
#	if GAIA_COMPILER != GAIA_COMPILER_GCC || GAIA_COMPILER_GCCVER < GAIA_COMPILER_GCCVER_USESYNCXX
#		include <asm/atomic.h>
#	endif
#endif

namespace GAIA
{
	namespace SYNC
	{
		class Atomic : public GAIA::Base
		{
		#ifdef __APPLE__
		#	pragma clang diagnostic push
		#	pragma clang diagnostic ignored"-Wdeprecated-declarations"
		#endif
		public:
			GINL Atomic(){this->init();}
			GINL Atomic(const Atomic& src){this->operator = (src);}
			GINL Atomic(const GAIA::N64& src){this->operator = (src);}
			GINL GAIA::GVOID Set(const GAIA::N64& l){m_n = l;}
			GINL GAIA::N64 Get() const{return m_n;}
			GINL Atomic& operator = (const Atomic& src){GAST(&src != this); m_n = src.m_n; return *this;}
			GINL Atomic& operator = (const GAIA::N64& src)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				#if GAIA_MACHINE == GAIA_MACHINE64
					m_n = src;
				#else
					*GRCAST(volatile GAIA::NM*)(&m_n) = GSCAST(GAIA::NM)(src);
				#endif
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				m_n = src;
			#else
			#	if GAIA_COMPILER == GAIA_COMPILER_GCC && GAIA_COMPILER_GCCVER >= GAIA_COMPILER_GCCVER_USESYNCXX
					*GRCAST(volatile GAIA::N32*)(&m_n) = GSCAST(GAIA::N32)(src);
			#	else
					*GRCAST(volatile GAIA::N32*)(&m_n) = GSCAST(GAIA::N32)(src);
			#	endif
			#endif
				return *this;
			}
			GINL operator GAIA::N64() const
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				#if GAIA_MACHINE == GAIA_MACHINE64
					return m_n;
				#else
					return *GRCAST(volatile const GAIA::NM*)(&m_n);
				#endif
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				return m_n;
			#else
			#	if GAIA_COMPILER == GAIA_COMPILER_GCC && GAIA_COMPILER_GCCVER >= GAIA_COMPILER_GCCVER_USESYNCXX
					return *GRCAST(volatile const GAIA::N32*)(&m_n);
			#	else
					return *GRCAST(volatile const GAIA::N32*)(&m_n);
			#	endif
			#endif
			}
			GINL GAIA::N64 Increase() // Return the increased value.
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				#if GAIA_MACHINE == GAIA_MACHINE64
					return InterlockedIncrement64(&m_n);
				#else
					return InterlockedIncrement((volatile GAIA::NM*)&m_n);
				#endif
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				return OSAtomicIncrement64(&m_n);
			#else
			#	if GAIA_COMPILER == GAIA_COMPILER_GCC && GAIA_COMPILER_GCCVER >= GAIA_COMPILER_GCCVER_USESYNCXX
					return __sync_add_and_fetch((GAIA::N32*)&m_n, 1);
			#	else
					return atomic_inc_return((GAIA::N32*)&m_n);
			#	endif
			#endif
			}
			GINL GAIA::N64 Decrease() // Return the decreaded value.
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				#if GAIA_MACHINE == GAIA_MACHINE64
					return InterlockedDecrement64(&m_n);
				#else
					return InterlockedDecrement((volatile GAIA::NM*)&m_n);
				#endif
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				return OSAtomicDecrement64(&m_n);
			#else
			#	if GAIA_COMPILER == GAIA_COMPILER_GCC && GAIA_COMPILER_GCCVER >= GAIA_COMPILER_GCCVER_USESYNCXX
					return __sync_sub_and_fetch((GAIA::N32*)&m_n, 1);
			#	else
					return atomic_dec_return((GAIA::N32*)&m_n);
			#	endif
			#endif
			}
			GINL GAIA::N64 Add(const GAIA::N64& src) // Return the added value.
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				#if GAIA_MACHINE == GAIA_MACHINE64
					return InterlockedExchangeAdd64(&m_n, src) + src;
				#else
					return InterlockedExchangeAdd((volatile GAIA::NM*)&m_n, (GAIA::NM)src) + src;
				#endif
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				return OSAtomicAdd64(src, &m_n);
			#else
			#	if GAIA_COMPILER == GAIA_COMPILER_GCC && GAIA_COMPILER_GCCVER >= GAIA_COMPILER_GCCVER_USESYNCXX
					return __sync_add_and_fetch((GAIA::N32*)&m_n, (GAIA::N32)src);
			#	else
					return atomic_add_return((GAIA::N32)src, (GAIA::N32*)&m_n);
			#	endif
			#endif
			}
			GINL GAIA::N64 Add(const Atomic& src) // Return the added value.
			{
				return this->Add((GAIA::N64)src);
			}
			GINL GAIA::N64 operator ++ (){return this->Increase();}
			GINL GAIA::N64 operator -- (){return this->Decrease();}
			GINL GAIA::N64 operator ++ (GAIA::N32){return this->Increase() - 1;}
			GINL GAIA::N64 operator -- (GAIA::N32){return this->Decrease() + 1;}
			GINL GAIA::N64 operator += (const GAIA::N64& src){return this->Add(+src);}
			GINL GAIA::N64 operator -= (const GAIA::N64& src){return this->Add(-src);}
			GINL GAIA::N32 compare(const Atomic& src) const
			{
				GAIA::N64 l = m_n;
				GAIA::N64 lsrc = src.m_n;
				if(l < lsrc)
					return -1;
				else if(l > lsrc)
					return +1;
				return 0;
			}
			GINL GAIA::N32 compare(const GAIA::N64& src) const
			{
				GAIA::N64 l = m_n;
				if(l < src)
					return -1;
				else if(l > src)
					return +1;
				return 0;
			}
			GINL GAIA::N32 compare(GAIA::N32 src) const
			{
				GAIA::N64 l = m_n;
				if(l < src)
					return -1;
				else if(l > src)
					return +1;
				return 0;
			}
			GCLASS_COMPARE_BYCOMPARE(GAIA::SYNC::Atomic)
			GCLASS_COMPARE_BYCOMPARE(GAIA::N64)
			GCLASS_COMPARE_BYCOMPARE(GAIA::N32)
		private:
			GINL GAIA::GVOID init(){m_n = 0;}
		private:
	#if NEG_OS == NEG_OS_OSX || NEG_OS == NEG_OS_IOS
			__attribute__ ((aligned (8)))
	#endif
			volatile GAIA::N64 m_n;
		#ifdef __APPLE__
		#	pragma clang diagnostic pop
		#endif
		};
	}
}

#endif
