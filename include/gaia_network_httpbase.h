/*!
	@brief Http base type and class declaration file.

	Components in current file.
		1. Http code list, include code enum list, code name list, code description list, and code description length list.
		2. Http header name list.
		3. Class HttpURL.
		4. Class HttpHead.

	All components could be used for http client or http server, so i write a independence file for it.
*/

#ifndef	 __GAIA_NETWORK_HTTPBASE_H__
#define	 __GAIA_NETWORK_HTTPBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_ctn_string.h"
#include "gaia_ctn_charsstring.h"
#include "gaia_ctn_vector.h"

namespace GAIA
{
	namespace NETWORK
	{
		// Http method.
		GAIA_ENUM_BEGIN(HTTP_METHOD)
			HTTP_METHOD_PUT,
			HTTP_METHOD_POST,
			HTTP_METHOD_GET,
			HTTP_METHOD_HEAD,
			HTTP_METHOD_DELETE,
			HTTP_METHOD_OPTIONS,
			HTTP_METHOD_TRACE,
			HTTP_METHOD_CONNECT,
		GAIA_ENUM_END(HTTP_METHOD)

		// Http method string.
		static const GAIA::CH* HTTP_METHOD_STRING[] =
		{
			"INVALID",	// HTTP_METHOD_INVALID
			"PUT",		// HTTP_METHOD_PUT
			"POST",		// HTTP_METHOD_POST
			"GET",		// HTTP_METHOD_GET
			"HEAD",		// HTTP_METHOD_HEAD
			"DELETE",	// HTTP_METHOD_DELETE
			"OPTIONS",	// HTTP_METHOD_OPTIONS
			"TRACE",	// HTTP_METHOD_TRACE
			"CONNECT",	// HTTP_METHOD_CONNECT
		};

		// Http code declaration here.
		GAIA_ENUM_BEGIN(HTTP_CODE)
			/* Information */
			HTTP_CODE_CONTINUE,					// 100
			HTTP_CODE_SWITCHINGPROTOCALS,		// 101

			/* Successful */
			HTTP_CODE_OK,						// 200
			HTTP_CODE_CREATED,					// 201
			HTTP_CODE_ACCEPTED,					// 202
			HTTP_CODE_NONAUTH,					// 203
			HTTP_CODE_NOCONTENT,				// 204
			HTTP_CODE_RESETCONTENT,				// 205
			HTTP_CODE_PARTIALCONTENT,			// 206

			/* Redirection */
			HTTP_CODE_MULTIPLECHOISE,			// 300
			HTTP_CODE_MOVEDPERMANENTLY,			// 301
			HTTP_CODE_FOUND,					// 302
			HTTP_CODE_SEEOTHER,					// 303
			HTTP_CODE_NOTMODIFIED,				// 304
			HTTP_CODE_USEPROXY,					// 305
			HTTP_CODE_TEMPORARYREDIRECT,		// 307

			/* Client error */
			HTTP_CODE_BADREQUEST,				// 400
			HTTP_CODE_UNAUTHORIZED,				// 401
			HTTP_CODE_PAYMENTREQUIRED,			// 402
			HTTP_CODE_FORBIDDEN,				// 403
			HTTP_CODE_NOTFOUND,					// 404
			HTTP_CODE_METHODNOTALLOWED,			// 405
			HTTP_CODE_NOTACCEPTABLE,			// 406
			HTTP_CODE_PROXYAUTHREQUIRED,		// 407
			HTTP_CODE_REQUESTTIMEOUT,			// 408
			HTTP_CODE_CONFLICT,					// 409
			HTTP_CODE_GONE,						// 410
			HTTP_CODE_LENGTHREQUIRED,			// 411
			HTTP_CODE_PRECONDITIONFAILED,		// 412
			HTTP_CODE_REQUESTENTITYTOOLARGE,	// 413
			HTTP_CODE_REQUESTURITOOLONG,		// 414
			HTTP_CODE_UNSUPPORTMEDIATYPE,		// 415
			HTTP_CODE_REQUESTRANGENOTSATISFIABLE,// 416
			HTTP_CODE_EXPECTATIONFAILED,		// 417

			/* Server error */
			HTTP_CODE_INTERNALSERVERERROR,		// 500
			HTTP_CODE_NOTIMPLEMENT,				// 501
			HTTP_CODE_BADGATEWAY,				// 502
			HTTP_CODE_SERVICEUNAVAILABLE,		// 503
			HTTP_CODE_GATEWAYTIMEOUT,			// 504
			HTTP_CODE_HTTPVERSIONNOTSUPPORT,	// 505
		GAIA_ENUM_END(HTTP_CODE)

		static const HTTP_CODE HTTP_CODE_LIST[] =
		{
			HTTP_CODE_INVALID,

			/* Information */
			HTTP_CODE_CONTINUE,					// 100
			HTTP_CODE_SWITCHINGPROTOCALS,		// 101

			/* Successful */
			HTTP_CODE_OK,						// 200
			HTTP_CODE_CREATED,					// 201
			HTTP_CODE_ACCEPTED,					// 202
			HTTP_CODE_NONAUTH,					// 203
			HTTP_CODE_NOCONTENT,				// 204
			HTTP_CODE_RESETCONTENT,				// 205
			HTTP_CODE_PARTIALCONTENT,			// 206

			/* Redirection */
			HTTP_CODE_MULTIPLECHOISE,			// 300
			HTTP_CODE_MOVEDPERMANENTLY,			// 301
			HTTP_CODE_FOUND,					// 302
			HTTP_CODE_SEEOTHER,					// 303
			HTTP_CODE_NOTMODIFIED,				// 304
			HTTP_CODE_USEPROXY,					// 305
			HTTP_CODE_TEMPORARYREDIRECT,		// 307

			/* Client error */
			HTTP_CODE_BADREQUEST,				// 400
			HTTP_CODE_UNAUTHORIZED,				// 401
			HTTP_CODE_PAYMENTREQUIRED,			// 402
			HTTP_CODE_FORBIDDEN,				// 403
			HTTP_CODE_NOTFOUND,					// 404
			HTTP_CODE_METHODNOTALLOWED,			// 405
			HTTP_CODE_NOTACCEPTABLE,			// 406
			HTTP_CODE_PROXYAUTHREQUIRED,		// 407
			HTTP_CODE_REQUESTTIMEOUT,			// 408
			HTTP_CODE_CONFLICT,					// 409
			HTTP_CODE_GONE,						// 410
			HTTP_CODE_LENGTHREQUIRED,			// 411
			HTTP_CODE_PRECONDITIONFAILED,		// 412
			HTTP_CODE_REQUESTENTITYTOOLARGE,	// 413
			HTTP_CODE_REQUESTURITOOLONG,		// 414
			HTTP_CODE_UNSUPPORTMEDIATYPE,		// 415
			HTTP_CODE_REQUESTRANGENOTSATISFIABLE,// 416
			HTTP_CODE_EXPECTATIONFAILED,		// 417

			/* Server error */
			HTTP_CODE_INTERNALSERVERERROR,		// 500
			HTTP_CODE_NOTIMPLEMENT,				// 501
			HTTP_CODE_BADGATEWAY,				// 502
			HTTP_CODE_SERVICEUNAVAILABLE,		// 503
			HTTP_CODE_GATEWAYTIMEOUT,			// 504
			HTTP_CODE_HTTPVERSIONNOTSUPPORT,	// 505
		};

		static const GAIA::CH* HTTP_CODE_STRING[] =
		{
			"0",	// HTTP_CODE_INVALID

			/* Information */
			"100",	// HTTP_CODE_CONTINUE
			"101",	// HTTP_CODE_SWITCHINGPROTOCALS

			/* Successful */
			"200",	// HTTP_CODE_OK
			"201",	// HTTP_CODE_CREATED
			"202",	// HTTP_CODE_ACCEPTED
			"203",	// HTTP_CODE_NONAUTH
			"204",	// HTTP_CODE_NOCONTENT
			"205",	// HTTP_CODE_RESETCONTENT
			"206",	// HTTP_CODE_PARTIALCONTENT

			/* Redirection */
			"300",	// HTTP_CODE_MULTIPLECHOISE
			"301",	// HTTP_CODE_MOVEDPERMANENTLY
			"302",	// HTTP_CODE_FOUND
			"303",	// HTTP_CODE_SEEOTHER
			"304",	// HTTP_CODE_NOTMODIFIED
			"305",	// HTTP_CODE_USEPROXY
			"307",	// HTTP_CODE_TEMPORARYREDIRECT

			/* Client error */
			"400",	// HTTP_CODE_BADREQUEST
			"401",	// HTTP_CODE_UNAUTHORIZED
			"402",	// HTTP_CODE_PAYMENTREQUIRED
			"403",	// HTTP_CODE_FORBIDDEN
			"404",	// HTTP_CODE_NOTFOUND
			"405",	// HTTP_CODE_METHODNOTALLOWED
			"406",	// HTTP_CODE_NOTACCEPTABLE
			"407",	// HTTP_CODE_PROXYAUTHREQUIRED
			"408",	// HTTP_CODE_REQUESTTIMEOUT
			"409",	// HTTP_CODE_CONFLICT
			"410",	// HTTP_CODE_GONE
			"411",	// HTTP_CODE_LENGTHREQUIRED
			"412",	// HTTP_CODE_PRECONDITIONFAILED
			"413",	// HTTP_CODE_REQUESTENTITYTOOLARGE
			"414",	// HTTP_CODE_REQUESTURITOOLONG
			"415",	// HTTP_CODE_UNSUPPORTMEDIATYPE
			"416",	// HTTP_CODE_REQUESTRANGENOTSATISFIABLE
			"417",	// HTTP_CODE_EXPECTATIONFAILED

			/* Server error */
			"500",	// HTTP_CODE_INTERNALSERVERERROR
			"501",	// HTTP_CODE_NOTIMPLEMENT
			"502",	// HTTP_CODE_BADGATEWAY
			"503",	// HTTP_CODE_SERVICEUNAVAILABLE
			"504",	// HTTP_CODE_GATEWAYTIMEOUT
			"505",	// HTTP_CODE_HTTPVERSIONNOTSUPPORT
		};

		static const GAIA::NUM HTTP_CODE_STRING_LENGTH[] =
		{
			1, // "0",		HTTP_CODE_INVALID

			/* Information */
			3, // "100",	HTTP_CODE_CONTINUE
			3, // "101",	HTTP_CODE_SWITCHINGPROTOCALS

			/* Successful */
			3, // "200",	HTTP_CODE_OK
			3, // "201",	HTTP_CODE_CREATED
			3, // "202",	HTTP_CODE_ACCEPTED
			3, // "203",	HTTP_CODE_NONAUTH
			3, // "204",	HTTP_CODE_NOCONTENT
			3, // "205",	HTTP_CODE_RESETCONTENT
			3, // "206",	HTTP_CODE_PARTIALCONTENT

			/* Redirection */
			3, // "300",	HTTP_CODE_MULTIPLECHOISE
			3, // "301",	HTTP_CODE_MOVEDPERMANENTLY
			3, // "302",	HTTP_CODE_FOUND
			3, // "303",	HTTP_CODE_SEEOTHER
			3, // "304",	HTTP_CODE_NOTMODIFIED
			3, // "305",	HTTP_CODE_USEPROXY
			3, // "307",	HTTP_CODE_TEMPORARYREDIRECT

			/* Client error */
			3, // "400",	HTTP_CODE_BADREQUEST
			3, // "401",	HTTP_CODE_UNAUTHORIZED
			3, // "402",	HTTP_CODE_PAYMENTREQUIRED
			3, // "403",	HTTP_CODE_FORBIDDEN
			3, // "404",	HTTP_CODE_NOTFOUND
			3, // "405",	HTTP_CODE_METHODNOTALLOWED
			3, // "406",	HTTP_CODE_NOTACCEPTABLE
			3, // "407",	HTTP_CODE_PROXYAUTHREQUIRED
			3, // "408",	HTTP_CODE_REQUESTTIMEOUT
			3, // "409",	HTTP_CODE_CONFLICT
			3, // "410",	HTTP_CODE_GONE
			3, // "411",	HTTP_CODE_LENGTHREQUIRED
			3, // "412",	HTTP_CODE_PRECONDITIONFAILED
			3, // "413",	HTTP_CODE_REQUESTENTITYTOOLARGE
			3, // "414",	HTTP_CODE_REQUESTURITOOLONG
			3, // "415",	HTTP_CODE_UNSUPPORTMEDIATYPE
			3, // "416",	HTTP_CODE_REQUESTRANGENOTSATISFIABLE
			3, // "417",	HTTP_CODE_EXPECTATIONFAILED

			/* Server error */
			3, // "500",	HTTP_CODE_INTERNALSERVERERROR
			3, // "501",	HTTP_CODE_NOTIMPLEMENT
			3, // "502",	HTTP_CODE_BADGATEWAY
			3, // "503",	HTTP_CODE_SERVICEUNAVAILABLE
			3, // "504",	HTTP_CODE_GATEWAYTIMEOUT
			3, // "505",	HTTP_CODE_HTTPVERSIONNOTSUPPORT
		};

		static const GAIA::N32 HTTP_CODE_VALUE[] =
		{
			0,		// HTTP_CODE_INVALID

			/* Information */
			100,	// HTTP_CODE_CONTINUE,
			101,	// HTTP_CODE_SWITCHINGPROTOCALS,

			/* Successful */
			200,	// HTTP_CODE_OK
			201,	// HTTP_CODE_CREATED
			202,	// HTTP_CODE_ACCEPTED
			203,	// HTTP_CODE_NONAUTH
			204,	// HTTP_CODE_NOCONTENT
			205,	// HTTP_CODE_RESETCONTENT
			206,	// HTTP_CODE_PARTIALCONTENT

			/* Redirection */
			300,	// HTTP_CODE_MULTIPLECHOISE
			301,	// HTTP_CODE_MOVEDPERMANENTLY
			302,	// HTTP_CODE_FOUND
			303,	// HTTP_CODE_SEEOTHER
			304,	// HTTP_CODE_NOTMODIFIED
			305,	// HTTP_CODE_USEPROXY
			307,	// HTTP_CODE_TEMPORARYREDIRECT

			/* Client error */
			400,	// HTTP_CODE_BADREQUEST
			401,	// HTTP_CODE_UNAUTHORIZED
			402,	// HTTP_CODE_PAYMENTREQUIRED
			403,	// HTTP_CODE_FORBIDDEN
			404,	// HTTP_CODE_NOTFOUND
			405,	// HTTP_CODE_METHODNOTALLOWED
			406,	// HTTP_CODE_NOTACCEPTABLE
			407,	// HTTP_CODE_PROXYAUTHREQUIRED
			408,	// HTTP_CODE_REQUESTTIMEOUT
			409,	// HTTP_CODE_CONFLICT
			410,	// HTTP_CODE_GONE
			411,	// HTTP_CODE_LENGTHREQUIRED
			412,	// HTTP_CODE_PRECONDITIONFAILED
			413,	// HTTP_CODE_REQUESTENTITYTOOLARGE
			414,	// HTTP_CODE_REQUESTURITOOLONG
			415,	// HTTP_CODE_UNSUPPORTMEDIATYPE
			416,	// HTTP_CODE_REQUESTRANGENOTSATISFIABLE
			417,	// HTTP_CODE_EXPECTATIONFAILED

			/* Server error */
			500,	// HTTP_CODE_INTERNALSERVERERROR
			501,	// HTTP_CODE_NOTIMPLEMENT
			502,	// HTTP_CODE_BADGATEWAY
			503,	// HTTP_CODE_SERVICEUNAVAILABLE
			504,	// HTTP_CODE_GATEWAYTIMEOUT
			505,	// HTTP_CODE_HTTPVERSIONNOTSUPPORT
		};

		static const GAIA::CH* HTTP_CODE_DESCRIPTION[] =
		{
			"Invalid",						// HTTP_CODE_INVALID(0)

			/* Information */
			"Continue",						// HTTP_CODE_CONTINUE(100)
			"Switching Protocals",			// HTTP_CODE_SWITCHINGPROTOCALS(101)

			/* Successful */
			"OK",							// HTTP_CODE_OK(200)
			"Created",						// HTTP_CODE_CREATED(201)
			"Accepted",						// HTTP_CODE_ACCEPTED(202)
			"Non-Authoritative Information",// HTTP_CODE_NONAUTH(203)
			"No Content",					// HTTP_CODE_NOCONTENT(204)
			"Reset Content",				// HTTP_CODE_RESETCONTENT(205)
			"Partial Content",				// HTTP_CODE_PARTIALCONTENT(206)

			/* Redirection */
			"Multiple Choices",				// HTTP_CODE_MULTIPLECHOISE(300)
			"Moved Permanently",			// HTTP_CODE_MOVEDPERMANENTLY(301)
			"Found",						// HTTP_CODE_FOUND(302)
			"See Other",					// HTTP_CODE_SEEOTHER(303)
			"Not Modified",					// HTTP_CODE_NOTMODIFIED(304)
			"Use Proxy",					// HTTP_CODE_USEPROXY(305)
			"Temporary Redirect",			// HTTP_CODE_TEMPORARYREDIRECT(307)

			/* Client error */
			"Bad Request",					// HTTP_CODE_BADREQUEST(400)
			"Unauthonzed",					// HTTP_CODE_UNAUTHORIZED(401)
			"Payment Required",				// HTTP_CODE_PAYMENTREQUIRED(402)
			"Forbidden",					// HTTP_CODE_FORBIDDEN(403)
			"Not Found",					// HTTP_CODE_NOTFOUND(404)
			"Method Not Allowed",			// HTTP_CODE_METHODNOTALLOWED(405)
			"Not Acceptable",				// HTTP_CODE_NOTACCEPTABLE(406)
			"Proxy Authentication Required",// HTTP_CODE_PROXYAUTHREQUIRED(407)
			"Request Timeout",				// HTTP_CODE_REQUESTTIMEOUT(408)
			"Conflict",						// HTTP_CODE_CONFLICT(409)
			"Gone",							// HTTP_CODE_GONE(410)
			"Length Required",				// HTTP_CODE_LENGTHREQUIRED(411)
			"Precondition Failed",			// HTTP_CODE_PRECONDITIONFAILED(412)
			"Request Entity Too Large",		// HTTP_CODE_REQUESTENTITYTOOLARGE(413)
			"Request-URI Too Long",			// HTTP_CODE_REQUESTURITOOLONG(414)
			"Unsupported Media Type",		// HTTP_CODE_UNSUPPORTMEDIATYPE(415)
			"Requested Range Not Satisfiable", // HTTP_CODE_REQUESTRANGENOTSATISFIABLE(416)
			"Expectation Failed",			// HTTP_CODE_EXPECTATIONFAILED(417)

			/* Server error */
			"Internal Server Error",		// HTTP_CODE_INTERNALSERVERERROR(500)
			"Not Implemented",				// HTTP_CODE_NOTIMPLEMENT(501)
			"Bad Gateway",					// HTTP_CODE_BADGATEWAY(502)
			"Service Unavailable",			// HTTP_CODE_SERVICEUNAVAILABLE(503)
			"Gateway Timeout",				// HTTP_CODE_GATEWAYTIMEOUT(504)
			"HTTP Version Not Supported",	// HTTP_CODE_HTTPVERSIONNOTSUPPORT(505)
		};

		static const GAIA::NUM HTTP_CODE_DESCRIPTION_LENGTH[] =
		{
			sizeof("Invalid") - 1,						// HTTP_CODE_INVALID(0)

			/* Information */
			sizeof("Continue") - 1,						// HTTP_CODE_CONTINUE(100)
			sizeof("Switching Protocals") - 1,			// HTTP_CODE_SWITCHINGPROTOCALS(101)

			/* Successful */
			sizeof("OK") - 1,							// HTTP_CODE_OK(200)
			sizeof("Created") - 1,						// HTTP_CODE_CREATED(201)
			sizeof("Accepted") - 1,						// HTTP_CODE_ACCEPTED(202)
			sizeof("Non-Authoritative Information") - 1,// HTTP_CODE_NONAUTH(203)
			sizeof("No Content") - 1,					// HTTP_CODE_NOCONTENT(204)
			sizeof("Reset Content") - 1,				// HTTP_CODE_RESETCONTENT(205)
			sizeof("Partial Content") - 1,				// HTTP_CODE_PARTIALCONTENT(206)

			/* Redirection */
			sizeof("Multiple Choices") - 1,				// HTTP_CODE_MULTIPLECHOISE(300)
			sizeof("Moved Permanently") - 1,			// HTTP_CODE_MOVEDPERMANENTLY(301)
			sizeof("Found") - 1,						// HTTP_CODE_FOUND(302)
			sizeof("See Other") - 1,					// HTTP_CODE_SEEOTHER(303)
			sizeof("Not Modified") - 1,					// HTTP_CODE_NOTMODIFIED(304)
			sizeof("Use Proxy") - 1,					// HTTP_CODE_USEPROXY(305)
			sizeof("Temporary Redirect") - 1,			// HTTP_CODE_TEMPORARYREDIRECT(307)

			/* Client error */
			sizeof("Bad Request") - 1,					// HTTP_CODE_BADREQUEST(400)
			sizeof("Unauthonzed") - 1,					// HTTP_CODE_UNAUTHORIZED(401)
			sizeof("Payment Required") - 1,				// HTTP_CODE_PAYMENTREQUIRED(402)
			sizeof("Forbidden") - 1,					// HTTP_CODE_FORBIDDEN(403)
			sizeof("Not Found") - 1,					// HTTP_CODE_NOTFOUND(404)
			sizeof("Method Not Allowed") - 1,			// HTTP_CODE_METHODNOTALLOWED(405)
			sizeof("Not Acceptable") - 1,				// HTTP_CODE_NOTACCEPTABLE(406)
			sizeof("Proxy Authentication Required") - 1,// HTTP_CODE_PROXYAUTHREQUIRED(407)
			sizeof("Request Timeout") - 1,				// HTTP_CODE_REQUESTTIMEOUT(408)
			sizeof("Conflict") - 1,						// HTTP_CODE_CONFLICT(409)
			sizeof("Gone") - 1,							// HTTP_CODE_GONE(410)
			sizeof("Length Required") - 1,				// HTTP_CODE_LENGTHREQUIRED(411)
			sizeof("Precondition Failed") - 1,			// HTTP_CODE_PRECONDITIONFAILED(412)
			sizeof("Request Entity Too Large") - 1,		// HTTP_CODE_REQUESTENTITYTOOLARGE(413)
			sizeof("Request-URI Too Long") - 1,			// HTTP_CODE_REQUESTURITOOLONG(414)
			sizeof("Unsupported Media Type") - 1,		// HTTP_CODE_UNSUPPORTMEDIATYPE(415)
			sizeof("Requested Range Not Satisfiable") - 1, // HTTP_CODE_REQUESTRANGENOTSATISFIABLE(416)
			sizeof("Expectation Failed") - 1,			// HTTP_CODE_EXPECTATIONFAILED(417)

			/* Server error */
			sizeof("Internal Server Error") - 1,		// HTTP_CODE_INTERNALSERVERERROR(500)
			sizeof("Not Implemented") - 1,				// HTTP_CODE_NOTIMPLEMENT(501)
			sizeof("Bad Gateway") - 1,					// HTTP_CODE_BADGATEWAY(502)
			sizeof("Service Unavailable") - 1,			// HTTP_CODE_SERVICEUNAVAILABLE(503)
			sizeof("Gateway Timeout") - 1,				// HTTP_CODE_GATEWAYTIMEOUT(504)
			sizeof("HTTP Version Not Supported") - 1,	// HTTP_CODE_HTTPVERSIONNOTSUPPORT(505)
		};

		// Http head origin name declaration here.
		static const GAIA::CH* HTTP_HEADNAME_ACCEPT				= "Accept";
		static const GAIA::CH* HTTP_HEADNAME_ACCEPTCHARSET		= "Accept-Charset";
		static const GAIA::CH* HTTP_HEADNAME_ACCEPTENCODING		= "Accept-Encoding";
		static const GAIA::CH* HTTP_HEADNAME_ACCEPTLANGUAGE		= "Accept-Language";
		static const GAIA::CH* HTTP_HEADNAME_ACCEPTRANGES 		= "Accept-Ranges";
		static const GAIA::CH* HTTP_HEADNAME_AGE				= "Age";
		static const GAIA::CH* HTTP_HEADNAME_ALLOW				= "Allow";
		static const GAIA::CH* HTTP_HEADNAME_AUTHORIZATION		= "Authorization";
		static const GAIA::CH* HTTP_HEADNAME_CACHECONTROL		= "Cache-Control";
		static const GAIA::CH* HTTP_HEADNAME_CONNECTION 		= "Connection";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTENCODING	= "Content-Encoding";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTLANGUAGE	= "Content-Language";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTLENGTH 		= "Content-Length";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTLOCATION	= "Content-Location";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTMD5			= "Content-MD5";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTRANGE		= "Content-Range";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTTYPE 		= "Content-Type";
		static const GAIA::CH* HTTP_HEADNAME_DATE 				= "Date";
		static const GAIA::CH* HTTP_HEADNAME_ETAG				= "ETag";
		static const GAIA::CH* HTTP_HEADNAME_EXPECT				= "Expect";
		static const GAIA::CH* HTTP_HEADNAME_EXPIRES			= "Expires";
		static const GAIA::CH* HTTP_HEADNAME_FROM				= "From";
		static const GAIA::CH* HTTP_HEADNAME_HOST 				= "Host";
		static const GAIA::CH* HTTP_HEADNAME_IFMATCH			= "If-Match";
		static const GAIA::CH* HTTP_HEADNAME_IFMODIFIEDSINCE	= "If-Modified-Since";
		static const GAIA::CH* HTTP_HEADNAME_IFNONEMATCH		= "If-None-Match";
		static const GAIA::CH* HTTP_HEADNAME_IFRANGE			= "If-Range";
		static const GAIA::CH* HTTP_HEADNAME_IFUNMODIFIEDSINCE	= "If-Unmodified-Since";
		static const GAIA::CH* HTTP_HEADNAME_LASTMODIFIED 		= "Last-Modified";
		static const GAIA::CH* HTTP_HEADNAME_LOCATION			= "Location";
		static const GAIA::CH* HTTP_HEADNAME_MAXFORWORDS		= "Max-Forwards";
		static const GAIA::CH* HTTP_HEADNAME_PRAGMA				= "Pragma";
		static const GAIA::CH* HTTP_HEADNAME_PROXYAUTHENTICATE	= "Proxy-Authenticate";
		static const GAIA::CH* HTTP_HEADNAME_PROXYAUTHORIZATION	= "Proxy-Authorization";
		static const GAIA::CH* HTTP_HEADNAME_RANGE				= "Range";
		static const GAIA::CH* HTTP_HEADNAME_REFERER			= "Referer";
		static const GAIA::CH* HTTP_HEADNAME_TRYAFTER			= "Try-After";
		static const GAIA::CH* HTTP_HEADNAME_SERVER 			= "Server";
		static const GAIA::CH* HTTP_HEADNAME_TE					= "TE";
		static const GAIA::CH* HTTP_HEADNAME_TRAILER			= "Trailer";
		static const GAIA::CH* HTTP_HEADNAME_TRANSFERENCODING	= "Transfer-Encoding";
		static const GAIA::CH* HTTP_HEADNAME_UPGRADE			= "Upgrade";
		static const GAIA::CH* HTTP_HEADNAME_USERAGENT			= "User-Agent";
		static const GAIA::CH* HTTP_HEADNAME_VARY				= "Vary";
		static const GAIA::CH* HTTP_HEADNAME_VIA				= "Via";
		static const GAIA::CH* HTTP_HEADNAME_WARNING			= "Warning";
		static const GAIA::CH* HTTP_HEADNAME_WWWAUTHENTICATE	= "WWW-Authenticate";

		static const GAIA::CH* HTTP_HEADNAME_LIST[] = 
		{
			HTTP_HEADNAME_ACCEPT,
			HTTP_HEADNAME_ACCEPTCHARSET,
			HTTP_HEADNAME_ACCEPTENCODING,
			HTTP_HEADNAME_ACCEPTLANGUAGE,
			HTTP_HEADNAME_ACCEPTRANGES,
			HTTP_HEADNAME_AGE,
			HTTP_HEADNAME_ALLOW,
			HTTP_HEADNAME_AUTHORIZATION,
			HTTP_HEADNAME_CACHECONTROL,
			HTTP_HEADNAME_CONNECTION,
			HTTP_HEADNAME_CONTENTENCODING,
			HTTP_HEADNAME_CONTENTLANGUAGE,
			HTTP_HEADNAME_CONTENTLENGTH,
			HTTP_HEADNAME_CONTENTLOCATION,
			HTTP_HEADNAME_CONTENTMD5,
			HTTP_HEADNAME_CONTENTRANGE,
			HTTP_HEADNAME_CONTENTTYPE,
			HTTP_HEADNAME_DATE,
			HTTP_HEADNAME_ETAG,
			HTTP_HEADNAME_EXPECT,
			HTTP_HEADNAME_EXPIRES,
			HTTP_HEADNAME_FROM,
			HTTP_HEADNAME_HOST,
			HTTP_HEADNAME_IFMATCH,
			HTTP_HEADNAME_IFMODIFIEDSINCE,
			HTTP_HEADNAME_IFNONEMATCH,
			HTTP_HEADNAME_IFRANGE,
			HTTP_HEADNAME_IFUNMODIFIEDSINCE,
			HTTP_HEADNAME_LASTMODIFIED,
			HTTP_HEADNAME_LOCATION,
			HTTP_HEADNAME_MAXFORWORDS,
			HTTP_HEADNAME_PRAGMA,
			HTTP_HEADNAME_PROXYAUTHENTICATE,
			HTTP_HEADNAME_PROXYAUTHORIZATION,
			HTTP_HEADNAME_RANGE,
			HTTP_HEADNAME_REFERER,
			HTTP_HEADNAME_TRYAFTER,
			HTTP_HEADNAME_SERVER,
			HTTP_HEADNAME_TE,
			HTTP_HEADNAME_TRAILER,
			HTTP_HEADNAME_TRANSFERENCODING,
			HTTP_HEADNAME_UPGRADE,
			HTTP_HEADNAME_USERAGENT,
			HTTP_HEADNAME_VARY,
			HTTP_HEADNAME_VIA,
			HTTP_HEADNAME_WARNING,
			HTTP_HEADNAME_WWWAUTHENTICATE,
		};

		/*!
			@brief Http url management class.

			@remarks This class support URL format like this :
				"protocol://hostname[:port]/path[/path1][/][;parameters][?query][#fragment]".
		*/
		class HttpURL : public GAIA::Base
		{
		public:
			/*!
				@brief Constructor.
			*/
			GINL HttpURL(){this->init();}

			/*!
				@brief Copy contructor.
			*/
			GINL HttpURL(const HttpURL& src){this->init(); this->operator = (src);}

			/*!
				@brief Contructor from a char array.
			*/
			GINL HttpURL(const GAIA::CH* psz){this->init(); this->operator = (psz);}

			/*!
				@brief Destructor.
			*/
			GINL ~HttpURL(){}

			/*!
				@brief Check current HttpURL object is empty or not.

				@return If curren HttpURL object is empty, return GAIA::True, or will return GAIA::False.
+			*/
			GINL GAIA::BL Empty() const{return m_url.empty();}

			/*!
				@brief Get URL's string length in chars.

				@return Return the URL's string length.
			*/
			GINL GAIA::BL Size() const{return m_url.size();}

			/*!
				@brief Reset URL to empty.

				@remarks It will clear URL analyzed information too.
			*/
			GINL GAIA::GVOID Reset(){m_url.clear(); this->clear_analyzed();}

			/*!
				@brief File HttpURL object by a URL string.

				@param psz [in] Specify the URL string. It can't be GNIL.

				@param pSize [in] Specify the URL string's size in characters(without '\0').
					If this parameter is GNIL, all characters will be used until to '\0'.

				@remarks
					This function will clear old URL analyzed information, and didn't genereta new URL analyzed information about 
					parameter psz.
			*/
			GINL GAIA::GVOID FromString(const GAIA::CH* psz, const GAIA::NUM* pSize = GNIL)
			{
				if(pSize != GNIL)
				{
					GAST(*pSize <= GAIA::ALGO::gstrlen(psz));
					m_url.assign(psz, *pSize);
				}
				else
					m_url = psz;
				this->clear_analyzed();
			}

			/*!
				@brief Get current HttpURL's string.

				@param pResultSize [out] Used for saving the URL string's size in characters(without '\0').
					If this paramter is GNIL, it will be ignored.

				@return Return the HttpURL's string. If current HttpURL string is empty, it will return "".
			*/
			GINL const GAIA::CH* ToString(GAIA::NUM* pResultSize = GNIL) const
			{
				if(pResultSize != GNIL)
					*pResultSize = m_url.size();
				return m_url.fptr();
			}

			/*!
				@brief Analyze the URL string.

				@return If the URL string content not exist any error and analyzed successfully, return GAIA::True,
					or will return GAIA::False.
			*/
			GINL GAIA::BL Analyze()
			{
				if(m_bAnalyzed)
					return GAIA::True;
				if(m_url.empty())
					return GAIA::False;

				// Local variable declarations.
				GAIA::NUM sProtocalBegin = GINVALID, sProtocalEnd = GINVALID;
				GAIA::NUM sHostNameBegin = GINVALID, sHostNameEnd = GINVALID;
				GAIA::NUM sPortBegin = GINVALID, sPortEnd = GINVALID;
				GAIA::NUM sPathBegin = GINVALID, sPathEnd = GINVALID;
				GAIA::NUM sFullParamBegin = GINVALID, sFullParamEnd = GINVALID;
				GAIA::NUM sFullQueryBegin = GINVALID, sFullQueryEnd = GINVALID;
				GAIA::NUM sFragmentBegin = GINVALID, sFragmentEnd = GINVALID;

				GAIA::NUM sUrlSize = m_url.size();
				const GAIA::CH* pszUrl = m_url.fptr();

				// Partition analyze.
				{
					for(GAIA::NUM x = 0; x < sUrlSize; ++x)
					{
						if(pszUrl[x] == ':')
						{
							if(sHostNameBegin == GINVALID)
							{
								if(x + 2 < sUrlSize && pszUrl[x + 1] == '/' && pszUrl[x + 2] == '/')
								{
									GAST(sProtocalBegin == GINVALID);
									GAST(sProtocalEnd == GINVALID);
									sProtocalBegin = 0;
									sProtocalEnd = x;
									sHostNameBegin = x + 3;
									x += 2;
								}
								else
								{
									sHostNameBegin = 0;
									sHostNameEnd = x;
									GAST(sPortBegin == GINVALID);
									sPortBegin = x + 1;
								}
							}
							else
							{
								if(sPortBegin == GINVALID)
								{
									sPortBegin = x + 1;
									GAST(sHostNameEnd == GINVALID);
									sHostNameEnd = x;
								}
							}
						}
						else if(pszUrl[x] == '/')
						{
							if(sHostNameEnd == GINVALID)
								sHostNameEnd = x;
							else
							{
								if(sPortBegin != GINVALID && sPortEnd == GINVALID)
									sPortEnd = x;
							}
							if(sPathBegin == GINVALID)
								sPathBegin = x;
						}
						else if(pszUrl[x] == ';')
						{
							if(sPathEnd == GINVALID)
								sPathEnd = x;
							if(sFullParamBegin == GINVALID)
								sFullParamBegin = x + 1;
						}
						else if(pszUrl[x] == '?')
						{
							if(sPathEnd == GINVALID)
								sPathEnd = x;
							if(sFullParamBegin != GINVALID && sFullParamEnd == GINVALID)
								sFullParamEnd = x;
							if(sFullQueryBegin == GINVALID)
								sFullQueryBegin = x + 1;
						}
						else if(pszUrl[x] == '#')
						{
							if(sPathEnd == GINVALID)
								sPathEnd = x;
							if(sFullParamBegin != GINVALID && sFullParamEnd == GINVALID)
								sFullParamEnd = x;
							if(sFullQueryBegin != GINVALID && sFullQueryEnd == GINVALID)
								sFullQueryEnd = x;
							if(sFragmentBegin == GINVALID)
								sFragmentBegin = x + 1;
						}
					}

					//
					if(sHostNameBegin == GINVALID && sHostNameEnd != GINVALID)
					{
						GAST(sProtocalBegin == GINVALID);
						GAST(sProtocalEnd == GINVALID);
						sHostNameBegin = 0;
					}

					// Close end index.
					if(sProtocalBegin != GINVALID && sProtocalEnd == GINVALID)
						sProtocalEnd = sUrlSize;
					if(sHostNameBegin != GINVALID && sHostNameEnd == GINVALID)
						sHostNameEnd = sUrlSize;
					if(sPortBegin != GINVALID && sPortEnd == GINVALID)
						sPortEnd = sUrlSize;
					if(sPathBegin != GINVALID && sPathEnd == GINVALID)
						sPathEnd = sUrlSize;
					if(sFullParamBegin != GINVALID && sFullParamEnd == GINVALID)
						sFullParamEnd = sUrlSize;
					if(sFullQueryBegin != GINVALID && sFullQueryEnd == GINVALID)
						sFullQueryEnd = sUrlSize;
					if(sFragmentBegin != GINVALID && sFragmentEnd == GINVALID)
						sFragmentEnd = sUrlSize;

					// If URL is a host address only, like "www.google.com" or "127.0.0.1".
					if(sProtocalBegin == GINVALID &&
					   sHostNameBegin == GINVALID &&
					   sPortBegin == GINVALID &&
					   sPathBegin == GINVALID &&
					   sFullParamBegin == GINVALID &&
					   sFullQueryBegin == GINVALID &&
					   sFragmentBegin == GINVALID)
					{
						sHostNameBegin = 0;
						sHostNameEnd = sUrlSize;
					}
				}

				// Checkup.
				{
					GAIA::NUM listOffset[] =
					{
						sProtocalBegin, sProtocalEnd,
						sHostNameBegin, sHostNameEnd,
						sPortBegin, sPortEnd,
						sPathBegin, sPathEnd,
						sFullParamBegin, sFullParamEnd,
						sFullQueryBegin, sFullQueryEnd,
						sFragmentBegin, sFragmentEnd,
					};
					GAST(sizeofarray(listOffset) % 2 == 0);
					GAIA::NUM sLastValue = 0;
					for(GAIA::NUM x = 0; x < sizeofarray(listOffset); x += 2)
					{
						if(listOffset[x] == GINVALID)
						{
							if(listOffset[x + 1] != GINVALID)
								return GAIA::False;
							listOffset[x] = sLastValue;
							listOffset[x + 1] = sLastValue;
						}
						else
						{
							if(listOffset[x + 1] == GINVALID)
								return GAIA::False;
							sLastValue = listOffset[x + 1];
						}
					}
					for(GAIA::NUM x = 1; x < sizeofarray(listOffset); ++x)
					{
						if(listOffset[x - 1] > listOffset[x])
							return GAIA::False;
					}

					if(sProtocalBegin >= 0 && sProtocalEnd <= sUrlSize && sProtocalBegin < sProtocalEnd)
					{
						m_protocal.psz = m_url.fptr() + sProtocalBegin;
						m_protocal.sLen = sProtocalEnd - sProtocalBegin;
					}

					if(sHostNameBegin >= 0 && sHostNameEnd <= sUrlSize && sHostNameBegin < sHostNameEnd)
					{
						m_hostname.psz = m_url.fptr() + sHostNameBegin;
						m_hostname.sLen = sHostNameEnd - sHostNameBegin;
					}

					if(sPortBegin >= 0 && sPortEnd <= sUrlSize && sPortBegin < sPortEnd)
					{
						m_port.psz = m_url.fptr() + sPortBegin;
						m_port.sLen = sPortEnd - sPortBegin;
					}

					if(sPathBegin >= 0 && sPathEnd <= sUrlSize && sPathBegin < sPathEnd)
					{
						m_path.psz = m_url.fptr() + sPathBegin;
						m_path.sLen = sPathEnd - sPathBegin;
					}

					if(sFullParamBegin >= 0 && sFullParamEnd <= sUrlSize && sFullParamBegin < sFullParamEnd)
					{
						m_fullparam.psz = m_url.fptr() + sFullParamBegin;
						m_fullparam.sLen = sFullParamEnd - sFullParamBegin;
					}

					if(sFullQueryBegin >= 0 && sFullQueryEnd <= sUrlSize && sFullQueryBegin < sFullQueryEnd)
					{
						m_fullquery.psz = m_url.fptr() + sFullQueryBegin;
						m_fullquery.sLen = sFullQueryEnd - sFullQueryBegin;
					}

					if(sFragmentBegin >= 0 && sFragmentEnd <= sUrlSize && sFragmentBegin < sFragmentEnd)
					{
						m_fragment.psz = m_url.fptr() + sFragmentBegin;
						m_fragment.sLen = sFragmentEnd - sFragmentBegin;
					}
				}

				// Full param analyze to list.
				{
					GAST(m_params.empty());
					if(m_fullparam.psz != GNIL)
					{
						const GAIA::CH* p = m_fullparam.psz;
						const GAIA::CH* pLast = p;
						while(p - m_fullparam.psz < m_fullparam.sLen)
						{
							if(*p == ';')
							{
								Node n;
								if(p - pLast == 0)
									n.reset();
								else
								{
									n.psz = pLast;
									n.sLen = p - pLast;
								}
								m_params.push_back(n);
								pLast = p + 1;
							}
							++p;
						}
						if(p != pLast)
						{
							Node n;
							n.psz = pLast;
							n.sLen = p - pLast;
							m_params.push_back(n);
						}
					}
				}

				// Full query analyze to list.
				{
					// Analyze support "a=b&c=d", "a=&c=d", "=b&c=d", "a=", "=b", "a=b&", "=", "=&=".
					GAST(m_queries.empty());
					if(m_fullquery.psz != GNIL)
					{
						const GAIA::CH* p = m_fullquery.psz;
						const GAIA::CH* pLast = p;
						while(p - m_fullquery.psz < m_fullquery.sLen)
						{
							if(*p == '=' || *p == '&')
							{
								Node n;
								if(p - pLast == 0)
									n.reset();
								else
								{
									n.psz = pLast;
									n.sLen = p - pLast;
								}
								m_queries.push_back(n);
								if(*p == '&' && m_queries.size() % 2 != 0)
									m_queries.push_back(n);
								pLast = p + 1;
							}
							++p;
						}

						// Last name or value.
						if(p != pLast)
						{
							Node n;
							n.psz = pLast;
							n.sLen = p - pLast;
							m_queries.push_back(n);
							if(m_queries.size() % 2 != 0)
								m_queries.push_back(n);
						}
					}
					GAST(m_queries.size() % 2 == 0);
				}

				m_bAnalyzed = GAIA::True;
				return GAIA::True;
			}

			/*!
				@brief Get protocal in HttpURL.

				@param psz [out] Used for saving the protocal string.

				@param nMaxSize [in] Specify parameter psz's max size in characters.
					If it is GINVALID, means parameter psz's buffer size is enough.

				@param pResultSize [out] Used for saving the result protocal string's size in characters(without '\0').

				@return If current HttpURL is empty or HttpURL's content can't be analyzed, return GNIL.
			*/
			GINL GAIA::CH* GetProtocal(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GNIL;
				return this->get_analyzed_node(m_protocal, psz, sMaxSize, pResultSize);
			}

			/*!
				@brief Get host name in HttpURL.

				@param psz [out] Used for saving the host name string.

				@param nMaxSize [in] Specify parameter psz's max size in characters.
					If it is GINVALID, means parameter psz's buffer size is enough.

				@param pResultSize [out] Use for saving the result protocal string's size in characters(without '\0').

				@return If current HttpURL is empty or HttpURL's content can't be analyzed, return GNIL.
			*/
			GINL GAIA::CH* GetHostName(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GNIL;
				return this->get_analyzed_node(m_hostname, psz, sMaxSize, pResultSize);
			}

			/*!
				@brief Get port in HttpURL.

				@param psz [out] Used for saving the port string.

				@param nMaxSize [in] Specify parameter psz's max size in characters.
					If it is GINVALID, means parameter psz's buffer size is enough.

				@param pResultSize [out] Used for saving the result port string's size in characters(without '\0').

				@return If current HttpURL is empty or HttpURL's content can't be analyzed, return GNIL.
			*/
			GINL GAIA::CH* GetPort(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GNIL;
				return this->get_analyzed_node(m_port, psz, sMaxSize, pResultSize);
			}

			/*!
				@brief Get the path string in HttpURL.

				@param psz [out] Used for saving the paths string.

				@param nMaxSize [in] Specify parameter psz's max size in characters.
					If it is GINVALID, means parameter psz's buffer size is enough.

				@param pResultSize [out] Used for saving the result paths string's size in characters(without '\0').

				@return If current HttpURL is empty or HttpURL's content can't be analyzed, return GNIL.
			*/
			GINL GAIA::CH* GetPath(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GNIL;
				return this->get_analyzed_node(m_path, psz, sMaxSize, pResultSize);
			}

			/*!
				@brief Get the full parameters string in HttpURL.

				@param psz [out] Used for saving the parameters string.

				@param nMaxSize [in] Specify parameter psz's max size in characters.
					If it is GINVALID, means parameter psz's buffer size is enough.

				@param pResultSize [out] Used for saving the result parameters string's size in characters(without '\0').

				@return If current HttpURL is empty or HttpURL's content can't be analyzed, return GNIL.
			*/
			GINL GAIA::CH* GetFullParam(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GNIL;
				return this->get_analyzed_node(m_fullparam, psz, sMaxSize, pResultSize);
			}

			/*!
				@brief Get the full queries string in HttpURL.

				@param psz [out] Used for saving the queries string.

				@param nMaxSize [in] Specify parameter psz's max size in characters.
					If it is GINVALID, means parameter psz's buffer size is enough.

				@param pResultSize [out] Used for saving the result queries string's size in characters(without '\0').

				@return If current HttpURL is empty or HttpURL's content can't be analyzed, return GNIL.
			*/
			GINL GAIA::CH* GetFullQuery(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GNIL;
				return this->get_analyzed_node(m_fullquery, psz, sMaxSize, pResultSize);
			}

			/*!
				@brief Get the fragment string in HttpURL.

				@param psz [out] Used for saving the fragment string.

				@param nMaxSize [in] Specify parameter psz's max size in characters.
					If it is GINVALID, means parameter psz's buffer size is enough.

				@param pResultSize [out] Used for saving the result fragment string's size in characters(without '\0').

				@return If current HttpURL is empty or HttpURL's content can't be analyzed, return GNIL.
			*/
			GINL GAIA::CH* GetFragment(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GNIL;
				return this->get_analyzed_node(m_fragment, psz, sMaxSize, pResultSize);
			}

			/*!
				@brief Check HttpURL is a pure URL or not.

				@return If HttpURL is a pure URL return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL IsPure() const
			{
				if(m_fullparam.psz != GNIL)
					return GAIA::False;
				if(m_fullquery.psz != GNIL)
					return GAIA::False;
				if(m_fragment.psz != GNIL)
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief Get parameter count in HttpURL's full parameter string.

				@return Return parameter count of parameter string.
			*/
			GINL GAIA::NUM GetParamCount() const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return 0;
				return m_params.size();
			}

			/*!
				@brief Get the parameter in HttpURL's full parameter string.

				@param sIndex [in] Specify the parameter's index.

				@param psz [out] Used for saving the parameter.

				@param nMaxSize [in] Specify parameter psz's max size in characters.
					If it is GINVALID, means parameter psz's buffer size is enough.

				@param pResultSize [out] Used for saving the result parameter's size in characters(without '\0').

				@return If get the parameter successfully, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL GetParam(GAIA::NUM sIndex, GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GAIA::False;
				if(sIndex < 0 || sIndex >= m_params.size())
					return GAIA::False;
				const Node& n = m_params[sIndex];
				GAIA::CH* pszResult = this->get_analyzed_node(n, psz, sMaxSize, pResultSize);
				if(pszResult == GNIL)
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief Check the parameter exist or not in the HttpURL's full parameter string.

				@param psz [in] Specify the parameter name.

				@return If exist return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL ExistParam(const GAIA::CH* psz) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GAIA::False;
				for(GAIA::NUM x = 0; x < m_params.size(); ++x)
				{
					const Node& n = m_params[x];
					if(GAIA::ALGO::gstrcmp(n.psz, psz, n.sLen) == 0)
						return GAIA::True;
				}
				return GAIA::False;
			}

			/*!
				@brief Get query count in HttpURL's full queries string.

				@return Return queries count of queries string.
			*/
			GINL GAIA::NUM GetQueryCount() const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return 0;
				return m_queries.size() / 2;
			}

			/*!
				@brief Get query by a index.

				@param pszName [out] Used for saving the query name string.

				@param pszValue [out] Used for saving the query value string.

				@param nMaxNameSize [in] Specify parameter pszName's max size in characters.
					If it is GINVALID, means parameter pszName's buffer size is enough.

				@param nMaxValueSize [in] Specify parameter pszValue's max size in characters.
					If it is GINVALID, means parameter pszValue's buffer size is enough.

				@param pNameResultSize [out] Used for saving the result query's name size in characters(without '\0').

				@param pValueResultSize [out] Used for saving the result query's value size in characters(without '\0').

				@return If get the query successfully, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL GetQuery(GAIA::NUM sIndex, GAIA::CH* pszName, GAIA::CH* pszValue, GAIA::NUM sMaxNameSize = GINVALID, GAIA::NUM sMaxValueSize = GINVALID, GAIA::NUM* pNameResultSize = GNIL, GAIA::NUM* pValueResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GAIA::False;
				if(sIndex < 0 || sIndex >= m_queries.size())
					return GAIA::False;
				const Node& nname = m_queries[sIndex * 2];
				GAIA::CH* pszResultName = this->get_analyzed_node(nname, pszName, sMaxNameSize, pNameResultSize);
				if(pszResultName == GNIL)
					return GAIA::False;
				const Node& nvalue = m_queries[sIndex * 2 + 1];
				GAIA::CH* pszResultValue = this->get_analyzed_node(nvalue, pszValue, sMaxValueSize, pValueResultSize);
				return GAIA::True;
			}

			/*!
				@brief Get the query value by query name.

				@param pszName [in] Specify the query's name.

				@param pszValue [out] Used for saving the query value.

				@param nMaxValueSize [in] Specify parameter pszValue's max size in characters.
					If it is GINVALID, means parameter pszValue's buffer size is enough.

				@param pValueResultSize [out] Used for saving the result query's value size in characters(without '\0').

				@return If get query value sucessfully, return pszValue, or will return GNIL.
			*/
			GINL GAIA::CH* GetQueryByName(const GAIA::CH* pszName, GAIA::CH* pszValue, GAIA::NUM sMaxValueSize = GINVALID, GAIA::NUM* pValueResultSize = GNIL) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GNIL;
				GAST(m_queries.size() % 2 == 0);
				for(GAIA::NUM x = 0; x < m_queries.size(); x += 2)
				{
					const Node& nname = m_queries[x];
					if(GAIA::ALGO::gstrcmp(nname.psz, pszName, nname.sLen) == 0)
					{
						const Node& nvalue = m_queries[x + 1];
						return this->get_analyzed_node(nvalue, pszValue, sMaxValueSize, pValueResultSize);
					}
				}
				return GNIL;
			}

			/*!
				@brief Check the query exist or not.

				@param pszName [in] Specify the query's name.

				@return If the query exist, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL ExistQuery(const GAIA::CH* pszName) const
			{
				if(!GCCAST(HttpURL*)(this)->Analyze())
					return GAIA::False;
				for(GAIA::NUM x = 0; x < m_queries.size(); x += 2)
				{
					const Node& nname = m_queries[x];
					if(GAIA::ALGO::gstrcmp(nname.psz, pszName, nname.sLen) == 0)
						return GAIA::True;
				}
				return GAIA::False;
			}

			/*!
				@brief Deep copy from another HttpURL object.

				@param src [in] Specify the copy source.

				@return Return current object's reference.
			*/
			GINL HttpURL& operator = (const HttpURL& src)
			{
				if(m_url == src.m_url)
					return *this;
				m_url = src.m_url;
				this->clear_analyzed();
				return *this;
			}

			/*!
				@brief Fill content by a URL string, it like HttpURL::FromString.

				@param psz [in] Specify a URL string.

				@return Return current object's reference.

				@see HttpURL::FromString.
			*/
			GINL HttpURL& operator = (const GAIA::CH* psz)
			{
				if(m_url == psz)
					return *this;
				this->FromString(psz);
				return *this;
			}

			/*!
				@brief Default convert current HttpURL to a ascii URL string.

				@return Return the URL string. If current HttpURL is empty, return "".
			*/
			GINL operator const GAIA::CH*(){return m_url.fptr();}

			/*!
				@brief Compare current HttpURL object with a ascii URL string.

				@param psz [in] Specify another operate value.

				@return
					If current HttpURL below parameter psz, return -1.
					If current HttpURL above parameter psz, return +1.
					If current HttpURL equal parameter psz, return 0.
			*/
			GINL GAIA::N32 compare(const GAIA::CH* psz) const{return m_url.compare(psz);}

			/*!
				@brief Compare current HttpURL object with another HttpURL object.

				@param src [in] Specify another operate value.

				@return
					If current HttpURL below parameter src, return -1.
					If current HttpURL above parameter src, return +1.
					If current HttpURL equal parameter src, return 0.
			*/
			GINL GAIA::N32 compare(const HttpURL& src) const{return m_url.compare(src.m_url);}
			GCLASS_COMPARE_BYCOMPAREPTR(GAIA::CH)
			GCLASS_COMPARE_BYCOMPARE(HttpURL)

		private:
			class Node : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset(){psz = GNIL; sLen = 0;}
			public:
				const GAIA::CH* psz;
				GAIA::NUM sLen;
			};

		private:
			GINL GAIA::GVOID init(){this->clear_analyzed();}
			GINL GAIA::GVOID clear_analyzed()
			{
				m_protocal.reset();
				m_hostname.reset();
				m_port.reset();
				m_path.reset();
				m_fullparam.reset();
				m_fullquery.reset();
				m_fragment.reset();
				m_params.clear();
				m_queries.clear();
				m_bAnalyzed = GAIA::False;
			}
			GINL GAIA::CH* get_analyzed_node(const Node& n, GAIA::CH* psz, GAIA::NUM sMaxSize, GAIA::NUM* pResultSize) const
			{
				if(psz == GNIL)
				{
					GAST(sMaxSize == GINVALID);
					if(pResultSize != GNIL)
						*pResultSize = n.sLen;
					return (GAIA::CH*)n.psz;
				}
				else
				{
					if(sMaxSize != GINVALID && n.sLen >= sMaxSize)
						return GNIL;
					if(pResultSize != GNIL)
						*pResultSize = n.sLen;
					GAIA::ALGO::gstrcpy(psz, n.psz, n.sLen);
					return psz;
				}
			}

		private:
			GAIA::CTN::AString m_url;
			Node m_protocal;
			Node m_hostname;
			Node m_port;
			Node m_path;
			Node m_fullparam;
			Node m_fullquery;
			Node m_fragment;
			GAIA::CTN::Vector<Node> m_params;
			GAIA::CTN::Vector<Node> m_queries;
			GAIA::BL m_bAnalyzed;
		};

		/*!
			@brief Http request heads management class.
		*/
		class HttpHead : public GAIA::Base
		{
		public:
			/*!
				@brief Constructor.
			*/
			GINL HttpHead(){this->init();}

			/*!
				@brief Copy contructor.
			*/
			GINL HttpHead(const HttpHead& src){this->init(); this->operator = (src);}

			/*!
				@brief Contructor from char array.
			*/
			GINL HttpHead(const GAIA::CH* psz){this->init(); this->operator = (psz);}

			/*!
				@brief Destructor.
			*/
			GINL ~HttpHead(){this->Reset();}

			/*!
				@brief Check HttpHead is empty or not.

				@return If HttpHead is empty, return GAIA::True, or return GAIA::False.
			*/
			GINL GAIA::BL Empty() const{return this->Size() == 0;}

			/*!
				@brief Fill a HttpHead with a string.

				@param psz [in] Specify the string buffer.

				@param pSize [in] Specify the string size in characters(without '\0').
					If could be GNIL, it means all string which specified by parameter psz are used until to '\0'.

				@return If the string which specified by parameter psz's format is correct, current function will return GAIA::True,
					or will return GAIA::False.

				@remarks
					Support head format like this : 
						a: b\r\n
						c: d\r\n
						e: f\r\n
					The space(' ') after the values is not nessesary.
					The last \r\n is nessesary.
			*/
			GINL GAIA::BL FromString(const GAIA::CH* psz, const GAIA::NUM* pSize = GNIL)
			{
				this->Reset();
				if(GAIA::ALGO::gstremp(psz))
					return GAIA::True;
				const GAIA::CH* p = psz;
				const GAIA::CH* pLast = p;
				GAIA::CTN::ACharsString strName;
				GAIA::CTN::ACharsString strValue;
				while(*p != '\0')
				{
					if(pSize != GNIL && p - psz == *pSize)
						break;
					if(strName.empty() && *p == ':') // Name end.
					{
						if(p - pLast == 0)
						{
							this->Reset();
							return GAIA::False;
						}
						strName.assign(pLast, p - pLast);
						p += 1;
						pLast = p;
					}
					else if(*p == '\r' && *(p + 1) == '\n') // Value end.
					{
						while(*pLast == ' ')
							++pLast;
						if(strName.empty() || p - pLast == 0)
						{
							this->Reset();
							return GAIA::False;
						}
						strValue.assign(pLast, p - pLast);
						this->Set(strName.fptr(), strValue.fptr());
						strName.clear();
						strValue.clear();
						p += 2;
						pLast = p;
					}
					else
						p++;
				}
				if(p != pLast)
				{
					while(*pLast == ' ')
						++pLast;
					if(strName.empty() || p - pLast == 0)
					{
						this->Reset();
						return GAIA::False;
					}
					strValue.assign(pLast, p - pLast);
					this->Set(strName.fptr(), strValue.fptr());
				}
				return GAIA::True;
			}

			/*!
				@brief Get header's string size.

				@return Return current header's string size.

				@remarks
					This string size is : (name length + value length + strlen(" :\r\n")) * (Name-Value pair count).
					If the content are two Name-Value pair, the first one's name is "a", value is "1", the second one's name is "b", value is "2",
					current function will return 12, it calculate by strlen("a: 1\r\nb: 2\r\n").

					If you want convert HttpHead to a string, you perhaps need call this function first,
					and then, you could allocate a buffer with certain and enough size.
					After that, you will call HttpHead::ToString to fill the buffer.
			*/
			GINL GAIA::NUM GetStringSize() const{return m_sStringLen;}

			/*!
				@brief Convert HttpHead's content to string.
					String type like : "a:b\r\nc:d\r\n".

				@param psz [out] Used for saving the result string.

				@param sMaxSize [in] Specify parameter psz's size in characters.
					If sMaxSize is equal GINVALID, it means parameter psz's max buffer size is enough.

				@param pResultSize [out] Used for saving the result string's size in characters(without '\0').
					If it is GNIL, this parameter will be ignored.

				@param pResult [out] Used for saving result, if success, it will be filled GAIA::True, or will be filled GAIA::False.
					If it is GNIL, this parameter will be ignored.

				@return Return the parameter psz.
					If current HttpHead is empty, parameter psz will equal to "".

				@remarks
					You could get the result size by HttpHead::GetStringSize before call this function.
			*/
			GINL GAIA::CH* ToString(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL, GAIA::BL* pResult = GNIL) const
			{
				GAST(psz != GNIL);
				GAST(sMaxSize == GINVALID || sMaxSize > 0);
				if(pResult != GNIL)
					*pResult = GAIA::True;
				if(this->Empty())
				{
					GAST(m_sStringLen == 0);
					*psz = '\0';
					if(pResultSize != GNIL)
						*pResultSize = 0;
					return psz;
				}
				GAIA::CH* p = psz;
				GAIA::NUM sSize = 0;
				for(GAIA::NUM x = 0; x < m_nodes.size(); ++x)
				{
					const Node& n = m_nodes[x];
					GAIA::NUM sNeedSize = GAIA::ALGO::gstrlen(n.pszName) + GAIA::ALGO::gstrlen(n.pszValue) + sizeof(": \r\n\0") - 1;
					if(sMaxSize != GINVALID && sSize + sNeedSize > sMaxSize)
					{
						if(pResult != GNIL)
							*pResult = GAIA::False;
						break;
					}
					sSize += sNeedSize - 1;
					p = GAIA::ALGO::gstradd_notend(p, n.pszName); *p++ = ':'; *p++ = ' ';
					p = GAIA::ALGO::gstradd_notend(p, n.pszValue); *p++ = '\r'; *p++ = '\n';
				}
				*p++ = '\0';
				if(pResultSize != GNIL)
				{
					GAST(sSize == this->GetStringSize());
					*pResultSize = sSize;
				}
				return psz;
			}

			/*!
				@brief Set a Name-Value pair to curernt header.

				@param pszName [in] Specify the name.

				@param pszValue [in] Specify the value.

				@param bNotExistFailed [in]
					If bNotExistFailed is GAIA::True, and the Name-Value pair not exist in current header,
					current function will failed and return GAIA::False.

					If bNotExistFailed is GAIA::False, and Name-Value pair not exist in current header,
					current function will insert a new Name-Value pair to current header.

				@return If insert or update the Name-Value pair success, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL Set(const GAIA::CH* pszName, const GAIA::CH* pszValue, GAIA::BL bNotExistFailed = GAIA::False)
			{
				GAST(!GAIA::ALGO::gstremp(pszName));
				GAST(pszValue != GNIL);
				if(m_bSorted)
				{
					Node finder;
					finder.pszName = pszName;
					GAIA::NUM sFinded = m_nodes.binary_search(finder);
					if(sFinded == GINVALID)
					{
						if(bNotExistFailed)
							return GAIA::False;
						Node n;
						n.pszName = this->requestname(pszName);
						n.pszValue = this->requestvalue(pszValue);
						m_nodes.push_back(n);
						this->increase_string_len(n);
						m_bSorted = GAIA::False;
					}
					else
					{
						Node& n = m_nodes[sFinded];
						this->releasevalue(n.pszValue);
						n.pszValue = this->requestvalue(pszValue);
					}
				}
				else
				{
					GAIA::BL bExist = GAIA::False;
					for(GAIA::NUM x = 0; x < m_nodes.size(); ++x)
					{
						Node& n = m_nodes[x];
						if(GAIA::ALGO::gstrequal(n.pszName, pszName))
						{
							this->releasevalue(n.pszValue);
							n.pszValue = this->requestvalue(pszValue);
							bExist = GAIA::True;
							break;
						}
					}
					if(!bExist)
					{
						if(bNotExistFailed)
							return GAIA::False;
						Node n;
						n.pszName = this->requestname(pszName);
						n.pszValue = this->requestvalue(pszValue);
						m_nodes.push_back(n);
						this->increase_string_len(n);
					}
				}
				return GAIA::True;
			}

			/*!
				@brief Get the value by name.

				@param pszName [in] Specify the Name-Value pair's name.

				@return Return the value which name is parameter pszName.
					If the Name-Value pair is not exist, return GNIL.

				@remarks
					If the Name-Value pair list in current HttpHead is not sorted,
					current function will sort the Name-Value pair first and
					call a binary search algorithm to find the result.
			*/
			GINL const GAIA::CH* Get(const GAIA::CH* pszName) const
			{
				GAST(!GAIA::ALGO::gstremp(pszName));
				GCCAST(HttpHead*)(this)->Optimize();
				Node finder;
				finder.pszName = pszName;
				GAIA::NUM sFinded = m_nodes.binary_search(finder);
				if(sFinded == GINVALID)
					return GNIL;
				return m_nodes[sFinded].pszValue;
			}

			/*!
				@brief Check the Name-Value pair exist or not.

				@param pszName [in] Specify the Name-Value pair's name.

				@return If the Name-Value pair exist in current header, return GAIA::True,
					or will return GAIA::False.

				@remarks
					If the Name-Value pair list in current HttpHead is not sorted,
					current function will sort the Name-Value pair first and
					call a binary search algorithm to check the result.
			*/
			GINL GAIA::BL Exist(const GAIA::CH* pszName) const
			{
				GAST(!GAIA::ALGO::gstremp(pszName));
				return this->Get(pszName) != GNIL;
			}

			/*!
				@brief Delete the Name-Value pair by name.

				@param pszName [in] Specify the Name-Value pair's name.

				@return
					If delete Name-Value pair success, return GAIA::True.
					If the Name-Value pair is not exist in current HttpHead, return GAIA::False.
			*/
			GINL GAIA::BL Delete(const GAIA::CH* pszName)
			{
				GAST(!GAIA::ALGO::gstremp(pszName));
				if(m_bSorted)
				{
					Node finder;
					finder.pszName = pszName;
					GAIA::NUM sFinded = m_nodes.binary_search(finder);
					if(sFinded == GINVALID)
						return GAIA::False;
					Node& n = m_nodes[sFinded];
					this->decrease_string_len(n);
					this->releasevalue(n.pszValue);
					return m_nodes.erase(sFinded);
				}
				else
				{
					for(GAIA::NUM x = 0; x < m_nodes.size(); ++x)
					{
						const Node& n = m_nodes[x];
						if(GAIA::ALGO::gstrequal(n.pszName, pszName))
						{
							this->decrease_string_len(n);
							this->releasevalue(n.pszValue);
							return m_nodes.erase(x);
						}
					}
					return GAIA::False;
				}
			}

			/*!
				@brief Reset current HttpHead, all Name-Value pair will be clear.
			*/
			GINL GAIA::GVOID Reset()
			{
				for(GAIA::NUM x = 0; x < m_nodes.size(); ++x)
				{
					Node& n = m_nodes[x];
					this->releasevalue(n.pszValue);
				}
				m_nodes.clear();
				m_sStringLen = 0;
				m_bSorted = GAIA::True;
			}

			/*!
				@brief Get Name-Value pair count in current HttpHead.

				@return Return Name-Value pair count.

				@remarks
			*/
			GINL GAIA::NUM Size() const{return m_nodes.size();}

			/*!
				@brief Get the Name-Value pair's name by a index.

				@param sIndex [in] Specify the Name-Value pair's index, it belong [0, this->Size).

				@return If Name-Value pair is exist which specified by parameter sIndex,
					current function call will success, and return GAIA::True,
					or will return GAIA::False.
			*/
			GINL const GAIA::CH* GetName(GAIA::NUM sIndex) const
			{
				GAST(sIndex >= 0 && sIndex < m_nodes.size());
				if(sIndex < 0 || sIndex >= m_nodes.size())
					return GNIL;
				return m_nodes[sIndex].pszName;
			}

			/*!
				@brief Get the Name-Value pair's value by a index.

				@param sIndex [in] Specify the Name-Value pair's index, it belong [0, this->size).

				@return If Name-Value pair is exist which specified by parameter sIndex,
					current function call will success, and return GAIA::True,
					or will return GAIA::False.
			*/
			GINL const GAIA::CH* GetValue(GAIA::NUM sIndex) const
			{
				GAST(sIndex >= 0 && sIndex < m_nodes.size());
				if(sIndex < 0 || sIndex >= m_nodes.size())
					return GNIL;
				return m_nodes[sIndex].pszValue;
			}

			/*!
				@brief Get the Name-Value pair's name and value by a index.

				@param sIndex [in] Specify the Name-Value pair's index, it belong [0, this->size).

				@param pszName [out] Used for saving Name-Value pair's name.

				@param pszValue [out] Used for saving Name-Value pair's value.

				@return If Name-Value pair is exist which specified by parameter index,
					current function call will success, and return GAIA::True,
					or will return GAIA::False.
			*/
			GINL GAIA::BL GetNameAndValue(GAIA::NUM sIndex, GAIA::CH** ppName, GAIA::CH** ppValue) const
			{
				GAST(sIndex >= 0 && sIndex < m_nodes.size());
				if(sIndex < 0 || sIndex >= m_nodes.size())
					return GAIA::False;
				const Node& n = m_nodes[sIndex];
				*ppName = (GAIA::CH*)n.pszName;
				*ppValue = (GAIA::CH*)n.pszValue;
				return GAIA::True;
			}

			/*!
				@brief Sort Name-Value pair list.

				@remarks
					If current HttpHead's Name-Value pair list is sorted, current function will do nothing.
			*/
			GINL GAIA::GVOID Optimize()
			{
				if(m_bSorted)
					return;
				m_nodes.sort();
				m_bSorted = GAIA::True;
			}

			/*!
				@brief Deep copy HttpHead from another.

				@param src [in] Specify another HttpHead object.

				@return Return current HttpHead's reference.
			*/
			GINL HttpHead& operator = (const HttpHead& src)
			{
				this->Reset();
				for(GAIA::NUM x = 0; x < src.m_nodes.size(); ++x)
				{
					const Node& n = src.m_nodes[x];
					Node nn;
					nn.pszName = n.pszName;
					nn.pszValue = this->requestvalue(n.pszValue);
					m_nodes.push_back(nn);
				}
				m_sStringLen = src.m_sStringLen;
				m_bSorted = src.m_bSorted;
				return *this;
			}

			/*!
				@brief Fill current HttpHead by a string.

				@param psz [in] Specify a string as source.

				@return Return current HttpHead's reference.

				@remarks
					The string which specified by parameter psz's format must like this:
					"a: 1\r\na: 2\r\n", the ' ' is not nessesary.
			*/
			GINL HttpHead& operator = (const GAIA::CH* psz)
			{
				this->FromString(psz);
				return *this;
			}

			/*!
				@brief Compare current HttpHead object with a string.

				@param psz [in] Specify a string as source to compare.

				@return
					If current HttpHead object below parameter psz, return -1.
					If current HttpHead object above parameter psz, return +1.
					If current HttpHead object equal parameter psz, return 0.
			*/
			GINL GAIA::N32 compare(const GAIA::CH* psz) const
			{
				HttpHead src = psz;
				return this->compare(src);
			}

			/*!
				@brief Compare current HttpHead object with another.

				@param src [in] Specify another HttpHead.

				@return
					If current HttpHead object below parameter src, return -1.
					If current HttpHead object above parameter src, return +1.
					If current HttpHead object equal parameter src, return 0.

				@remarks
			*/
			GINL GAIA::N32 compare(const HttpHead& src) const
			{
				GCCAST(HttpHead*)(this)->Optimize();
				GCCAST(HttpHead*)(&src)->Optimize();
				for(GAIA::NUM x = 0; ; ++x)
				{
					if(x == m_nodes.size())
					{
						if(x == src.m_nodes.size())
							break;
						else
							return -1;
					}
					else if(x == src.m_nodes.size())
						return +1;
					const Node& n1 = m_nodes[x];
					const Node& n2 = src.m_nodes[x];
					GAIA::N32 nCmp = n1.compare(n2);
					if(nCmp != 0)
						return nCmp;
				}
				return 0;
			}
			GCLASS_COMPARE_BYCOMPAREPTR(GAIA::CH)
			GCLASS_COMPARE_BYCOMPARE(HttpHead)

		private:
			class Node : public GAIA::Base
			{
			public:
				GINL GAIA::N32 compare(const Node& src) const{return GAIA::ALGO::gstrcmp(pszName, src.pszName);}
				GCLASS_COMPARE_BYCOMPARE(Node)
			public:
				const GAIA::CH* pszName;
				const GAIA::CH* pszValue;
			};

		private:
			GINL GAIA::GVOID init(){m_sStringLen = 0; m_bSorted = GAIA::False;}
			GINL GAIA::GVOID increase_string_len(const Node& n)
			{
				m_sStringLen += GAIA::ALGO::gstrlen(n.pszName);
				m_sStringLen += GAIA::ALGO::gstrlen(n.pszValue);
				m_sStringLen += sizeof(": \r\n") - 1;
			}
			GINL GAIA::GVOID decrease_string_len(const Node& n)
			{
				m_sStringLen -= GAIA::ALGO::gstrlen(n.pszName);
				m_sStringLen -= GAIA::ALGO::gstrlen(n.pszValue);
				m_sStringLen -= sizeof(": \r\n") - 1;
				GAST(m_sStringLen >= 0);
			}
			const GAIA::CH* requestname(const GAIA::CH* pszName);
			const GAIA::CH* requestvalue(const GAIA::CH* pszValue);
			GAIA::BL releasevalue(const GAIA::CH* pszValue);

		private:
			GAIA::CTN::Vector<Node> m_nodes;
			GAIA::NUM m_sStringLen;
			GAIA::BL m_bSorted;
		};
	}
}

#endif
