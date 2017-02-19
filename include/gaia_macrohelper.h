/*
*	WARNING! Only helper macro will be define in this file.
*/

#ifndef		__GAIA_MACROHELPER_H__
#define		__GAIA_MACROHELPER_H__

/* Common operation. */
#define sizeofarray(arr) ((GAIA::NM)(sizeof(arr) / sizeof((arr)[0])))
#define sizeofarray2(arr) ((GAIA::NM)(sizeof(arr) / sizeof((arr)[0][0])))
#define gsizeofobj(type, member) sizeof(GRCAST(const type*)(0)->member)
#define gsizeofarray(type, member) sizeofarray(GRCAST(const type*)(0)->member)
#define gsizeofarray2(type, member) sizeofarray2(GRCAST(const type*)(0)->member)
#define goffsetof(type, member) (GAIA::NM)(&GRCAST(const type*)(0)->member)
#define zeromem(type) do{GAIA::ALGO::gmemset(type, 0, sizeof(*type));}while(0)
#define zeromems(type) do{GAIA::ALGO::gmemset(type, 0, sizeof(type));}while(0)

#define isbase(type) GAIA::isbasetype((type)0);
#define isinteger(type) GAIA::isintegertype((type)0)
#define isreal(type) GAIA::isrealtype((type)0)
#define ischar(type) GAIA::ischartype((type)0)
#define issigned(type) GAIA::issignedtype((type)0)
#define isunsigned(type) GAIA::isunsignedtype((type)0)

/* Class helper. */
#define GCLASS_COMPARE_BYCOMPARE(type) \
			GINL GAIA::BL operator == (const type& src) const{return this->compare(src) == 0;}\
			GINL GAIA::BL operator != (const type& src) const{return this->compare(src) != 0;}\
			GINL GAIA::BL operator >= (const type& src) const{return this->compare(src) >= 0;}\
			GINL GAIA::BL operator <= (const type& src) const{return this->compare(src) <= 0;}\
			GINL GAIA::BL operator > (const type& src) const{return this->compare(src) > 0;}\
			GINL GAIA::BL operator < (const type& src) const{return this->compare(src) < 0;}

#define GCLASS_COMPARE_BYCOMPAREPTR(type) \
			GINL GAIA::BL operator == (const type* src) const{return this->compare(src) == 0;}\
			GINL GAIA::BL operator != (const type* src) const{return this->compare(src) != 0;}\
			GINL GAIA::BL operator >= (const type* src) const{return this->compare(src) >= 0;}\
			GINL GAIA::BL operator <= (const type* src) const{return this->compare(src) <= 0;}\
			GINL GAIA::BL operator > (const type* src) const{return this->compare(src) > 0;}\
			GINL GAIA::BL operator < (const type* src) const{return this->compare(src) < 0;}

#define GCLASS_COMPAREEX(t, k, type) \
			GINL GAIA::BL operator == (const type& src) const{return (t) == (src.k);}\
			GINL GAIA::BL operator != (const type& src) const{return (t) != (src.k);}\
			GINL GAIA::BL operator >= (const type& src) const{return (t) >= (src.k);}\
			GINL GAIA::BL operator <= (const type& src) const{return (t) <= (src.k);}\
			GINL GAIA::BL operator > (const type& src) const{return (t) > (src.k);}\
			GINL GAIA::BL operator < (const type& src) const{return (t) < (src.k);}

#define GCLASS_COMPAREEX2(t1, k1, t2, k2, type) \
			GINL GAIA::BL operator == (const type& src) const\
			{\
				if((t1) == (src.k1) && (t2) == (src.k2))\
					return GAIA::True;\
				return GAIA::False;\
			}\
			GINL GAIA::BL operator != (const type& src) const\
			{\
				if((t1) != (src.k1) || (t2) != (src.k2))\
					return GAIA::True;\
				return GAIA::False;\
			}\
			GINL GAIA::BL operator >= (const type& src) const\
			{\
				if((t1) > (src.k1))\
					return GAIA::True;\
				else if((t1) < (src.k1))\
					return GAIA::False;\
				if((t2) >= (src.k2))\
					return GAIA::True;\
				else\
					return GAIA::False;\
			}\
			GINL GAIA::BL operator <= (const type& src) const\
			{\
				if((t1) < (src.k1))\
					return GAIA::True;\
				else if((t1) > (src.k1))\
					return GAIA::False;\
				if((t2) <= (src.k2))\
					return GAIA::True;\
				else\
					return GAIA::False;\
			}\
			GINL GAIA::BL operator > (const type& src) const{return !(*this <= src);}\
			GINL GAIA::BL operator < (const type& src) const{return !(*this >= src);}

#define GCLASS_COMPAREEX3(t1, k1, t2, k2, t3, k3, type) \
			GINL GAIA::BL operator == (const type& src) const\
			{\
				if((t1) == (src.k1) && (t2) == (src.k2) && (t3) == (src.k3))\
					return GAIA::True;\
				return GAIA::False;\
			}\
			GINL GAIA::BL operator != (const type& src) const\
			{\
				if((t1) != (src.k1) || (t2) != (src.k2) || (t3) != (src.k3))\
					return GAIA::True;\
				return GAIA::False;\
			}\
			GINL GAIA::BL operator >= (const type& src) const\
			{\
				if((t1) > (src.k1))\
					return GAIA::True;\
				else if((t1) < (src.k1))\
					return GAIA::False;\
				if((t2) > (src.k2))\
					return GAIA::True;\
				else if((t2) < (src.k2))\
					return GAIA::False;\
				if((t3) >= (src.k3))\
					return GAIA::True;\
				else\
					return GAIA::False;\
			}\
			GINL GAIA::BL operator <= (const type& src) const\
			{\
				if((t1) < (src.k1))\
					return GAIA::True;\
				else if((t1) > (src.k1))\
					return GAIA::False;\
				if((t2) < (src.k2))\
					return GAIA::True;\
				else if((t2) > (src.k2))\
					return GAIA::False;\
				if((t3) <= (src.k3))\
					return GAIA::True;\
				else\
					return GAIA::False;\
			}\
			GINL GAIA::BL operator > (const type& src) const{return !(*this <= src);}\
			GINL GAIA::BL operator < (const type& src) const{return !(*this >= src);}

#define GCLASS_COMPAREEX4(t1, k1, t2, k2, t3, k3, t4, k4, type) \
			GINL GAIA::BL operator == (const type& src) const\
			{\
				if((t1) == (src.k1) && (t2) == (src.k2) && (t3) == (src.k3) && (t4) == (src.k4))\
					return GAIA::True;\
				return GAIA::False;\
			}\
			GINL GAIA::BL operator != (const type& src) const\
			{\
				if((t1) != (src.k1) || (t2) != (src.k2) || (t3) != (src.k3) || (t4) != (src.k4))\
					return GAIA::True;\
				return GAIA::False;\
			}\
			GINL GAIA::BL operator >= (const type& src) const\
			{\
				if((t1) > (src.k1))\
					return GAIA::True;\
				else if((t1) < (src.k1))\
					return GAIA::False;\
				if((t2) > (src.k2))\
					return GAIA::True;\
				else if((t2) < (src.k2))\
					return GAIA::False;\
				if((t3) > (src.k3))\
					return GAIA::True;\
				else if((t3) < (src.k3))\
					return GAIA::False;\
				if((t4) >= (src.k4))\
					return GAIA::True;\
				else\
					return GAIA::False;\
			}\
			GINL GAIA::BL operator <= (const type& src) const\
			{\
				if((t1) < (src.k1))\
					return GAIA::True;\
				else if((t1) > (src.k1))\
					return GAIA::False;\
				if((t2) < (src.k2))\
					return GAIA::True;\
				else if((t2) > (src.k2))\
					return GAIA::False;\
				if((t3) < (src.k3))\
					return GAIA::True;\
				else if((t3) > (src.k3))\
					return GAIA::False;\
				if((t4) <= (src.k4))\
					return GAIA::True;\
				else\
					return GAIA::False;\
			}\
			GINL GAIA::BL operator > (const type& src) const{return !(*this <= src);}\
			GINL GAIA::BL operator < (const type& src) const{return !(*this >= src);}

#define GCLASS_COMPARE(t, type) GCLASS_COMPAREEX(t, t, type)
#define GCLASS_COMPARE2(t1, t2, type) GCLASS_COMPAREEX2(t1, t1, t2, t2, type)
#define GCLASS_COMPARE3(t1, t2, t3, type) GCLASS_COMPAREEX3(t1, t1, t2, t2, t3, t3, type)
#define GCLASS_COMPARE4(t1, t2, t3, t4, type) GCLASS_COMPAREEX4(t1, t1, t2, t2, t3, t3, t4, t4, type)

/* GPCHR(GAIA parameter checkup helper) */
#define GPCHR_NULL(p) do{\
			GAST(!!(p));\
			if(!(p))\
				return;}while(0)

#define GPCHR_NULL_RET(p, ret) do{\
			GAST(!!(p));\
			if(!(p))\
				return ret;}while(0)

#define GPCHR_NOTNULL(p) do{\
			GAST(!(p));\
			if(p)\
				return;}while(0)

#define GPCHR_NOTNULL_RET(p, ret) do{\
			GAST(!(p));\
			if(p)\
				return ret;}while(0)

#define GPCHR_NULLSTR(p) do{\
			GAST(!GAIA::ALGO::gstremp(p));\
			if(GAIA::ALGO::gstremp(p))\
				return;}while(0)

#define GPCHR_NULLSTR_RET(p, ret) do{\
			GAST(!GAIA::ALGO::gstremp(p));\
			if(GAIA::ALGO::gstremp(p))\
				return ret;}while(0)

#define GPCHR_NOTNULLSTR(p) do{\
			GAST(GAIA::ALGO::gstremp(p));\
			if(!GAIA::ALGO::gstremp(p))\
				return;}while(0)

#define GPCHR_NOTNULLSTR_RET(p, ret) do{\
			GAST(GAIA::ALGO::gstremp(p));\
			if(!GAIA::ALGO::gstremp(p))\
				return ret;}while(0)

#define GPCHR_TRUE(p) do{\
			GAST(!(p));\
			if(p)\
				return;}while(0)

#define GPCHR_TRUE_RET(p, ret) do{\
			GAST(!(p));\
			if(p)\
				return ret;}while(0)

#define GPCHR_FALSE(p) do{\
			GAST(p);\
			if(!(p))\
				return;}while(0)

#define GPCHR_FALSE_RET(p, ret) do{\
			GAST(p);\
			if(!(p))\
				return ret;}while(0)

#define GPCHR_ZERO(p) do{\
			GAST((p) != 0);\
			if((p) == 0)\
				return;}while(0)

#define GPCHR_ZERO_RET(p, ret) do{\
			GAST((p) != 0);\
			if((p) == 0)\
				return ret;}while(0)

#define GPCHR_INVALID(p) do{\
			GAST((p) != GINVALID);\
			if((p) == GINVALID)\
				return;}while(0);

#define GPCHR_INVALID_RET(p, ret) do{\
			GAST((p) != GINVALID);\
			if((p) == GINVALID)\
				return ret;}while(0);

#define GPCHR_VALID(p) do{\
			GAST((p) == GINVALID);\
			if((p) != GINVALID)\
				return;}while(0);

#define GPCHR_VALID_RET(p, ret) do{\
			GAST((p) == GINVALID);\
			if((p) != GINVALID)\
				return ret;}while(0);

#define GPCHR_BELOWZERO(p) do{\
			GAST((p) >= 0);\
			if((p) < 0)\
				return;}while(0)

#define GPCHR_BELOWZERO_RET(p, ret) do{\
			GAST((p) >= 0);\
			if((p) < 0)\
				return ret;}while(0)

#define GPCHR_BELOWEQUALZERO(p) do{\
			GAST((p) > 0);\
			if((p) <= 0)\
				return;}while(0)

#define GPCHR_BELOWEQUALZERO_RET(p, ret) do{\
			GAST((p) > 0);\
			if((p) <= 0)\
				return ret;}while(0)

#define GPCHR_ABOVEZERO(p) do{\
			GAST((p) <= 0);\
			if((p) > 0)\
				return;}while(0)

#define GPCHR_ABOVEZERO_RET(p, ret) do{\
			GAST((p) <= 0);\
			if((p) > 0)\
				return ret;}while(0)

#define GPCHR_ABOVEEQUALZERO(p) do{\
			GAST((p) < 0);\
			if((p) >= 0)\
				return;}while(0)

#define GPCHR_ABOVEEQUALZERO_RET(p, ret) do{\
			GAST((p) < 0);\
			if((p) >= 0)\
				return ret;}while(0)

#ifdef GAIA_STREAM
#	define GSTM g_gaia_stdstream
#else
#	define GSTM g_gaia_invalidstream
#endif

#ifdef GAIA_DEBUG_LOG
#	define GLOG g_gaia_log << g_gaia_log.Type(GAIA::LOG::Log::TYPE_LOG) << g_gaia_log.UserFilter(0xFFFFFFFF)
#	define GWAR g_gaia_log << g_gaia_log.Type(GAIA::LOG::Log::TYPE_WARNING) << g_gaia_log.UserFilter(0xFFFFFFFF)
#	define GERR g_gaia_log << g_gaia_log.Type(GAIA::LOG::Log::TYPE_ERROR) << g_gaia_log.UserFilter(0xFFFFFFFF)
#	define GEND g_gaia_log.End()
#else
#	define GLOG g_gaia_invalidlog
#	define GWAR g_gaia_invalidlog
#	define GERR g_gaia_invalidlog
#	define GEND g_gaia_invalidlog
#endif

#define GAIA_EXCEPTION(name) \
	class Ect##name : public GAIA::ECT::Ect\
	{\
	public:\
		Ect##name(const GAIA::CH* pszFile, GAIA::N32 nCodeLine, const GAIA::CH* pszMsg)\
			: Ect(pszFile, nCodeLine, pszMsg){}\
		Ect##name(const GAIA::CH* pszFile, GAIA::N32 nCodeLine, const GAIA::CH* pszMsg, GAIA::N32 nError, GAIA::N32 nOSError)\
			: Ect(pszFile, nCodeLine, pszMsg, nError, nOSError){}\
	}

#define GAIA_EXCEPTION_FROM(name, basename) \
	class Ect##name : public GAIA::ECT::Ect##basename\
	{\
	public:\
		Ect##name(const GAIA::CH* pszFile, GAIA::N32 nCodeLine, const GAIA::CH* pszMsg)\
			: Ect##basename(pszFile, nCodeLine, pszMsg){}\
		Ect##name(const GAIA::CH* pszFile, GAIA::N32 nCodeLine, const GAIA::CH* pszMsg, GAIA::N32 nError, GAIA::N32 nOSError)\
			: Ect##basename(pszFile, nCodeLine, pszMsg, nError, nOSError){}\
	}

#ifdef GAIA_DEBUG_THROWEXCEPTION
#	define GTHROW(name) do{throw GAIA::ECT::Ect##name(__FILE__, __LINE__, "");}while(0)
#	define GTHROWM(name, message) do{throw GAIA::ECT::Ect##name(__FILE__, __LINE__, message);}while(0)
#	define GTHROW_RET(name, ret) GTHROW(name)
#	define GTHROWM_RET(name, message, ret) GTHROWM(name, message)
#	define GTHROW_PURE(name) throw name;
#else
#	define GTHROW(name) do{}while(0)
#	define GTHROWM(name, message) do{}while(0)
#	define GTHROW_RET(name, ret) return ret
#	define GTHROWM_RET(name, message, ret) return ret
#	define GTHROW_PURE(name) do{}while(0)
#endif

#ifdef GAIA_DEBUG_CATCHEXCEPTION
#	define GTRY try
#	define GCATCH(Type) catch(GAIA::ECT::Ect##Type e)
#	define GCATCHBASE catch(GAIA::ECT::Ect e)
#	define GCATCHALL catch(...)
#else
#	define GTRY if(GAIA::ALWAYSTRUE)
#	define GCATCH(Type) if(GAIA::ALWAYSFALSE)
#	define GCATCHBASE if(GAIA::ALWAYSFALSE)
#	define GCATCHALL if(GAIA::ALWAYSFALSE)
#endif

#if defined(GAIA_HEAP) && defined(GAIA_DEBUG_MEMORYLEAK)
#	define gnew new(GAIA::NEW_FLAG_NORMAL, __FILE__, __LINE__)
#	define gdel delete
#else
#	define gnew new
#	define gdel delete
#endif

#define GAIA_DELETE_TRY(p) do{if((p) != GNIL){gdel (p);}}while(0)
#define GAIA_DELETEARRAY_TRY(p) do{if((p) != GNIL){gdel[] (p);}}while(0)
#define GAIA_RELEASE_TRY(p) do{if((p) != GNIL){(p)->drop_ref();}}while(0)

#define GAIA_DELETE_SAFE(p) do{if((p) != GNIL){gdel (p); (p) = GNIL;}}while(0)
#define GAIA_DELETEARRAY_SAFE(p) do{if((p) != GNIL){gdel[] (p); (p) = GNIL;}}while(0)
#define GAIA_RELEASE_SAFE(p) do{if((p) != GNIL){(p)->drop_ref(); (p) = GNIL;}}while(0)

#endif
