#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID tp_objstatus(GAIA::LOG::Log& logobj)
	{
		GAIA::STREAM::StringStream s;

		s << "[BASE OBJECT SIZE]" << GAIA_FILELINEBREAK_RN;
		s << "GAIA::Base size = " << sizeof(GAIA::Base) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::Entity size = " << sizeof(GAIA::Entity) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::Object size = " << sizeof(GAIA::Object) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::RefObject size = " << sizeof(GAIA::RefObject) << GAIA_FILELINEBREAK_RN;

		s << GAIA_FILELINEBREAK_RN << "[CONTAINER SIZE]" << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Array<GAIA::CH, 1> size = " << sizeof(GAIA::CTN::Array<GAIA::CH, 1>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Vector<GAIA::CH> size = " << sizeof(GAIA::CTN::Vector<GAIA::CH>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::TString size = " << sizeof(GAIA::CTN::TString) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Pool size = " << sizeof(GAIA::CTN::Pool<GAIA::CH>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Set size = " << sizeof(GAIA::CTN::Set<GAIA::CH>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Map size = " << sizeof(GAIA::CTN::Map<GAIA::CH, GAIA::CH>) << GAIA_FILELINEBREAK_RN;

		s << GAIA_FILELINEBREAK_RN << "[MATH SIZE]" << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::VEC2<GAIA::F32> size = " << sizeof(GAIA::MATH::VEC2<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::VEC3<GAIA::F32> size = " << sizeof(GAIA::MATH::VEC3<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::VEC4<GAIA::F32> size = " << sizeof(GAIA::MATH::VEC4<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::ARGB<GAIA::F32> size = " << sizeof(GAIA::MATH::ARGB<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::NRGB<GAIA::F32> size = " << sizeof(GAIA::MATH::NRGB<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::AABR<GAIA::F32> size = " << sizeof(GAIA::MATH::AABR<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::AABB<GAIA::F32> size = " << sizeof(GAIA::MATH::AABB<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::MTX44<GAIA::F32> size = " << sizeof(GAIA::MATH::MTX44<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::MTX43<GAIA::F32> size = " << sizeof(GAIA::MATH::MTX43<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::MTX33<GAIA::F32> size = " << sizeof(GAIA::MATH::MTX33<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::MTX32<GAIA::F32> size = " << sizeof(GAIA::MATH::MTX32<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::MTX22<GAIA::F32> size = " << sizeof(GAIA::MATH::MTX22<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::QUA<GAIA::F32> size = " << sizeof(GAIA::MATH::QUA<GAIA::F32>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::PLANE<GAIA::F32> size = " << sizeof(GAIA::MATH::PLANE<GAIA::F32>) << GAIA_FILELINEBREAK_RN;

		GAIA::FSYS::File f;
		GAIA::TCH* pszFileName = gnew GAIA::TCH[GAIA::MAXPL];
		GAIA::ALGO::gstrcpy(pszFileName, g_gaia_appdocdir);
		GAIA::ALGO::gstrcat(pszFileName, _T("objstatus.txt"));
		if(!f.Open(pszFileName, GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS | GAIA::FSYS::File::OPEN_TYPE_WRITE))
		{
			TERROR;

			gdel[] pszFileName;
			return;
		}
		gdel[] pszFileName;
		if(!GAIA::ALGO::gstremp(s.getstring()))
		{
			GAIA::CH* psz = GAIA::ALGO::gstrnewex<GAIA::CH>(s.getstring());
			f.WriteText(psz);
			GAIA_DELETEARRAY_SAFE(psz);
		}
		f.Close();
	}
}
