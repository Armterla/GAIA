#ifndef		__GAIA_TIME_H__
#define		__GAIA_TIME_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <time.h>
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

extern GAIA::U64 g_gaia_processlaunchtime;

namespace GAIA
{
	namespace TIME
	{
	#if GAIA_OS == GAIA_OS_WINDOWS
		GINL GAIA::GVOID FileTimeToTime(const FILETIME& ft, GAIA::U64& t)
		{
			GAIA::U64 ll;
			ll = (((GAIA::U64)ft.dwHighDateTime) << 32) + ((GAIA::U64)ft.dwLowDateTime);
			t = (ll - 116444736000000000L) / 10;
		}
	#endif
		GINL GAIA::N32 leapyear(GAIA::N32 year){return year % 4 == 0;}
		GINL GAIA::N32 yeardays(GAIA::N32 year)
		{
			if(GAIA::TIME::leapyear(year))
				return 366;
			return 365;
		}
		GINL GAIA::N32 monthdays(GAIA::N32 year, GAIA::N32 month)
		{
			static const GAIA::N32 DAYS[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
			if(month == 2)
			{
				if(GAIA::TIME::leapyear(year))
					return DAYS[month] + 1;
			}
			return DAYS[month];
		}
		GINL GAIA::N32 monthdaysall(GAIA::N32 year, GAIA::N32 month)
		{
			static const GAIA::N32 DAYSALL[] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
			if(month > 2)
			{
				if(GAIA::TIME::leapyear(year))
					return DAYSALL[month] + 1;
			}
			return DAYSALL[month];
		}
		GINL GAIA::U64 local_time() // return value is micro second from 1970.1.1.
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			SYSTEMTIME st;
			::GetLocalTime(&st);
			FILETIME ft;
			::SystemTimeToFileTime(&st, &ft);
			GAIA::U64 uTime;
			GAIA::TIME::FileTimeToTime(ft, uTime);
			return uTime;
		#else
			timeval now;
			struct timezone tz;
			::gettimeofday(&now, &tz);
			return (GAIA::U64)(now.tv_sec - tz.tz_minuteswest * 60) * 1000 * 1000 + (GAIA::U64)now.tv_usec;
		#endif
		}
		GINL GAIA::U64 gmt_time() // return value is micro second from 1970.1.1.
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			FILETIME ft;
			::GetSystemTimeAsFileTime(&ft);
			GAIA::U64 uTime;
			GAIA::TIME::FileTimeToTime(ft, uTime);
			return uTime;
		#else
			timeval now;
			::gettimeofday(&now, GNIL);
			return (GAIA::U64)now.tv_sec * 1000 * 1000 + (GAIA::U64)now.tv_usec;
		#endif
		}
		GINL GAIA::U64 tick_time() // return value is micro second from process startuped.
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			static GAIA::N64 nFreq = 0;
			if(nFreq == 0)
			{
				::QueryPerformanceFrequency((LARGE_INTEGER*)&nFreq);
				if(nFreq == 0)
					nFreq = 1;
			}
			GAIA::N64 nCounter;
			::QueryPerformanceCounter((LARGE_INTEGER*)&nCounter);
			return (GAIA::U64)((GAIA::F64)nCounter / (GAIA::F64)nFreq * 1000.0 * 1000.0);
		#else
			timeval now;
			::gettimeofday(&now, GNIL);
			return ((GAIA::U64)now.tv_sec * 1000 * 1000 + (GAIA::U64)now.tv_usec);
		#endif
		}
		GINL GAIA::U64 process_time()
		{
			return GAIA::TIME::tick_time() - g_gaia_processlaunchtime;
		}
		/*
		 *	64-bit time compressed object as follow:
		 *
		 *	year 			: 18 bit.
		 *	month			: 4 bit.
		 *	day				: 5 bit.
		 *	hour			: 5 bit.
		 * 	minute			: 6 bit.
		 *	second			: 6 bit.
		 *	million second	: 10 bit.
		 *	micro second	: 10 bit.
		 */
		template<typename _DataType1, typename _DataType2> GAIA::BL timemkaux(const _DataType1* pSrc, _DataType2* pDst)
		{
			/*
			*	convert "00010203" to "0001-02-03";
			*	convert "000102030405" to "0001-02-03_04:05";
			*	convert "00010203040506" to "0001-02-03_04:05:06";
			*	convert "00010203040506007" to "0001-02-03_04:05:06_007";
			*	convert "00010203040506007008" to "0001-02-03_04:05:06_007:008";
			*/
			GPCHR_NULLSTR_RET(pSrc, GAIA::False);
			GPCHR_NULL_RET(pDst, GAIA::False);
			GAST(pSrc != pDst);
			if(pSrc == pDst)
				return GAIA::False;
			GAIA::NUM sLen = GAIA::ALGO::gstrlen(pSrc);
			if(sLen != 8 && sLen != 12 && sLen != 14 && sLen != 17 && sLen != 20 && sLen < 20)
			{
				GASTFALSE;
				return GAIA::False;
			}
			if(sLen >= 8)
			{
				GAIA::ALGO::gstrcpy(pDst, pSrc, 4);
				pDst += 4;
				pSrc += 4;
				*pDst++ = '-';
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
				*pDst++ = '-';
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
			}
			if(sLen >= 12)
			{
				*pDst++ = '_';
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
				*pDst++ = ':';
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
			}
			if(sLen >= 14)
			{
				*pDst++ = ':';
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
			}
			if(sLen >= 17)
			{
				*pDst++ = '_';
				GAIA::ALGO::gstrcpy(pDst, pSrc, 3);
				pDst += 3;
				pSrc += 3;
			}
			if(sLen >= 20)
			{
				*pDst++ = '_';
				GAIA::ALGO::gstrcpy(pDst, pSrc, 3);
				pDst += 3;
				pSrc += 3;
			}
			*pDst = '\0';
			return GAIA::True;
		}
		template<typename _DataType1, typename _DataType2> GAIA::BL timermaux(const _DataType1* pSrc, _DataType2* pDst)
		{
			/*
			* convert "00010203" from "0001-02-03";
			* convert "000102030405" from "0001-02-03_04:05";
			* convert "00010203040506" from "0001-02-03_04:05:06";
			* convert "00010203040506007" from "0001-02-03_04:05:06_007";
			* convert "00010203040506007008" from "0001-02-03_04:05:06_007:008";
			*/
			GPCHR_NULLSTR_RET(pSrc, GAIA::False);
			GPCHR_NULL_RET(pDst, GAIA::False);
			GAST(pSrc != pDst);
			if(pSrc == pDst)
				return GAIA::False;
			GAIA::NUM sLen = GAIA::ALGO::gstrlen(pSrc);
			if(sLen != 10 && sLen != 16 && sLen != 19 && sLen != 23 && sLen != 27 && sLen < 27)
			{
				GASTFALSE;
				return GAIA::False;
			}
			if(sLen >= 10)
			{
				GAIA::ALGO::gstrcpy(pDst, pSrc, 4);
				pDst += 4;
				pSrc += 4;
				++pSrc;
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
				++pSrc;
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
			}
			if(sLen >= 16)
			{
				++pSrc;
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
				++pSrc;
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
			}
			if(sLen >= 19)
			{
				++pSrc;
				GAIA::ALGO::gstrcpy(pDst, pSrc, 2);
				pDst += 2;
				pSrc += 2;
			}
			if(sLen >= 23)
			{
				++pSrc;
				GAIA::ALGO::gstrcpy(pDst, pSrc, 3);
				pDst += 3;
				pSrc += 3;
			}
			if(sLen >= 27)
			{
				++pSrc;
				GAIA::ALGO::gstrcpy(pDst, pSrc, 3);
				pDst += 3;
				pSrc += 3;
			}
			*pDst = '\0';
			return GAIA::True;
		}
		template<typename _DataType> _DataType* deltatime2string(GAIA::U64 uTime, _DataType* pszDst, GAIA::NUM sMaxResultLen = GINVALID, GAIA::BL* pResult = GNIL)
		{
			// Convert format like ddd_hh-mm-ss_ms_us.

			GAST(pszDst != GNIL);
			GAST(sMaxResultLen == GINVALID || sMaxResultLen > 0);

			GAIA::U64 us = uTime % 1000;
			uTime -= us;
			uTime /= 1000;

			GAIA::U64 ms = uTime % 1000;
			uTime -= ms;
			uTime /= 1000;

			GAIA::U64 s = uTime % 60;
			uTime -= s;
			uTime /= 60;

			GAIA::U64 m = uTime % 60;
			uTime -= m;
			uTime /= 60;

			GAIA::U64 h = uTime % 24;
			uTime -= h;
			uTime /= 24;

			GAIA::U64 d = uTime;

			GAIA::CH szTemp[64];
			_DataType* p = szTemp;

			GAIA::ALGO::castv(d, p, szTemp + sizeof(szTemp) - p);
			p += GAIA::ALGO::gstrlen(p);
			*p++ = '_';

			GAIA::ALGO::castv(h, p, szTemp + sizeof(szTemp) - p);
			p += GAIA::ALGO::gstrlen(p);
			*p++ = ':';

			GAIA::ALGO::castv(m, p, szTemp + sizeof(szTemp) - p);
			p += GAIA::ALGO::gstrlen(p);
			*p++ = ':';

			GAIA::ALGO::castv(s, p, szTemp + sizeof(szTemp) - p);
			p += GAIA::ALGO::gstrlen(p);
			*p++ = '_';

			GAIA::ALGO::castv(ms, p, szTemp + sizeof(szTemp) - p);
			p += GAIA::ALGO::gstrlen(p);
			*p++ = '_';

			GAIA::ALGO::castv(us, p, szTemp + sizeof(szTemp) - p);
			p += GAIA::ALGO::gstrlen(p);

			if(pResult != GNIL)
				*pResult = GAIA::True;
			if(sMaxResultLen != GINVALID)
			{
				if(p - szTemp >= sMaxResultLen)
				{
					if(pResult != GNIL)
						*pResult = GAIA::False;
					if(sMaxResultLen > 1)
						GAIA::ALGO::gstrcpy(pszDst, szTemp, sMaxResultLen - 1);
				}
				GAIA::ALGO::gstrcpy(pszDst, szTemp);
			}
			else
				GAIA::ALGO::gstrcpy(pszDst, szTemp);

			return pszDst;
		}
		class Time : public GAIA::Base
		{
		public:
			GINL Time(){}
			GINL Time(const Time& src){this->operator = (src);}
			GINL Time(const GAIA::N64& src){this->operator = (src);}
			template<typename _ParamDataType> Time(const _ParamDataType* psz){this->operator = (psz);}
			GINL GAIA::BL check() const{return !(y == 0 && mo == 0 && d == 0 && h == 0 && mi == 0 && sec == 0 && msec == 0 && usec == 0);}
			GINL GAIA::GVOID reset(){y = mo = d = h = mi = sec = msec = usec = 0;}
			GINL GAIA::BL isvalid() const
			{
				if(y < 0)
					return GAIA::False;
				if(mo < 1 || mo > 12)
					return GAIA::False;
				if(d < 1 || d > GAIA::TIME::monthdays(y, mo))
					return GAIA::False;
				if(h < 0 || h > 23)
					return GAIA::False;
				if(mi < 0 || mi > 59)
					return GAIA::False;
				if(sec < 0 || sec > 59)
					return GAIA::False;
				if(msec < 0 || msec > 999)
					return GAIA::False;
				if(usec < 0 || usec > 999)
					return GAIA::False;
				return GAIA::True;
			}
			GINL Time& operator = (const Time& src){GAST(&src != this); y = src.y; mo = src.mo; d = src.d; h = src.h; mi = src.mi; sec = src.sec; msec = src.msec; usec = src.usec; return *this;}
			GINL Time& operator = (const GAIA::N64& src){this->usecond(src); return *this;}
			template<typename _ParamDataType> Time& operator = (const _ParamDataType* psz){this->from(psz); return *this;}
			GINL GAIA::N32 compare(const Time& src) const
			{
				if(y < src.y)
					return -1;
				else if(y > src.y)
					return +1;
				if(mo < src.mo)
					return -1;
				else if(mo > src.mo)
					return +1;
				if(d < src.d)
					return -1;
				else if(d > src.d)
					return +1;
				if(h < src.h)
					return -1;
				else if(h > src.h)
					return +1;
				if(mi < src.mi)
					return -1;
				else if(mi > src.mi)
					return +1;
				if(sec < src.sec)
					return -1;
				else if(sec > src.sec)
					return +1;
				if(msec < src.msec)
					return -1;
				else if(msec > src.msec)
					return +1;
				if(usec < src.usec)
					return -1;
				else if(usec > src.usec)
					return +1;
				return 0;
			}
			GCLASS_COMPARE_BYCOMPARE(Time)
			template<typename _ParamDataType> GAIA::BL operator == (const _ParamDataType* psz) const{GAIA::TIME::Time t; t.from(psz); return (*this) == t;}
			template<typename _ParamDataType> GAIA::BL operator != (const _ParamDataType* psz) const{GAIA::TIME::Time t; t.from(psz); return (*this) != t;}
			template<typename _ParamDataType> GAIA::BL operator >= (const _ParamDataType* psz) const{GAIA::TIME::Time t; t.from(psz); return (*this) >= t;}
			template<typename _ParamDataType> GAIA::BL operator <= (const _ParamDataType* psz) const{GAIA::TIME::Time t; t.from(psz); return (*this) <= t;}
			template<typename _ParamDataType> GAIA::BL operator > (const _ParamDataType* psz) const{GAIA::TIME::Time t; t.from(psz); return (*this) > t;}
			template<typename _ParamDataType> GAIA::BL operator < (const _ParamDataType* psz) const{GAIA::TIME::Time t; t.from(psz); return (*this) < t;}
			GINL GAIA::BL operator == (const GAIA::N64& src) const{GAIA::TIME::Time t = src; return (*this) == t;}
			GINL GAIA::BL operator != (const GAIA::N64& src) const{GAIA::TIME::Time t = src; return (*this) != t;}
			GINL GAIA::BL operator >= (const GAIA::N64& src) const{GAIA::TIME::Time t = src; return (*this) >= t;}
			GINL GAIA::BL operator <= (const GAIA::N64& src) const{GAIA::TIME::Time t = src; return (*this) <= t;}
			GINL GAIA::BL operator > (const GAIA::N64& src) const{GAIA::TIME::Time t = src; return (*this) > t;}
			GINL GAIA::BL operator < (const GAIA::N64& src) const{GAIA::TIME::Time t = src; return (*this) < t;}
			GINL Time operator + (const GAIA::N64& src) const
			{
				Time ret = *this;
				ret += src;
				return ret;
			}
			GINL Time operator - (const GAIA::N64& src) const
			{
				Time ret = *this;
				ret -= src;
				return ret;
			}
			GINL Time& operator += (const GAIA::N64& src)
			{
				GAIA::N64 n = this->usecond();
				n += src;
				this->usecond(n);
				return *this;
			}
			GINL Time& operator -= (const GAIA::N64& src)
			{
				GAIA::N64 n = this->usecond();
				n -= src;
				this->usecond(n);
				return *this;
			}
			GINL GAIA::GVOID localtime()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				SYSTEMTIME st;
				::GetLocalTime(&st);
				y = (GAIA::U16)st.wYear;
				mo = (GAIA::U8)st.wMonth;
				d = (GAIA::U8)st.wDay;
				h = (GAIA::U8)st.wHour;
				mi = (GAIA::U8)st.wMinute;
				sec = (GAIA::U8)st.wSecond;
				msec = (GAIA::U16)st.wMilliseconds;
				usec = (GAIA::U16)0;
			#else
				timeval tv;
				::gettimeofday(&tv, GNIL);
				time_t tsec = tv.tv_sec;
				struct tm* pTM = ::localtime(&tsec);
				y = GSCAST(GAIA::U16)(pTM->tm_year + 1900);
				mo = GSCAST(GAIA::U8)(pTM->tm_mon + 1);
				d = GSCAST(GAIA::U8)(pTM->tm_mday);
				h = GSCAST(GAIA::U8)(pTM->tm_hour);
				mi = GSCAST(GAIA::U8)(pTM->tm_min);
				sec = GSCAST(GAIA::U8)(pTM->tm_sec);
				msec = GSCAST(GAIA::U16)(tv.tv_usec / 1000);
				usec = GSCAST(GAIA::U16)(tv.tv_usec % 1000);
			#endif
			}
			GINL GAIA::GVOID gmttime()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				SYSTEMTIME st;
				::GetSystemTime(&st);
				y = (GAIA::U16)st.wYear;
				mo = (GAIA::U8)st.wMonth;
				d = (GAIA::U8)st.wDay;
				h = (GAIA::U8)st.wHour;
				mi = (GAIA::U8)st.wMinute;
				sec = (GAIA::U8)st.wSecond;
				msec = (GAIA::U16)st.wMilliseconds;
				usec = (GAIA::U16)0;
			#else
				timeval tv;
				::gettimeofday(&tv, GNIL);
				time_t tsec = tv.tv_sec;
				struct tm* pTM = ::gmtime(&tsec);
				y = GSCAST(GAIA::U16)(pTM->tm_year + 1900);
				mo = GSCAST(GAIA::U8)(pTM->tm_mon + 1);
				d = GSCAST(GAIA::U8)(pTM->tm_mday);
				h = GSCAST(GAIA::U8)(pTM->tm_hour);
				mi = GSCAST(GAIA::U8)(pTM->tm_min);
				sec = GSCAST(GAIA::U8)(pTM->tm_sec);
				msec = GSCAST(GAIA::U16)(tv.tv_usec / 1000);
				usec = GSCAST(GAIA::U16)(tv.tv_usec % 1000);
			#endif
			}
			GINL GAIA::GVOID ticktime()
			{
				this->usecond(GAIA::TIME::tick_time());
			}
			GINL GAIA::GVOID processtime()
			{
				this->usecond(GAIA::TIME::process_time());
			}
			GINL operator GAIA::N64() const{return this->usecond();}
			GINL GAIA::N64 year() const{return y;}
			GINL GAIA::N64 month() const{return this->year() * 12 + mo - 1;}
			GINL GAIA::N64 day() const{return y * 365 + y / 4 + ((y % 4 == 0) ? 0 : 1) + GAIA::TIME::monthdaysall(y, mo) + d - 1;}
			GINL GAIA::N64 hour() const{return this->day() * 24 + h;}
			GINL GAIA::N64 minute() const{return this->hour() * 60 + mi;}
			GINL GAIA::N64 second() const{return this->minute() * 60 + sec;}
			GINL GAIA::N64 msecond() const{return this->second() * 1000 + msec;}
			GINL GAIA::N64 usecond() const{return this->msecond() * 1000 + usec;}
			GINL GAIA::GVOID year(GAIA::N64 year){y = year; mo = d = 1; h = mi = sec = msec = usec = 0;}
			GINL GAIA::GVOID month(GAIA::N64 month){y = month / 12; mo = month % 12 + 1; d = 1; h = mi = sec = msec = usec = 0;}
			GINL GAIA::GVOID day(GAIA::N64 day)
			{
				static const GAIA::N64 DAYS4YEAR = 4 * 365 + 1;
				static const GAIA::N64 ONEYEARDAYS = 366;
				static const GAIA::N64 TWOYEARDAYS = ONEYEARDAYS + 365;
				static const GAIA::N64 THREEYEARDAYS = TWOYEARDAYS + 365;
				y = day / DAYS4YEAR * 4;
				day %= DAYS4YEAR;
				if(day < ONEYEARDAYS){}
				else if(day < TWOYEARDAYS)
				{
					day -= ONEYEARDAYS;
					y += 1;
				}
				else if(day < THREEYEARDAYS)
				{
					day -= TWOYEARDAYS;
					y += 2;
				}
				else
				{
					day -= THREEYEARDAYS;
					y += 3;
				}
				for(GAIA::NUM x = 1; x <= 12; ++x)
				{
					if(day < GAIA::TIME::monthdaysall(y, x + 1))
					{
						day -= GAIA::TIME::monthdaysall(y, x);
						mo = x;
						break;
					}
					GAST(x != 12);
				}
				d = day + 1;
				h = mi = sec = msec = usec = 0;
			}
			GINL GAIA::GVOID hour(GAIA::N64 hour)
			{
				this->day(hour / 24);
				h = hour % 24;
				mi = sec = msec = usec = 0;
			}
			GINL GAIA::GVOID minute(GAIA::N64 minute)
			{
				this->hour(minute / 60);
				mi = minute % 60;
				sec = msec = usec = 0;
			}
			GINL GAIA::GVOID second(GAIA::N64 second)
			{
				this->minute(second / 60);
				sec = second % 60;
				msec = usec = 0;
			}
			GINL GAIA::GVOID msecond(GAIA::N64 msecond)
			{
				this->second(msecond / 1000);
				msec = msecond % 1000;
				usec = 0;
			}
			GINL GAIA::GVOID usecond(GAIA::N64 usecond)
			{
				this->msecond(usecond / 1000);
				usec = usecond % 1000;
			}
			GINL GAIA::GVOID dayinc(){(*this) += GSCAST(GAIA::N64)(24) * 60 * 60 * 1000 * 1000;}
			GINL GAIA::GVOID daydec(){(*this) -= GSCAST(GAIA::N64)(24) * 60 * 60 * 1000 * 1000;}
			template<typename _ParamDataType> GAIA::GVOID from(const _ParamDataType* psz)
			{
				GPCHR_NULLSTR(psz);
				GAIA::NUM sLen = GAIA::ALGO::gstrlen(psz);
				if(sLen != 8 && sLen != 12 && sLen != 14 && sLen != 17 && sLen != 20 && sLen < 20)
				{
					GASTFALSE;
					return;
				}
				this->reset();
				_ParamDataType temp[5];
				if(sLen >= 8)
				{
					GAIA::ALGO::gstrcpy(temp, psz + 0, 4);
					y = GAIA::ALGO::acasts(temp);
					GAIA::ALGO::gstrcpy(temp, psz + 4, 2);
					mo = GAIA::ALGO::acasts(temp);
					GAIA::ALGO::gstrcpy(temp, psz + 6, 2);
					d = GAIA::ALGO::acasts(temp);
				}
				if(sLen >= 12)
				{
					GAIA::ALGO::gstrcpy(temp, psz + 8, 2);
					h = GAIA::ALGO::acasts(temp);
					GAIA::ALGO::gstrcpy(temp, psz + 10, 2);
					mi = GAIA::ALGO::acasts(temp);
				}
				if(sLen >= 14)
				{
					GAIA::ALGO::gstrcpy(temp, psz + 12, 2);
					sec = GAIA::ALGO::acasts(temp);
				}
				if(sLen >= 17)
				{
					GAIA::ALGO::gstrcpy(temp, psz + 14, 3);
					msec = GAIA::ALGO::acasts(temp);
				}
				if(sLen >= 20)
				{
					GAIA::ALGO::gstrcpy(temp, psz + 17, 3);
					usec = GAIA::ALGO::acasts(temp);
				}
			}
			template<typename _ParamDataType> GAIA::GVOID to(_ParamDataType* psz) const
			{
				_ParamDataType temp[32];

				GAIA::ALGO::castv(y, temp, sizeofarray(temp));
				GAIA::ALGO::gstrfillleft(temp, '0', 4);
				GAIA::ALGO::gstrcpy(psz, temp);
				psz += 4;

				GAIA::ALGO::castv(mo, temp, sizeofarray(temp));
				GAIA::ALGO::gstrfillleft(temp, '0', 2);
				GAIA::ALGO::gstrcpy(psz, temp);
				psz += 2;

				GAIA::ALGO::castv(d, temp, sizeofarray(temp));
				GAIA::ALGO::gstrfillleft(temp, '0', 2);
				GAIA::ALGO::gstrcpy(psz, temp);
				psz += 2;

				GAIA::ALGO::castv(h, temp, sizeofarray(temp));
				GAIA::ALGO::gstrfillleft(temp, '0', 2);
				GAIA::ALGO::gstrcpy(psz, temp);
				psz += 2;

				GAIA::ALGO::castv(mi, temp, sizeofarray(temp));
				GAIA::ALGO::gstrfillleft(temp, '0', 2);
				GAIA::ALGO::gstrcpy(psz, temp);
				psz += 2;

				GAIA::ALGO::castv(sec, temp, sizeofarray(temp));
				GAIA::ALGO::gstrfillleft(temp, '0', 2);
				GAIA::ALGO::gstrcpy(psz, temp);
				psz += 2;

				GAIA::ALGO::castv(msec, temp, sizeofarray(temp));
				GAIA::ALGO::gstrfillleft(temp, '0', 3);
				GAIA::ALGO::gstrcpy(psz, temp);
				psz += 3;

				GAIA::ALGO::castv(usec, temp, sizeofarray(temp));
				GAIA::ALGO::gstrfillleft(temp, '0', 3);
				GAIA::ALGO::gstrcpy(psz, temp);
				psz += 3;

				*psz = '\0';
			}
			GINL GAIA::GVOID from(const GAIA::U64& src)
			{
				const GAIA::U32* p = (const GAIA::U32*)&src;
				y = (p[0] >> 14) & 0x00003FFF;
				mo = (p[0] >> 10) & 0x0000000F;
				d = (p[0] >> 5) & 0x0000001F;
				h = p[0] & 0x0000001F;
				mi = (p[1] >> 26) & 0x0000003F;
				sec = (p[1] >> 20) & 0x0000003F;
				msec = (p[1] >> 10) & 0x000003FF;
				usec = p[1] & 0x000003FF;
			}
			GINL GAIA::GVOID to(GAIA::U64& u) const
			{
				GAIA::U32* p = (GAIA::U32*)&u;
				p[0] = (GSCAST(GAIA::U32)(y) << 14) & 0xFFFFC000;
				p[0] |= (GSCAST(GAIA::U32)(mo) << 10) & 0x00003C00;
				p[0] |= (GSCAST(GAIA::U32)(d) << 5) & 0x000003E0;
				p[0] |= (GSCAST(GAIA::U32)(h)) & 0x0000001F;
				p[1] = (GSCAST(GAIA::U32)(mi) << 26) & 0xFC000000;
				p[1] |= (GSCAST(GAIA::U32)(sec) << 20) & 0x03F00000;
				p[1] |= (GSCAST(GAIA::U32)(msec) << 10) & 0x000FFC00;
				p[1] |= (GSCAST(GAIA::U32)(usec)) & 0x000003FF;
			}
		public:
			GAIA::N16 y; 	// Year.4
			GAIA::N8 mo; 	// Month.2
			GAIA::N8 d; 	// Day.2
			GAIA::N8 h; 	// Hour.2
			GAIA::N8 mi; 	// Minute.2
			GAIA::N8 sec; 	// Second.2
			GAIA::N16 msec; // MilliSecond.3
			GAIA::N16 usec; // MicroSecond.3
		};
	}
}

#endif
