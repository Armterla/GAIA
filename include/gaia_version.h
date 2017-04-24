#ifndef		__GAIA_VERSION__
#define		__GAIA_VERSION__

static const unsigned long long GAIA_VERSION_MAJOR		= 0;
static const unsigned long long GAIA_VERSION_RELEASE	= 0;
static const unsigned long long GAIA_VERSION_MINOR		= 4;
static const unsigned long long GAIA_VERSION_BUILD		= 1;

static const unsigned long long GAIA_VERSION =
		(GAIA_VERSION_MAJOR << 48) |
		(GAIA_VERSION_RELEASE << 32) |
		(GAIA_VERSION_MINOR << 16) |
		GAIA_VERSION_BUILD;
#define GAIA_VERSION_STRING  "0.0.4 build:1"

static const char* GAIA_VERSION_COMPILEDATE = __DATE__;
static const char* GAIA_VERSION_COMPILETIME = __TIME__;
static const char* GAIA_VERSION_LASTMODIFYTIME = __TIMESTAMP__;

#endif
