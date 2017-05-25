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
		s << "GAIA::X128 size = " << sizeof(GAIA::X128) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Array<GAIA::NUM, 1> size = " << sizeof(GAIA::CTN::Array<GAIA::NUM, 1>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Vector<GAIA::NUM> size = " << sizeof(GAIA::CTN::Vector<GAIA::NUM>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::TString size = " << sizeof(GAIA::CTN::TString) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Pool<GAIA::NUM> size = " << sizeof(GAIA::CTN::Pool<GAIA::NUM>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Set<GAIA::NUM> size = " << sizeof(GAIA::CTN::Set<GAIA::NUM>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Map<GAIA::NUM, GAIA::NUM> size = " << sizeof(GAIA::CTN::Map<GAIA::NUM, GAIA::NUM>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::HashSet<GAIA::NUM> size = " << sizeof(GAIA::CTN::HashSet<GAIA::NUM>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::HashMap<GAIA::NUM, GAIA::NUM> size = " << sizeof(GAIA::CTN::HashMap<GAIA::NUM, GAIA::NUM>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::CTN::Msg size = " << sizeof(GAIA::CTN::Msg) << GAIA_FILELINEBREAK_RN;

		s << GAIA_FILELINEBREAK_RN << "[DIGIT SIZE]" << GAIA_FILELINEBREAK_RN;
		s << "GAIA::DIGIT::RC4<256> size = " << sizeof(GAIA::DIGIT::RC4<256>) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::DIGIT::AES size = " << sizeof(GAIA::DIGIT::AES) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::DIGIT::RSA size = " << sizeof(GAIA::DIGIT::RSA) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::DIGIT::Zip size = " << sizeof(GAIA::DIGIT::Zip) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::DIGIT::Diff size = " << sizeof(GAIA::DIGIT::Diff) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::DIGIT::MD5 size = " << sizeof(GAIA::DIGIT::MD5) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::DIGIT::CRC32 size = " << sizeof(GAIA::DIGIT::CRC32) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::DIGIT::SHA1 size = " << sizeof(GAIA::DIGIT::SHA1) << GAIA_FILELINEBREAK_RN;

		s << GAIA_FILELINEBREAK_RN << "[XML & JSON SIZE]" << GAIA_FILELINEBREAK_RN;
		s << "GAIA::XML::XMLReaderA size = " << sizeof(GAIA::XML::XmlReaderA) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::XML::XMLReaderW size = " << sizeof(GAIA::XML::XmlReaderW) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::XML::XMLWriterA size = " << sizeof(GAIA::XML::XmlWriterA) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::XML::XMLWriterW size = " << sizeof(GAIA::XML::XmlWriterW) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::JSON::JsonReaderA size = " << sizeof(GAIA::JSON::JsonReaderA) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::JSON::JsonReaderW size = " << sizeof(GAIA::JSON::JsonReaderW) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::JSON::JsonWriterA size = " << sizeof(GAIA::JSON::JsonWriterA) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::JSON::JsonWriterW size = " << sizeof(GAIA::JSON::JsonWriterW) << GAIA_FILELINEBREAK_RN;

		s << GAIA_FILELINEBREAK_RN << "[MATH SIZE]" << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::RID32 size = " << sizeof(GAIA::MATH::RID32) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::RID64 size = " << sizeof(GAIA::MATH::RID64) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::MATH::RID128 size = " << sizeof(GAIA::MATH::RID128) << GAIA_FILELINEBREAK_RN;
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

		s << GAIA_FILELINEBREAK_RN << "[NETWORK SIZE]" << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::Socket size = " << sizeof(GAIA::NETWORK::Socket) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::SUDPSocket size = " << sizeof(GAIA::NETWORK::SUDPSocket) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::AsyncSocket size = " << sizeof(GAIA::NETWORK::AsyncSocket) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::AsyncDispatcher size = " << sizeof(GAIA::NETWORK::AsyncDispatcher) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::HttpURL size = " << sizeof(GAIA::NETWORK::HttpURL) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::HttpHead size = " << sizeof(GAIA::NETWORK::HttpHead) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::HttpServer size = " << sizeof(GAIA::NETWORK::HttpServer) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::HttpRequest size = " << sizeof(GAIA::NETWORK::HttpRequest) << GAIA_FILELINEBREAK_RN;
		s << "GAIA::NETWORK::Http size = " << sizeof(GAIA::NETWORK::Http) << GAIA_FILELINEBREAK_RN;
		
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
