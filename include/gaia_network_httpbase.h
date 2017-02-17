#ifndef	 __GAIA_NETWORK_HTTPBASE_H__
#define	 __GAIA_NETWORK_HTTPBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
//#include "gaia_sync_lock.h"
//#include "gaia_sync_autolock.h"
//#include "gaia_sync_lockrw.h"
//#include "gaia_sync_autolockr.h"
//#include "gaia_sync_autolockw.h"
#include "gaia_algo_string.h"
//#include "gaia_ctn_ref.h"
#include "gaia_ctn_string.h"
#include "gaia_ctn_vector.h"
//#include "gaia_ctn_queue.h"
//#include "gaia_ctn_buffer.h"
//#include "gaia_ctn_pool.h"
//#include "gaia_ctn_list.h"
//#include "gaia_ctn_set.h"
//#include "gaia_network_ip.h"
//#include "gaia_network_addr.h"
//#include "gaia_network_base.h"
//#include "gaia_network_socket.h"

namespace GAIA
{
	namespace NETWORK
	{
		static const GAIA::N32 HTTP_CODE_OK = 200;
		static const GAIA::N32 HTTP_CODE_BADREQUEST = 400;
		static const GAIA::N32 HTTP_CODE_UNAUTHORIZED = 401;
		static const GAIA::N32 HTTP_CODE_FORBIDDEN = 403;
		static const GAIA::N32 HTTP_CODE_NOTFOUND = 404;
		static const GAIA::N32 HTTP_CODE_INTERNALSERVERERROR = 500;
		static const GAIA::N32 HTTP_CODE_SERVERUNAVAILABLE = 503;
		static const GAIA::N32 HTTP_CODE_LIST[] = 
		{
			HTTP_CODE_OK,
			HTTP_CODE_BADREQUEST,
			HTTP_CODE_UNAUTHORIZED,
			HTTP_CODE_FORBIDDEN,
			HTTP_CODE_NOTFOUND,
			HTTP_CODE_INTERNALSERVERERROR,
			HTTP_CODE_SERVERUNAVAILABLE,
		};

		static const GAIA::CH* HTTP_HEADNAME_ACCEPTRANGES = "Accept-Ranges";
		static const GAIA::CH* HTTP_HEADNAME_CONNECTION = "Connection";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTLENGTH = "Content-Length";
		static const GAIA::CH* HTTP_HEADNAME_CONTENTTYPE = "Content-Type";
		static const GAIA::CH* HTTP_HEADNAME_DATE = "Date";
		static const GAIA::CH* HTTP_HEADNAME_HOST = "Host";
		static const GAIA::CH* HTTP_HEADNAME_LASTMODIFIED = "Last-Modified";
		static const GAIA::CH* HTTP_HEADNAME_SERVER = "Server";
		static const GAIA::CH* HTTP_HEADNAME_SETCOOKIE = "Set-Cookie";
		static const GAIA::CH* HTTP_HEADNAME_USERAGENT = "User-Agent";
		static const GAIA::CH* HTTP_HEADNAME_LIST[] = 
		{
			HTTP_HEADNAME_ACCEPTRANGES,
			HTTP_HEADNAME_CONNECTION,
			HTTP_HEADNAME_CONTENTLENGTH,
			HTTP_HEADNAME_CONTENTTYPE,
			HTTP_HEADNAME_DATE,
			HTTP_HEADNAME_HOST,
			HTTP_HEADNAME_LASTMODIFIED,
			HTTP_HEADNAME_SERVER,
			HTTP_HEADNAME_SETCOOKIE,
			HTTP_HEADNAME_USERAGENT,
		};

		class HttpURL : public GAIA::Base
		{
		public:
			GINL HttpURL();
			GINL HttpURL(const HttpURL& src);
			GINL HttpURL(const GAIA::CH* psz);
			GINL ~HttpURL();
			GINL GAIA::BL FromString(const GAIA::CH* psz);
			GINL GAIA::GVOID ToString(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const;
			GINL GAIA::CH* GetProtocal(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const;
			GINL GAIA::CH* GetHostName(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const;
			GINL GAIA::CH* GetPort(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const;
			GINL GAIA::CH* GetPath(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const;
			GINL GAIA::CH* GetFullQuery(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const;
			GINL GAIA::BL GetQuery(GAIA::NUM sIndex, GAIA::CH* pszName, GAIA::CH* pszValue, GAIA::NUM sMaxNameSize = GINVALID, GAIA::NUM sMaxValueSize = GINVALID, GAIA::NUM* pNameResultSize = GNIL, GAIA::NUM* pValueResultSize = GNIL) const;
			GINL GAIA::CH* GetQueryByName(const GAIA::CH* pszName, GAIA::CH* pszValue, GAIA::NUM sMaxValueSize, GAIA::NUM* pValueResultSize = GNIL) const;
			GINL GAIA::CH* GetFragment(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const;
			GINL HttpURL& operator = (const HttpURL& src);
			GINL HttpURL& operator = (const GAIA::CH* psz);
			GINL operator const GAIA::CH*(){return m_url.fptr();}
			GINL GAIA::N32 compare(const GAIA::CH* psz) const;
			GINL GAIA::N32 compare(const HttpURL& src) const;
			GCLASS_COMPARE_BYCOMPARE(HttpURL)

		private:
			class Node : public GAIA::Base
			{
			public:
				const GAIA::CH* psz;
				GAIA::NUM sLen;
			};

		private:
			GAIA::CTN::AString m_url;
			GAIA::CTN::Vector<Node> m_nodes;
		};

		class HttpHead : public GAIA::Base
		{
		public:
			GINL HttpHead(){this->init();}
			GINL HttpHead(const HttpHead& src){this->init(); this->operator = (src);}
			GINL HttpHead(const GAIA::CH* psz){this->init(); this->operator = (psz);}
			GINL ~HttpHead(){this->DeleteAll();}
			GINL GAIA::BL FromString(const GAIA::CH* psz)
			{

			}
			GINL GAIA::GVOID ToString(GAIA::CH* psz, GAIA::NUM sMaxSize = GINVALID, GAIA::NUM* pResultSize = GNIL) const
			{

			}
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
					}
				}
				return GAIA::True;
			}
			GINL const GAIA::CH* Get(const GAIA::CH* pszName) const
			{
				GAST(!GAIA::ALGO::gstremp(pszName));
				GCCAST(HttpHead*)(this)->sortnodes();
				Node finder;
				finder.pszName = pszName;
				GAIA::NUM sFinded = m_nodes.binary_search(finder);
				if(sFinded == GINVALID)
					return GNIL;
				return m_nodes[sFinded].pszName;
			}
			GINL GAIA::BL Exist(const GAIA::CH* pszName) const
			{
				GAST(!GAIA::ALGO::gstremp(pszName));
				return this->Get(pszName) != GNIL;
			}
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
					this->releasevalue(m_nodes[sFinded].pszValue);
					return m_nodes.erase(sFinded);
				}
				else
				{
					for(GAIA::NUM x = 0; x < m_nodes.size(); ++x)
					{
						const Node& n = m_nodes[x];
						if(GAIA::ALGO::gstrequal(n.pszName, pszName))
						{
							this->releasevalue(n.pszValue);
							return m_nodes.erase(x);
						}
					}
					return GAIA::False;
				}
			}
			GINL GAIA::GVOID DeleteAll()
			{
				for(GAIA::NUM x = 0; x < m_nodes.size(); ++x)
				{
					Node& n = m_nodes[x];
					this->releasevalue(n.pszValue);
				}
				m_nodes.clear();
				m_bSorted = GAIA::True;
			}
			GINL GAIA::NUM Count() const{return m_nodes.size();}
			GINL const GAIA::CH* GetName(GAIA::NUM sIndex) const
			{
				GAST(sIndex >= 0 && sIndex < m_nodes.size());
				if(sIndex < 0 || sIndex >= m_nodes.size())
					return GNIL;
				return m_nodes[sIndex].pszName;
			}
			GINL const GAIA::CH* GetValue(GAIA::NUM sIndex) const
			{
				GAST(sIndex >= 0 && sIndex < m_nodes.size());
				if(sIndex < 0 || sIndex >= m_nodes.size())
					return GNIL;
				return m_nodes[sIndex].pszValue;
			}
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
			GINL HttpHead& operator = (const HttpHead& src)
			{
				this->DeleteAll();
				for(GAIA::NUM x = 0; x < src.m_nodes.size(); ++x)
				{
					const Node& n = src.m_nodes[x];
					Node nn;
					nn.pszName = n.pszName;
					nn.pszValue = this->requestvalue(n.pszValue);
					m_nodes.push_back(nn);
				}
				m_bSorted = src.m_bSorted;
				return *this;
			}
			GINL HttpHead& operator = (const GAIA::CH* psz)
			{
				return *this;
			}
			GINL GAIA::N32 compare(const GAIA::CH* psz) const
			{

			}
			GINL GAIA::N32 compare(const HttpHead& src) const
			{

			}
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
			GINL GAIA::GVOID init(){m_bSorted = GAIA::False;}
			GINL GAIA::GVOID sortnodes()
			{
				if(m_bSorted)
					return;
				m_nodes.sort();
				m_bSorted = GAIA::True;
			}
			GINL const GAIA::CH* requestname(const GAIA::CH* pszName);
			GINL const GAIA::CH* requestvalue(const GAIA::CH* pszValue);
			GINL GAIA::BL releasevalue(const GAIA::CH* pszValue);

		private:
			GAIA::CTN::Vector<Node> m_nodes;
			GAIA::BL m_bSorted;
		};
	}
}

#endif
