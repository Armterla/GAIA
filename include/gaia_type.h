#ifndef		__GAIA_TYPE_H__
#define		__GAIA_TYPE_H__

#include "gaia_version.h"
#include "gaia_sysconfig.h"
#include "gaia_macrohelper.h"
#include "gaia_predecl.h"

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
#ifdef GRCAST
#	undef GRCAST
#endif

#ifdef GSCAST
#	undef GSCAST
#endif

#ifdef GDCAST
#	undef GDCAST
#endif

#ifdef GCCAST
#	undef GCCAST
#endif

#define GRCAST(type) reinterpret_cast<type>
#define GSCAST(type) static_cast<type>
#define GDCAST(type) dynamic_cast<type>
#define GCCAST(type) const_cast<type>

	/* Integer type declaration. */
	typedef long NM;
	typedef unsigned long UM;
	typedef bool BL;
	typedef char N8;
	typedef unsigned char U8;
	typedef short N16;
	typedef unsigned short U16;
	typedef int N32;
	typedef unsigned int U32;
	typedef long long N64;
	typedef unsigned long long U64;
	typedef U64 UMAXTYPE;
	typedef N64 NMAXTYPE;
#if GAIA_COMPILER == GAIA_COMPILER_VC
#	pragma warning(disable : 4310)
#endif
	static const GAIA::N8 N8MIN = (GAIA::N8)0x80;
	static const GAIA::N8 N8MAX = (GAIA::N8)0x7F;
	static const GAIA::U8 U8MIN = (GAIA::U8)0x00;
	static const GAIA::U8 U8MAX = (GAIA::U8)0xFF;
	static const GAIA::N16 N16MIN = (GAIA::N16)0x8000;
	static const GAIA::N16 N16MAX = (GAIA::N16)0x7FFF;
	static const GAIA::U16 U16MIN = (GAIA::U16)0x0000;
	static const GAIA::U16 U16MAX = (GAIA::U16)0xFFFF;
	static const GAIA::N32 N32MIN = (GAIA::N32)0x80000000;
	static const GAIA::N32 N32MAX = (GAIA::N32)0x7FFFFFFF;
	static const GAIA::U32 U32MIN = (GAIA::U32)0x00000000;
	static const GAIA::U32 U32MAX = (GAIA::U32)0xFFFFFFFF;
	static const GAIA::N64 N64MIN = (GAIA::N64)0x8000000000000000;
	static const GAIA::N64 N64MAX = (GAIA::N64)0x7FFFFFFFFFFFFFFF;
	static const GAIA::U64 U64MIN = (GAIA::U64)0x0000000000000000;
	static const GAIA::U64 U64MAX = (GAIA::U64)0xFFFFFFFFFFFFFFFF;
#if GAIA_MACHINE == GAIA_MACHINE32
	static const GAIA::NM NMMIN = (GAIA::NM)0x80000000;
	static const GAIA::NM NMMAX = (GAIA::NM)0x7FFFFFFF;
	static const GAIA::UM UMMIN = (GAIA::UM)0x00000000;
	static const GAIA::UM UMMAX = (GAIA::UM)0xFFFFFFFF;
#elif GAIA_MACHINE == GAIA_MACHINE64
	static const GAIA::NM NMMIN = (GAIA::NM)0x8000000000000000;
	static const GAIA::NM NMMAX = (GAIA::NM)0x7FFFFFFFFFFFFFFF;
	static const GAIA::UM UMMIN = (GAIA::UM)0x0000000000000000;
	static const GAIA::UM UMMAX = (GAIA::UM)0xFFFFFFFFFFFFFFFF;
#endif
#if GAIA_COMPILER == GAIA_COMPILER_VC
#	pragma warning(default : 4310)
#endif
	/* Float type declaration. */
	typedef float F32; // Variable will have a 'f' prefix.
	typedef double F64; // Variable will have a 'd' prefix.
#if GAIA_REAL == GAIA_REAL_F32
	typedef F32 REAL; // variable will have a 'r' prefix.
#	define R(x) x##F
#elif GAIA_REAL == GAIA_REAL_F64
	typedef F64 REAL;
#	define R(x) x
#endif

	typedef N32 EN;

	/* Char type declaration. */
	typedef char CH;
	typedef wchar_t WCH;
#if GAIA_CHARSET == GAIA_CHARSET_UNICODE
	typedef wchar_t TCH;
#	ifndef _T
#		ifndef __T
#			define __T(x) L##x
#		endif
#		define _T(x) __T(x)
#	endif
#else
	typedef char TCH;
#	ifndef _T
#		define _T(x) x
#	endif
#endif

	/* GAIA size. */
	typedef GAIA::N32 NUM; // The variable will have a 's' prefix.
	static const GAIA::NUM MINSIZE = (GAIA::NUM)N32MIN;
	static const GAIA::NUM MAXSIZE = (GAIA::NUM)N32MAX;

	/* Bool enum. */
	static const BL True = true;
	static const BL False = false;

	/* Void. */
	typedef void GVOID;

	/* Other global constants. */
	static GAIA_DEBUG_CONST GAIA::BL ALWAYSTRUE = GAIA::True;
	static GAIA_DEBUG_CONST GAIA::BL ALWAYSFALSE = GAIA::False;

	/* Common constants. */
	#define GNIL 0
	#define GNILSTR ""
	#define GNILWSTR L""
	#define GNILTSTR _T("")
	#define GINVALID (~0)

	/* X128 */
	class X128
	{
	public:
		GINL X128(){}
		GINL X128(const GAIA::U64& v1, const U64& v2){u64_0 = v1; u64_1 = v2;}
		GINL X128(const GAIA::X128& src){this->operator = (src);}
		template<typename _DataType> X128(const _DataType* p){this->operator = (p);}
		GINL X128(GAIA::U32 u){(*this) = u;}
		GINL GAIA::BL empty() const{return u64_0 == 0 && u64_1 == 0;}
		GINL GAIA::GVOID clear(){u64_0 = u64_1 = 0;}
		template<typename _ParamDataType> GAIA::BL check(const _ParamDataType* psz) const;
		template<typename _ParamDataType> GAIA::GVOID fromstring(const _ParamDataType* psz);
		template<typename _ParamDataType> _ParamDataType* tostring(_ParamDataType* psz) const;
		GINL GAIA::X128& operator = (const GAIA::X128& src){u64_0 = src.u64_0; u64_1 = src.u64_1; return *this;}
		GINL X128& operator = (GAIA::U32 u){u32_0 = u; u32_1 = 0; u32_2 = 0; u32_3 = 0; return *this;}
		template<typename _DataType> GAIA::X128& operator = (const _DataType* p)
		{
			u32_0 = u32_1 = u32_2 = u32_3 = 0;
			for(GAIA::U32 x = 0; x < 32; ++x)
			{
				GAIA::U32 uIndex = x / 8;
				if(p[x] >= '0' && p[x] <= '9')
					u32[uIndex] |= p[x] - '0';
				else if(p[x] >= 'a' && p[x] <= 'f')
					u32[uIndex] |= p[x] - 'a' + 10;
				else if(p[x] >= 'A' && p[x] <= 'F')
					u32[uIndex] |= p[x] - 'A' + 10;
				if((x % 8) != 7)
					u32[uIndex] = u32[uIndex] << 4;
			}
			return *this;
		}
		GINL GAIA::N32 compare(const GAIA::X128& src) const
		{
			if(u64_0 < src.u64_0)
				return -1;
			else if(u64_0 > src.u64_0)
				return +1;
			if(u64_1 < src.u64_1)
				return -1;
			else if(u64_1 > src.u64_1)
				return +1;
			return 0;
		}
		GCLASS_COMPARE_BYCOMPARE(GAIA::X128)
	public:
		union
		{
			GAIA::U8 u8[16];
			GAIA::U16 u16[8];
			GAIA::U32 u32[4];
			GAIA::U64 u64[2];
			class
			{
			public:
				GAIA::U16 u16_0;
				GAIA::U16 u16_1;
				GAIA::U16 u16_2;
				GAIA::U16 u16_3;
				GAIA::U16 u16_4;
				GAIA::U16 u16_5;
				GAIA::U16 u16_6;
				GAIA::U16 u16_7;
			};
			class
			{
			public:
				GAIA::U32 u32_0;
				GAIA::U32 u32_1;
				GAIA::U32 u32_2;
				GAIA::U32 u32_3;
			};
			class
			{
			public:
				GAIA::U64 u64_0;
				GAIA::U64 u64_1;
			};
		};
	};

	/* isbasetype. */
	template<typename _DataType> GINL GAIA::BL isbasetype(const _DataType& t){return GAIA::False;}
	template<> GINL GAIA::BL isbasetype(const GAIA::BL& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::U8& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::N8& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::U16& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::N16& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::U32& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::N32& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::U64& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::N64& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::UM& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::NM& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::F32& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::F64& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::X128& t){return GAIA::True;}
	template<> GINL GAIA::BL isbasetype(const GAIA::WCH& t){return GAIA::True;}

	/* isintegertype. */
	template<typename _DataType> GINL GAIA::BL isintegertype(const _DataType& t){return GAIA::False;}
	template<> GINL GAIA::BL isintegertype(const GAIA::U8& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::N8& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::U16& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::N16& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::U32& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::N32& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::U64& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::N64& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::UM& t){return GAIA::True;}
	template<> GINL GAIA::BL isintegertype(const GAIA::NM& t){return GAIA::True;}

	/* isrealtype. */
	template<typename _DataType> GINL GAIA::BL isrealtype(const _DataType& t){return GAIA::False;}
	template<> GINL GAIA::BL isrealtype(const GAIA::F32& t){return GAIA::True;}
	template<> GINL GAIA::BL isrealtype(const GAIA::F64& t){return GAIA::True;}

	/* ischartype. */
	template<typename _DataType> GINL GAIA::BL ischartype(const _DataType& t){return GAIA::False;}
	template<> GINL GAIA::BL ischartype(const GAIA::CH& t){return GAIA::True;}
	template<> GINL GAIA::BL ischartype(const GAIA::WCH& t){return GAIA::True;}

	/* issignedtype. */
	template<typename _DataType> GINL GAIA::BL issignedtype(const _DataType& t){return GAIA::False;}
	template<> GINL GAIA::BL issignedtype(const GAIA::N8& t){return GAIA::True;}
	template<> GINL GAIA::BL issignedtype(const GAIA::N16& t){return GAIA::True;}
	template<> GINL GAIA::BL issignedtype(const GAIA::N32& t){return GAIA::True;}
	template<> GINL GAIA::BL issignedtype(const GAIA::N64& t){return GAIA::True;}
	template<> GINL GAIA::BL issignedtype(const GAIA::NM& t){return GAIA::True;}

	/* isunsignedtype. */
	template<typename _DataType> GINL GAIA::BL isunsignedtype(const _DataType& t){return GAIA::False;}
	template<> GINL GAIA::BL isunsignedtype(const GAIA::U8& t){return GAIA::True;}
	template<> GINL GAIA::BL isunsignedtype(const GAIA::U16& t){return GAIA::True;}
	template<> GINL GAIA::BL isunsignedtype(const GAIA::U32& t){return GAIA::True;}
	template<> GINL GAIA::BL isunsignedtype(const GAIA::U64& t){return GAIA::True;}
	template<> GINL GAIA::BL isunsignedtype(const GAIA::UM& t){return GAIA::True;}

	/* Class Base. All class's parent. */
	GAIA_ENUM_BEGIN(NEW_FLAG)
		NEW_FLAG_NORMAL,
	GAIA_ENUM_END(NEW_FLAG)

#ifdef GAIA_DEBUG_MEMORYLEAK
#	define GAIA_NEW_INFO , GAIA::NEW_FLAG nf, const GAIA::CH* pszFileName, GAIA::N32 nCodeLine
#else
#	define GAIA_NEW_INFO
#endif

	class Base
	{
	public:
	#if defined(GAIA_HEAP)
	#	if GAIA_OS == GAIA_OS_OSX
			GAIA::GVOID* operator new(GAIA::UM size GAIA_NEW_INFO);
			GAIA::GVOID* operator new[](GAIA::UM size GAIA_NEW_INFO);
	#	else
	#		if GAIA_MACHINE == GAIA_MACHINE32
				GAIA::GVOID* operator new(GAIA::U32 size GAIA_NEW_INFO);
				GAIA::GVOID* operator new[](GAIA::U32 size GAIA_NEW_INFO);
	#		elif GAIA_MACHINE == GAIA_MACHINE64
				GAIA::GVOID* operator new(GAIA::U64 size GAIA_NEW_INFO);
				GAIA::GVOID* operator new[](GAIA::U64 size GAIA_NEW_INFO);
	#		endif
	#endif
		GAIA::GVOID operator delete(GAIA::GVOID* p);
		GAIA::GVOID operator delete[](GAIA::GVOID* p);
	#endif
	};

	/* Entity can been dispatch by virtual table and have a virtual destructor. */
	class Entity : public GAIA::Base
	{
	public:
		GAIA_ENUM_BEGIN(FEATURE)
			FEATURE_CAPACITY,
			FEATURE_SIZE,
		GAIA_ENUM_END(FEATURE)
	public:
		virtual ~Entity(){}
		virtual GAIA::BL base_statistics(FEATURE of, GAIA::GVOID* pResult) const{return GAIA::False;}
		virtual GAIA::BL base_optimize(FEATURE of, GAIA::GVOID* pResult){return GAIA::False;}
	};

	/* Class Object. It's the all class's base(except high-performance container and math class. */
	class Object : public GAIA::Entity
	{
	public:
	};

	/* Class RefObject. If a class need a reference function, it will derived from here. */
	class RefObject : public GAIA::Object
	{
	#ifdef __APPLE__
	#	pragma clang diagnostic push
	#	pragma clang diagnostic ignored"-Wdeprecated-declarations"
	#endif
	public:
		GINL RefObject(){m_nRef = 1; m_bDestructing = GAIA::False;}
		GINL GAIA::N64 rise_ref()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
		#	if GAIA_MACHINE == GAIA_MACHINE64
				return InterlockedIncrement64(&m_nRef);
		#	else
				return InterlockedIncrement(&m_nRef);
		#	endif
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
		#	if GAIA_MACHINE == GAIA_MACHINE64
				return OSAtomicIncrement64(&m_nRef);
		#	else
				return OSAtomicIncrement32(&m_nRef);
		#	endif
		#else
		#	if GAIA_COMPILER == GAIA_COMPILER_GCC && GAIA_COMPILER_GCCVER >= GAIA_COMPILER_GCCVER_USESYNCXX
				return __sync_add_and_fetch(&m_nRef, 1);
		#	else
				return atomic_inc_return(&m_nRef);
		#	endif
		#endif
		}
		GINL GAIA::N64 drop_ref()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
		#	if GAIA_MACHINE == GAIA_MACHINE64
				GAIA::NM nNew = InterlockedDecrement64(&m_nRef);
		#	else
				GAIA::NM nNew = InterlockedDecrement(&m_nRef);
		#	endif
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
		#	if GAIA_MACHINE == GAIA_MACHINE64
				GAIA::N64 nNew = OSAtomicDecrement64(&m_nRef);
		#	else
				GAIA::N32 nNew = OSAtomicDecrement32(&m_nRef);
		#	endif
		#else
		#	if GAIA_COMPILER == GAIA_COMPILER_GCC && GAIA_COMPILER_GCCVER >= GAIA_COMPILER_GCCVER_USESYNCXX
				GAIA::NM nNew = __sync_sub_and_fetch(&m_nRef, 1);
		#	else
				GAIA::NM nNew = atomic_dec_return(&m_nRef);
		#	endif
		#endif
			if(nNew == 0 && !m_bDestructing)
			{
				m_bDestructing = GAIA::True;
				this->RefObjectDestruct();
				gdel this;
			}
			return nNew;
		}
		GINL GAIA::N64 get_ref() const{return m_nRef;}
	protected:
		virtual GAIA::GVOID RefObjectDestruct(){}
	private:
		GINL RefObject& operator = (const RefObject& src){return *this;}
	private:
	#if GAIA_OS == GAIA_OS_WINDOWS
	#	if GAIA_MACHINE == GAIA_MACHINE64
			volatile GAIA::N64 m_nRef;
	#	else
			volatile GAIA::NM m_nRef;
	#	endif
	#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
		#if GAIA_MACHINE == GAIA_MACHINE64
			volatile GAIA::N64 m_nRef;
		#else
			volatile GAIA::N32 m_nRef;
		#endif
	#else
		volatile GAIA::N32 m_nRef;
	#endif
		GAIA::U8 m_bDestructing : 1;
	#ifdef __APPLE__
	#	pragma clang diagnostic pop
	#endif
	};

	/* Type id declaration. */
	GAIA_ENUM_BEGIN(TYPEID)
		TYPEID_NM,
		TYPEID_UM,
		TYPEID_BL,
		TYPEID_N8,
		TYPEID_U8,
		TYPEID_N16,
		TYPEID_U16,
		TYPEID_N32,
		TYPEID_U32,
		TYPEID_N64,
		TYPEID_U64,
		TYPEID_X128,
		TYPEID_F32,
		TYPEID_F64,
		TYPEID_CHARPOINTER,
		TYPEID_CONSTCHARPOINTER,
		TYPEID_WCHARPOINTER,
		TYPEID_CONSTWCHARPOINTER,
		TYPEID_POINTER,
		TYPEID_CONSTPOINTER,
	GAIA_ENUM_END(TYPEID)

	static const GAIA::CH* TYPEID_ANAME[] =
	{
		"Invalid",
		"NM",
		"UM",
		"BL",
		"N8",
		"U8",
		"N16",
		"U16",
		"N32",
		"U32",
		"N64",
		"U64",
		"X128",
		"F32",
		"F64",
		"CharPointer",
		"ConstCharPointer",
		"WCharPointer",
		"ConstWCharPointer",
		"Pointer",
		"ConstPointer",
	};

	static const GAIA::WCH* TYPEID_WNAME[] =
	{
		L"Invalid",
		L"NM",
		L"UM",
		L"BL",
		L"N8",
		L"U8",
		L"N16",
		L"U16",
		L"N32",
		L"U32",
		L"N64",
		L"U64",
		L"X128",
		L"F32",
		L"F64",
		L"CharPointer",
		L"ConstCharPointer",
		L"WCharPointer",
		L"ConstWCharPointer",
		L"Pointer",
		L"ConstPointer",
	};

	GINL TYPEID nametotype(const GAIA::CH* psz);
	GINL TYPEID nametotype(const GAIA::WCH* psz);
	TYPEID nametotype(const char* psz);
	TYPEID nametotype(const wchar_t* psz);
	template<typename _DataType> inline TYPEID datatotype(_DataType t){return TYPEID_INVALID;}
	template<typename _DataType> inline TYPEID datatotype(_DataType* t){return TYPEID_INVALID;}
	template<typename _DataType> inline TYPEID datatotype(const _DataType* t){return TYPEID_INVALID;}
	template<> inline TYPEID datatotype(GAIA::NM t){return TYPEID_NM;}
	template<> inline TYPEID datatotype(GAIA::UM t){return TYPEID_UM;}
	template<> inline TYPEID datatotype(GAIA::BL t){return TYPEID_BL;}
	template<> inline TYPEID datatotype(GAIA::N8 t){return TYPEID_N8;}
	template<> inline TYPEID datatotype(GAIA::U8 t){return TYPEID_U8;}
	template<> inline TYPEID datatotype(GAIA::N16 t){return TYPEID_N16;}
	template<> inline TYPEID datatotype(GAIA::U16 t){return TYPEID_U16;}
	template<> inline TYPEID datatotype(GAIA::N32 t){return TYPEID_N32;}
	template<> inline TYPEID datatotype(GAIA::U32 t){return TYPEID_U32;}
	template<> inline TYPEID datatotype(GAIA::N64 t){return TYPEID_N64;}
	template<> inline TYPEID datatotype(GAIA::U64 t){return TYPEID_U64;}
	template<> inline TYPEID datatotype(X128 t){return TYPEID_X128;}
	template<> inline TYPEID datatotype(GAIA::F32 t){return TYPEID_F32;}
	template<> inline TYPEID datatotype(GAIA::F64 t){return TYPEID_F64;}
	template<> inline TYPEID datatotype(GAIA::N8* t){return TYPEID_CHARPOINTER;}
	template<> inline TYPEID datatotype(const GAIA::N8* t){return TYPEID_CONSTCHARPOINTER;}
	template<> inline TYPEID datatotype(GAIA::WCH* t){return TYPEID_WCHARPOINTER;}
	template<> inline TYPEID datatotype(const GAIA::WCH* t){return TYPEID_CONSTWCHARPOINTER;}
	template<> inline TYPEID datatotype(GAIA::GVOID* t){return TYPEID_POINTER;}
	template<> inline TYPEID datatotype(const GAIA::GVOID* t){return TYPEID_CONSTPOINTER;}

	/* Common type. */
	GAIA_ENUM_BEGIN(SEEK_TYPE)
		SEEK_TYPE_BEGIN,
		SEEK_TYPE_END,
		SEEK_TYPE_FORWARD,
		SEEK_TYPE_BACKWARD,
	GAIA_ENUM_END(SEEK_TYPE)

	GAIA_ENUM_BEGIN(RELATION_TYPE)
		RELATION_TYPE_EQUAL,
		RELATION_TYPE_NOTEQUAL,
		RELATION_TYPE_ABOVE,
		RELATION_TYPE_BELOW,
		RELATION_TYPE_ABOVEEQUAL,
		RELATION_TYPE_BELOWEQUAL,
		RELATION_TYPE_TRUE,
		RELATION_TYPE_FALSE,
	GAIA_ENUM_END(RELATION_TYPE)

	GAIA_ENUM_BEGIN(STRING_TYPE)
		STRING_TYPE_STRING	= 1 << 0,

		STRING_TYPE_INTEGER	= 1 << 1,
		STRING_TYPE_N8		= 1 << 2,	// Not supported!
		STRING_TYPE_U8		= 1 << 3,	// Not supported!
		STRING_TYPE_N16		= 1 << 4,	// Not supported!
		STRING_TYPE_U16		= 1 << 5,	// Not supported!
		STRING_TYPE_N32		= 1 << 6,	// Not supported!
		STRING_TYPE_U32		= 1 << 7,	// Not supported!
		STRING_TYPE_N64		= 1 << 8,	// Not supported!
		STRING_TYPE_U64		= 1 << 9,	// Not supported!

		STRING_TYPE_REAL	= 1 << 10,
		STRING_TYPE_F32		= 1 << 11,	// Not supported!
		STRING_TYPE_F64		= 1 << 12,	// Not supported!

		STRING_TYPE_RID		= 1 << 20,
		STRING_TYPE_RID32	= 1 << 21,
		STRING_TYPE_RID64	= 1 << 22,
		STRING_TYPE_RID128	= 1 << 23,
		STRING_TYPE_X128	= 1 << 23,
	GAIA_ENUM_END(STRING_TYPE)

	GAIA_ENUM_BEGIN(CHARSET_TYPE) // If you change the sequence of the enum item, you must change CHARSET_CODEPAGE array element's sequence.
		CHARSET_TYPE_SYS,
		CHARSET_TYPE_ASCII,
		CHARSET_TYPE_UTF8,
		CHARSET_TYPE_UTF16LE,
		CHARSET_TYPE_UTF16BE,
		CHARSET_TYPE_UTF32LE,
		CHARSET_TYPE_UTF32BE,
	GAIA_ENUM_END(CHARSET_TYPE)

	static const GAIA::U32 CHARSET_CODEPAGE[] =
	{
		0,		// CHARSET_INVALID
		1,		// CHARSET_TYPE_SYS
		2,		// CHARSET_TYPE_ASCII
		65001,	// CHARSET_TYPE_UTF8
		1200,	// CHARSET_TYPE_UTF16LE
		1201,	// CHARSET_TYPE_UTF16BE
		65005,	// CHARSET_TYPE_UTF32LE
		65006,	// CHARSET_TYPE_UTF32BE
	};

	static const GAIA::CH* CHARSET_CODEPAGE_NAMEA[] =
	{
		"invalid",			// CHARSET_INVALID
		GAIA_SYS_CODEPAGE,	// CHARSET_TYPE_SYS
		"ASCII",			// CHARSET_TYPE_ASCII
		"UTF-8",			// CHARSET_TYPE_UTF8
		"UTF-16LE",			// CHARSET_TYPE_UTF16LE
		"UTF-16BE",			// CHARSET_TYPE_UTF16BE
		"UTF-32LE",			// CHARSET_TYPE_UTF32LE
		"UTF-32BE",			// CHARSET_TYPE_UTF32BE
	};

	static const GAIA::U8 UTF8_FILEHEAD[3] = {0xEF, 0xBB, 0xBF};
	static const GAIA::U8 UTF16LE_FILEHEAD[2] = {0xFF, 0xFE};
	static const GAIA::U8 UTF16BE_FILEHEAD[2] = {0xFE, 0xFF};

	GAIA_ENUM_BEGIN(INTERSECT_TYPE)
		INTERSECT_TYPE_OUT,
		INTERSECT_TYPE_NOTINTERSECT = INTERSECT_TYPE_OUT,
		INTERSECT_TYPE_IN,
		INTERSECT_TYPE_INTERSECTED,
	GAIA_ENUM_END(INTERSECT_TYPE)

#if GAIA_OS == GAIA_OS_WINDOWS
	static const GAIA::U32 MAXPL = MAX_PATH;
#else
	static const GAIA::U32 MAXPL = 260;
#endif

	static const GAIA::NUM THREAD_STACK_SIZE = 1024 * 1024;

	static const GAIA::CH TABLE_ASCII[128] =
	{
		/* 1-8 */
		'\0', //	'\0'	NUL(null)
		1	, //			SOH(start of headline)
		2	, //			STX(start of text)
		3	, //			ETX(end of text)
		4	, //			EOT(end of transmission)
		5	, //			ENQ(enquiry)
		6	, //			ACK(acknowledge)
		7	, //			BEL(bell)

		/* 9-16 */
		8	, //			BS(backspace)
		9	, //			HT(horizontal tab)
		10	, //	'\n'	LF(NL line feed, new line)
		11	, //			VT(vertical tab)
		12	, //			FF(NP form feed, new page)
		13	, //	'\r'	CR(carriage return)
		14	, //			SO(shift out)
		15	, //			SI(shift in)

		/* 17-24 */
		16	, //			DLE(data link escape)
		17	, //			DC1(device control 1)
		18	, //			DC2(device control 2)
		19	, //			DC3(device control 3)
		20	, //			DC4(device control 4)
		21	, //			NAK(negative acknowledge)
		22	, //			SYN(synchronous idle)
		23	, //			ETB(end of trans. block)

		/* 25-32 */
		24	, //			CAN(cancel)
		25	, //			EM(end of medium)
		26	, //			SUB(substitute)
		27	, //			ESC(escape)
		28	, //			FS(file separator)
		29	, //			GS(group separator)
		30	, //			RS(record separator)
		31	, //			US(unit separator)

		/* 33-40 */
		' '	, //	' '		(space)
		'!'	, //	'!'		CH
		'\"', //	'\"'	CH
		'#'	, //	'#'		CH
		'$'	, //	'$'		CH
		'%'	, //	'%'		CH
		'&'	, //	'&'		CH
		'\''	, //	'\''	CH

		/* 41-48 */
		'('	, //	'('		CH
		')'	, //	')'		CH
		'*'	, //	'*'		CH
		'+'	, //	'+'		CH
		','	, //	','		CH
		'-'	, //	'-'		CH
		'.'	, //	'.'		CH
		'/'	, //	'/'		CH

		/* 49-56 */
		'0'	, //	'0'		CH
		'1'	, //	'1'		CH
		'2'	, //	'2'		CH
		'3'	, //	'3'		CH
		'4'	, //	'4'		CH
		'5'	, //	'5'		CH
		'6'	, //	'6'		CH
		'7'	, //	'7'		CH

		/* 57-64 */
		'8'	, //	'8'		CH
		'9'	, //	'9'		CH
		':'	, //	':'		CH
		';'	, //	';'		CH
		'<'	, //	'<'		CH
		'='	, //	'='		CH
		'>'	, //	'>'		CH
		'?'	, //	'?'		CH

		/* 65-72 */
		'@'	, //	'@'		CH
		'A'	, //	'A'		CH
		'B'	, //	'B'		CH
		'C'	, //	'C'		CH
		'D'	, //	'D'		CH
		'E'	, //	'E'		CH
		'F'	, //	'F'		CH
		'G'	, //	'G'		CH

		/* 73-80 */
		'H'	, //	'H'		CH
		'I'	, //	'I'		CH
		'J'	, //	'J'		CH
		'K'	, //	'K'		CH
		'L'	, //	'L'		CH
		'M'	, //	'M'		CH
		'N'	, //	'N'		CH
		'O'	, //	'O'		CH

		/* 81-88 */
		'P'	, //	'P'		CH
		'Q'	, //	'Q'		CH
		'R'	, //	'R'		CH
		'S'	, //	'S'		CH
		'T'	, //	'T'		CH
		'U'	, //	'U'		CH
		'V'	, //	'V'		CH
		'W'	, //	'W'		CH

		/* 89-96 */
		'X'	, //	'X'		CH
		'Y'	, //	'Y'		CH
		'Z'	, //	'Z'		CH
		'['	, //	'['		CH
		'\\', //	'\\'	CH
		']'	, //	']'		CH
		'^'	, //	'^'		CH
		'_'	, //	'_'		CH

		/* 97-104 */
		'`'	, //	'`'		CH
		'a'	, //	'a'		CH
		'b'	, //	'b'		CH
		'c'	, //	'c'		CH
		'd'	, //	'd'		CH
		'e'	, //	'e'		CH
		'f'	, //	'f'		CH
		'g'	, //	'g'		CH

		/* 105-112 */
		'h'	, //	'h'		CH
		'i'	, //	'i'		CH
		'j'	, //	'j'		CH
		'k'	, //	'k'		CH
		'l'	, //	'l'		CH
		'm'	, //	'm'		CH
		'n'	, //	'n'		CH
		'o'	, //	'o'		CH

		/* 113-120 */
		'p'	, //	'p'		CH
		'q'	, //	'q'		CH
		'r'	, //	'r'		CH
		's'	, //	's'		CH
		't'	, //	't'		CH
		'u'	, //	'u'		CH
		'v'	, //	'v'		CH
		'w'	, //	'w'		CH

		/* 121-128 */
		'x'	, //	'x'		CH
		'y'	, //	'y'		CH
		'z'	, //	'z'		CH
		'{'	, //	'{'		CH
		'|'	, //	'|'		CH
		'}'	, //	'}'		CH
		'~'	, //	'~'		CH
		127	, //			DEL(delete)
	};
}

namespace GAIA{namespace MSYS{class HeapESG; class LeakDetector;}}
extern GAIA::MSYS::HeapESG g_gaia_globalmsys;
#ifdef GAIA_DEBUG_MEMORYLEAK
	extern GAIA::MSYS::LeakDetector g_gaia_leakdetector;
#endif

#if defined(GAIA_HEAP)
#	if GAIA_OS == GAIA_OS_OSX
		extern GAIA::GVOID* operator new(GAIA::UM size GAIA_NEW_INFO);
		extern GAIA::GVOID* operator new[](GAIA::UM size GAIA_NEW_INFO);
#	else
#		if GAIA_MACHINE == GAIA_MACHINE32
			extern GAIA::GVOID* operator new(GAIA::U32 size GAIA_NEW_INFO);
			extern GAIA::GVOID* operator new[](GAIA::U32 size GAIA_NEW_INFO);
#		elif GAIA_MACHINE == GAIA_MACHINE64
			extern GAIA::GVOID* operator new(GAIA::U64 size GAIA_NEW_INFO);
			extern GAIA::GVOID* operator new[](GAIA::U64 size GAIA_NEW_INFO);
#		endif
#	endif
	extern GAIA::GVOID operator delete(GAIA::GVOID* p) throw();
	extern GAIA::GVOID operator delete[](GAIA::GVOID* p) throw();
#endif

#endif
