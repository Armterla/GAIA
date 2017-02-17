#ifndef		__GAIA_VERSION__
#define		__GAIA_VERSION__

#define GAIA_VERSION_MAJOR		0 // unsigned int type. 0 is not a valid version.
#define GAIA_VERSION_RELEASE	0 // unsigned int type. 0 is not a valid version.
#define GAIA_VERSION_MINOR		0 // unsigned int type. 0 is not a valid version.
#define GAIA_VERSION_BUILD		0 // unsigned int type. 0 is not a valid version.

static const char* GAIA_VERSION_COMPILEDATE = __DATE__;
static const char* GAIA_VERSION_COMPILETIME = __TIME__;
static const char* GAIA_VERSION_LASTMODIFYTIME = __TIMESTAMP__;

#endif