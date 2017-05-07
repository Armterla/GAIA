#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_rsa(GAIA::LOG::Log& logobj)
	{
		GAIA::CH n[1024] = {0};
		GAIA::CH e[1024] = {0};
		GAIA::CH d[1024] = {0};
		GAIA::CH szCrypted[1024] = {0};
		GAIA::CH szUncrypted[1024] = {0};
		
		GAIA::DIGIT::RSA rsa;
		TAST(rsa.bit_count() == 0);
		TAST(!rsa.get_public_key(n, e));
		TAST(!rsa.get_private_key(n, d));
		TAST(!rsa.check_key());
		TAST(!rsa.check_public_key());
		TAST(!rsa.check_private_key());
		TAST(rsa.encrypt("Hello World!1234", GAIA::ALGO::gstrlen("Hello World!1234"), szCrypted) == GINVALID);
		rsa.reset();
		
		// Build key.
		if(!rsa.build_key())
			TERROR;
		if(!rsa.build_key())
			TERROR;
		TAST(rsa.check_key());
		TAST(rsa.check_public_key());
		TAST(rsa.check_private_key());

		// Save key.
		if(!rsa.get_public_key(n, e))
			TERROR;
		if(!rsa.get_private_key(n, d))
			TERROR;

		// Crypt by build key(PKCS1).
		if(rsa.encrypt("Hello World!1234", GAIA::ALGO::gstrlen("Hello World!1234"), szCrypted, 128, GAIA::True) != 128)
			TERROR;
		if(rsa.decrypt(szCrypted, szUncrypted, 128, GAIA::True) == GINVALID)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szUncrypted, "Hello World!1234") != 0)
			TERROR;

		// Crypt by build key(not PKCS1).
		if(rsa.encrypt("Hello World!1234", GAIA::ALGO::gstrlen("Hello World!1234"), szCrypted, 128, GAIA::False) != 128)
			TERROR;
		if(rsa.decrypt(szCrypted, szUncrypted, 128, GAIA::False) == GINVALID)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szUncrypted, "Hello World!1234") != 0)
			TERROR;

		// Crypt by set key(PKCS1).
		rsa.reset();
		if(!rsa.set_public_key(n, e))
			TERROR;
		if(rsa.encrypt("Hello World!1234", GAIA::ALGO::gstrlen("Hello World!1234"), szCrypted, 128, GAIA::True) != 128)
			TERROR;

		rsa.reset();
		if(!rsa.set_private_key(n, d))
			TERROR;
		if(!rsa.get_private_key(n, d))
			TERROR;
		if(rsa.decrypt(szCrypted, szUncrypted, 128, GAIA::True) != 16)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szUncrypted, "Hello World!1234") != 0)
			TERROR;

		// Crypt by set key(not PKCS1).
		rsa.reset();
		if(!rsa.set_public_key(n, e))
			TERROR;
		if(!rsa.get_public_key(n, e))
			TERROR;
		if(rsa.encrypt("Hello World!1234", GAIA::ALGO::gstrlen("Hello World!1234"), szCrypted, 128, GAIA::False) != 128)
			TERROR;

		rsa.reset();
		if(!rsa.set_private_key(n, d))
			TERROR;
		if(rsa.decrypt(szCrypted, szUncrypted, 128, GAIA::False) != 16)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szUncrypted, "Hello World!1234") != 0)
			TERROR;
	}
}
