#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_rsa(GAIA::LOG::Log& logobj)
	{
		GAIA::DIGIT::RSA rsa;
		if(!rsa.build_key())
			TERROR;

		GAIA::CH n[1024] = {0};
		GAIA::CH e[1024] = {0};
		GAIA::CH d[1024] = {0};
		GAIA::CH szCrypted[1024] = {0};
		GAIA::CH szUncrypted[1024] = {0};

		//
		if(!rsa.get_public_key(n, e))
			TERROR;
		if(!rsa.get_private_key(n, d))
			TERROR;

		//
		if(rsa.encrypt("Hello World!", GAIA::ALGO::gstrlen("Hello World!"), szCrypted, 128, GAIA::True) == GINVALID)
			TERROR;
		if(rsa.decrypt(szCrypted, szUncrypted, 128, GAIA::True) == GINVALID)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szUncrypted, "Hello World!") != 0)
			TERROR;

		//
		if(rsa.encrypt("Hello World!", GAIA::ALGO::gstrlen("Hello World!"), szCrypted, 128, GAIA::False) == GINVALID)
			TERROR;
		if(rsa.decrypt(szCrypted, szUncrypted, 128, GAIA::False) == GINVALID)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szUncrypted, "Hello World!") != 0)
			TERROR;

		//
		rsa.reset();
		if(!rsa.set_public_key(n, e))
			TERROR;
		if(rsa.encrypt("Hello World!", GAIA::ALGO::gstrlen("Hello World!"), szCrypted, 128, GAIA::True) == GINVALID)
			TERROR;

		rsa.reset();
		if(!rsa.set_private_key(n, d))
			TERROR;
		if(rsa.decrypt(szCrypted, szUncrypted, 128, GAIA::True) == GINVALID)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szUncrypted, "Hello World!") != 0)
			TERROR;

		//
		rsa.reset();
		if(!rsa.set_public_key(n, e))
			TERROR;
		if(rsa.encrypt("Hello World!", GAIA::ALGO::gstrlen("Hello World!"), szCrypted, 128, GAIA::False) == GINVALID)
			TERROR;

		rsa.reset();
		if(!rsa.set_private_key(n, d))
			TERROR;
		if(rsa.decrypt(szCrypted, szUncrypted, 128, GAIA::False) == GINVALID)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szUncrypted, "Hello World!") != 0)
			TERROR;
	}
}
