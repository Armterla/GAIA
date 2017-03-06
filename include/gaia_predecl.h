#ifndef		__GAIA_PREDECL_H__
#define		__GAIA_PREDECL_H__

namespace GAIA
{
	namespace STREAM
	{
		class StreamFormat;
		class StreamBase;
		class STDStream;
		class StringStream;
	}

	namespace FSYS
	{
		class Dir;
		class File;
	}

	namespace NETWORK
	{
		class IP;
		class Addr;
		class Socket;
		class AsyncSocket;
		class AsyncDispatcher;
		class HttpURL;
		class HttpHead;
		class HttpRequest;
		class Http;
		class HttpServerLink;
		class HttpServerCallBack;
		class HttpServer;
		class Client;
		class Server;
	}

	namespace XML
	{
		class XMLReader;
		class XMLWriter;
		class XMLFactoryDesc;
		class XMLFmt;
		class XMLProp;
		class XMLNode;
		class XML;
		class XMLFactory;
	}

	namespace HTML
	{
		class HTMLFactoryDesc;
		class HTMLFmt;
		class HTMLProp;
		class HTMLNode;
		class HTML;
		class HTMLFactory;
	}

	namespace JSON
	{
		class JsonReader;
		class JsonWriter;
		class JsonFactoryDesc;
		class JsonProp;
		class JsonNode;
		class Json;
		class JsonFactory;
	}

	namespace TEST
	{
		class Case;
		class Controller;
	}

	namespace STATE
	{
		class StateCtn;
		class StateMgr;
	}

	namespace TIME
	{
		class TimerMgr;
	}

	namespace DB
	{
		class DBLocalQuery;
		class DBLocal;
		class DBRemoteQuery;
		class DBRemote;
	}

	namespace IMG
	{
		class ImageBase;
		class ImageJPEG;
		class ImagePNG;
		class Image;
	}

	namespace DOC
	{
		class PDFWriter;
	}
}

#endif
