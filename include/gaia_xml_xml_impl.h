﻿#ifndef		__GAIA_XML_XML_IMPL_H__
#define		__GAIA_XML_XML_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_locale.h"
#include "gaia_ctn_buffer.h"
#include "gaia_fsys_file.h"
#include "gaia_xml_xmlnode.h"
#include "gaia_xml_xml.h"
#include "gaia_xml_xmlfactory.h"

namespace GAIA
{
	namespace XML
	{
		GINL Xml::Xml()
		{
			m_root.m_pszName = GAIA::XML::XML_DEFAULT_ROOT_NODE_NAME;
			m_root.SetRoot(GAIA::True);
			m_root.SetType(GAIA::XML::XML_NODE_CONTAINER);
		}
		GINL Xml::~Xml()
		{
			this->Reset();
		}
		GINL GAIA::GVOID Xml::Reset()
		{
			if(m_root.m_pszName == GAIA::XML::XML_DEFAULT_ROOT_NODE_NAME)
				m_root.m_pszName = GNIL;
			m_root.Reset();
		}
		GINL GAIA::BL Xml::LoadFromFile(const GAIA::TCH* pszFileName)
		{
			GAST(!GAIA::ALGO::gstremp(pszFileName));
			if(GAIA::ALGO::gstremp(pszFileName))
				return GAIA::False;

			GAIA::FSYS::File f;
			if(!f.Open(pszFileName, GAIA::FSYS::File::OPEN_TYPE_READ))
				return GAIA::False;

			this->Reset();

			m_strTempW.reserve(1024);

			GAIA::CTN::Buffer buf;

			GAIA::XML::Xml::Cursor cur;
			for(;;)
			{
				buf.resize(1024 * 64);
				GAIA::NUM sReaded = (GAIA::NUM)f.Read(buf.fptr() + buf.read_size(), buf.write_size() - buf.read_size());
				if(sReaded <= 0)
				{
					buf.seek_write(buf.read_size());
					buf.seek_read(0);
					break;
				}
				buf.seek_write(buf.read_size() + sReaded);
				buf.seek_read(0);

				for(;;)
				{
					GAIA::XML::XML_NODE nt;
					GAIA::U8* pBegin;
					GAIA::U8* pEnd;
					GAIA::BL bBegin;
					GAIA::NUM sOffset = this->load_node(buf.read_ptr(), buf.remain(), nt, pBegin, pEnd, bBegin);
					if(sOffset <= 0)
						break;
					else
					{
						this->read_string(pBegin, pEnd, m_strTempW);
						buf.seek_read(sOffset, GAIA::SEEK_TYPE_FORWARD);
						switch(nt)
						{
						case GAIA::XML::XML_NODE_CONTAINER:
							{
								if(bBegin)
									this->BeginWriteNode(cur, nt, m_strTempW.fptr());
								else
								{
									if(cur.pCurrentNode != GNIL)
										cur.pCurrentNode->SetType(GAIA::XML::XML_NODE_CONTAINER);
									this->EndWriteNode(cur);
								}
							}
							break;
						case GAIA::XML::XML_NODE_MULTICONTAINER:
							{
								if(bBegin)
									this->BeginWriteNode(cur, nt, m_strTempW.fptr());
								else
								{
									if(cur.pCurrentNode != GNIL)
										cur.pCurrentNode->SetType(GAIA::XML::XML_NODE_MULTICONTAINER);
									this->EndWriteNode(cur);
								}
							}
							break;
						case GAIA::XML::XML_NODE_NAME:
							{
								this->BeginWriteNode(cur, nt, m_strTempW.fptr());
							}
							break;
						case GAIA::XML::XML_NODE_VALUE:
							{
								this->WriteNode(cur, nt, m_strTempW.fptr());
								this->EndWriteNode(cur);
							}
							break;
						case GAIA::XML::XML_NODE_HEAD:
							break;
						case GAIA::XML::XML_NODE_COMMENT:
							{
								this->WriteNode(cur, nt, m_strTempW.fptr());
							}
							break;
						default:
							GASTFALSE;
							m_strTempW.destroy();
							return GAIA::False;
						}
						if(buf.remain() == 0)
							break;
					}
				}
				buf.keep_remain();
			}

			m_strTempW.destroy();

			if(buf.remain() != 0)
				return GAIA::False;

			return GAIA::True;
		}
		GINL GAIA::BL Xml::SaveToFile(const GAIA::TCH* pszFileName, GAIA::XML::XML_SAVE st)
		{
			GAST(!GAIA::ALGO::gstremp(pszFileName));
			if(GAIA::ALGO::gstremp(pszFileName))
				return GAIA::False;

			GAST(st == GAIA::XML::XML_SAVE_BESTSIZE ||
				 st == GAIA::XML::XML_SAVE_BESTREAD);
			if(st != GAIA::XML::XML_SAVE_BESTSIZE &&
					st != GAIA::XML::XML_SAVE_BESTREAD)
				return GAIA::False;

			GAIA::FSYS::File f;
			if(!f.Open(pszFileName, GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS | GAIA::FSYS::File::OPEN_TYPE_WRITE))
				return GAIA::False;

			m_strTempA.reserve(1024);

			static const GAIA::CH XML_FILE_HEADER[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
			this->write_string(st, f, XML_FILE_HEADER, sizeof(XML_FILE_HEADER) - sizeof(XML_FILE_HEADER[0]));
			this->write_linebreak(st, f);
			this->save_node(m_root, st, f);

			m_strTempA.destroy();

			return GAIA::True;
		}
		GINL GAIA::BL Xml::LoadFromMem(const GAIA::TCH* pszSrc, const GAIA::NUM& sSize)
		{
			GAST(pszSrc != GNIL);
			if(pszSrc == GNIL)
				return GAIA::False;

			GAST(sSize > 0 && sSize <= 1024 * 1024 * 1024);
			if(sSize <= 0 || sSize > 1024 * 1024 * 1024)
				return GAIA::False;

			return GAIA::True;
		}
		GINL GAIA::BL Xml::SaveToMem(GAIA::TCH* pszDst, const GAIA::NUM& sSize, GAIA::XML::XML_SAVE st)
		{
			GAST(pszDst != GNIL);
			if(pszDst == GNIL)
				return GAIA::False;

			GAST(sSize > 0 && sSize <= 1024 * 1024 * 1024);
			if(sSize <= 0 || sSize > 1024 * 1024 * 1024)
				return GAIA::False;

			GAST(st == GAIA::XML::XML_SAVE_BESTSIZE ||
				 st == GAIA::XML::XML_SAVE_BESTREAD);
			if(st != GAIA::XML::XML_SAVE_BESTSIZE &&
					st != GAIA::XML::XML_SAVE_BESTREAD)
				return GAIA::False;

			return GAIA::True;
		}
		GINL XmlNode& Xml::GetRootNode()
		{
			return m_root;
		}
		GINL const XmlNode& Xml::GetRootNode() const
		{
			return m_root;
		}
		GINL GAIA::BL Xml::BeginWriteNode(GAIA::XML::Xml::Cursor& cur, GAIA::XML::XML_NODE nt, const GAIA::TCH* pszNodeName)
		{
			if(cur.pCurrentNode == GNIL)
			{
				cur.pCurrentNode = &m_root;
				cur.pCurrentNode->SetType(nt);
				cur.pCurrentNode->SetName(pszNodeName);
			}
			else
			{
				GAIA::XML::XmlFactory* pFactory = this->GetFactory();
				GAIA::XML::XmlNode* pNewNode = (pFactory != GNIL) ? (&pFactory->CreateNode()) : (gnew GAIA::XML::XmlNode);
				pNewNode->SetType(nt);
				pNewNode->SetName(pszNodeName);

				if(cur.pCurrentNode->InsertChild(*pNewNode) == GINVALID)
				{
					if(pFactory != GNIL)
						pFactory->ReleaseNode(*pNewNode);
					else
						gdel pNewNode;
					return GAIA::False;
				}
				cur.pCurrentNode = pNewNode;
			}
			return GAIA::True;
		}
		GINL GAIA::BL Xml::EndWriteNode(GAIA::XML::Xml::Cursor& cur)
		{
			if(cur.pCurrentNode == GNIL)
				return GAIA::False;
			cur.pCurrentNode = cur.pCurrentNode->GetParent();
			return GAIA::True;
		}
		GINL GAIA::BL Xml::WriteNode(GAIA::XML::Xml::Cursor& cur, GAIA::XML::XML_NODE nt, const GAIA::TCH* pszNodeName)
		{
			if(!this->BeginWriteNode(cur, nt, pszNodeName))
				return GAIA::False;
			if(!this->EndWriteNode(cur))
				return GAIA::False;
			return GAIA::True;
		}
		GINL const GAIA::TCH* Xml::BeginReadNode(GAIA::XML::Xml::Cursor& cur) const
		{
			return GNIL;
		}
		GINL GAIA::BL Xml::EndReadNode(GAIA::XML::Xml::Cursor& cur) const
		{
			return GAIA::True;
		}
		GINL const GAIA::TCH* Xml::ReadNode(GAIA::XML::Xml::Cursor& cur) const
		{
			return GNIL;
		}
		GINL GAIA::GVOID Xml::write_linebreak(GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f)
		{
			if(st != GAIA::XML::XML_SAVE_BESTREAD)
				return;
			f.WriteText(GAIA_FILELINEBREAK);
		}
		GINL GAIA::GVOID Xml::write_indent(GAIA::XML::XmlNode& n, GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f)
		{
			if(st != GAIA::XML::XML_SAVE_BESTREAD)
				return;
			GAIA::NUM sDepth = n.GetDepth();
			GAST(sDepth >= 0);
			if(sDepth <= 0)
				return;
			GAIA::CH szTemp[64];
			if(sDepth > sizeof(szTemp))
				sDepth = 64;
			for(GAIA::NUM x = 0; x < sDepth; ++x)
				szTemp[x] = '\t';
			this->write_string(st, f, szTemp, sDepth);
		}
		GINL GAIA::GVOID Xml::write_string(GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f, const GAIA::CH* p, GAIA::NUM sLen)
		{
			GAST(!GAIA::ALGO::gstremp(p));
			if(GAIA::ALGO::gstremp(p))
				return;
			if(sLen == GINVALID)
				sLen = GAIA::ALGO::gstrlen(p);
			if(sLen == 0)
				return;
			f.Write(p, sLen);
		}
		GINL GAIA::GVOID Xml::write_string(GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f, const GAIA::WCH* p, GAIA::NUM sLen)
		{
			GAST(!GAIA::ALGO::gstremp(p));
			if(GAIA::ALGO::gstremp(p))
				return;
			if(sLen == GINVALID)
				sLen = GAIA::ALGO::gstrlen(p);
			if(sLen == 0)
				return;
		#if GAIA_CHARSET == GAIA_CHARSET_UNICODE
			GAIA::NUM sMaxUtf8Len = sLen * 6;
			m_strTempA.resize(sMaxUtf8Len);
			GAIA::NUM sLenUTF8 = GAIA::LOCALE::w2m(p, sLen, m_strTempA.fptr(), sMaxUtf8Len, GAIA::CHARSET_TYPE_UTF8);
			if(sLenUTF8 > 0)
				f.Write(m_strTempA.fptr(), sLenUTF8);
		#elif GAIA_CHARSET == GAIA_CHARSET_ANSI
			f.Write(p, sLen);
		#endif
		}
		GINL GAIA::GVOID Xml::save_node(GAIA::XML::XmlNode& n, GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f)
		{
			switch(n.GetType())
			{
			case GAIA::XML::XML_NODE_CONTAINER:
				{
					GAIA::NUM sLen = GAIA::ALGO::gstrlen(n.GetName());

					this->write_indent(n, st, f);
					this->write_string(st, f, "<", 1);
					this->write_string(st, f, n.GetName(), sLen);
					this->save_child_node_value(n, st, f);
					this->write_string(st, f, "/>", 2);
					this->write_linebreak(st, f);
				}
				break;
			case GAIA::XML::XML_NODE_MULTICONTAINER:
				{
					GAIA::NUM sLen = GAIA::ALGO::gstrlen(n.GetName());

					this->write_indent(n, st, f);
					this->write_string(st, f, "<", 1);
					this->write_string(st, f, n.GetName(), sLen);
					this->save_child_node_value(n, st, f);
					this->write_string(st, f, ">", 1);
					this->write_linebreak(st, f);

					this->save_child_node(n, st, f);

					this->write_indent(n, st, f);
					this->write_string(st, f, "</", 2);
					this->write_string(st, f, n.GetName(), sLen);
					this->write_string(st, f, ">", 1);
					this->write_linebreak(st, f);
				}
				break;
			case GAIA::XML::XML_NODE_NAME:
				{
					this->write_string(st, f, n.GetName(), GINVALID);
					this->write_string(st, f, "=", 1);
					this->save_child_node_value(n, st, f);
				}
				break;
			case GAIA::XML::XML_NODE_VALUE:
				{
					GAIA::NUM sLen = GAIA::ALGO::gstrlen(n.GetName());
					this->write_string(st, f, "\"", 1);
					this->write_string(st, f, n.GetName(), sLen);
					this->write_string(st, f, "\"", 1);
				}
				break;
			case GAIA::XML::XML_NODE_COMMENT:
				{
					GAIA::NUM sLen = GAIA::ALGO::gstrlen(n.GetName());

					this->write_indent(n, st, f);
					this->write_string(st, f, "<!--", 4);
					this->write_string(st, f, n.GetName(), sLen);
					this->write_string(st, f, "-->", 3);
					this->write_linebreak(st, f);
				}
				break;
			default:
				GASTFALSE;
				break;
			}
		}
		GINL GAIA::GVOID Xml::save_child_node(GAIA::XML::XmlNode& n, GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f)
		{
			for(GAIA::NUM x = 0; x < n.GetChildCount(); ++x)
			{
				GAIA::XML::XmlNode* pChild = n.GetChild(x);
				if(pChild == GNIL)
					continue;
				GAIA::XML::XML_NODE childtype = pChild->GetType();
				if(childtype != GAIA::XML::XML_NODE_CONTAINER &&
						childtype != GAIA::XML::XML_NODE_MULTICONTAINER &&
						childtype != GAIA::XML::XML_NODE_COMMENT)
					continue;
				this->save_node(*pChild, st, f);
			}
		}
		GINL GAIA::GVOID Xml::save_child_node_value(GAIA::XML::XmlNode& n, GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f)
		{
			for(GAIA::NUM x = 0; x < n.GetChildCount(); ++x)
			{
				GAIA::XML::XmlNode* pChild = n.GetChild(x);
				if(pChild == GNIL)
					continue;
				GAIA::XML::XML_NODE childtype = pChild->GetType();
				if(childtype != GAIA::XML::XML_NODE_NAME &&
						childtype != GAIA::XML::XML_NODE_VALUE)
					continue;
				if(childtype == GAIA::XML::XML_NODE_NAME)
					this->write_string(st, f, " ", 1);
				this->save_node(*pChild, st, f);
			}
		}
		GINL GAIA::NUM Xml::load_node(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::XML::XML_NODE& nt, GAIA::U8*& pBegin, GAIA::U8*& pEnd, GAIA::BL& bBegin) const
		{
			GAST(pBuf != GNIL);
			if(pBuf == GNIL)
				return GINVALID;
			GAST(sSize > 0);
			if(sSize <= 0)
				return GINVALID;
			for(GAIA::NUM x = 0; x < sSize; ++x)
			{
				if(pBuf[x] == '<')
				{
					if(x + 1 >= sSize)
						return GINVALID;
					if(pBuf[x + 1] == '?') // Head begin.
					{
						if(x + 4 >= sSize)
							return GINVALID;
						GAIA::NUM sRet;
						GAIA::NUM sLen = this->load_head(pBuf, sSize, x + 2, sRet);
						if(sLen > 0)
						{
							pBegin = pBuf + x + 2;
							pEnd = pBegin + sLen;
							nt = GAIA::XML::XML_NODE_HEAD;
							bBegin = GAIA::True;
							return sRet;
						}
						else
							return GINVALID;
					}
					else if(pBuf[x + 1] == '/') // Multi-Container node end.
					{
						if(x + 3 >= sSize)
							return GINVALID;
						GAIA::NUM sRet;
						GAIA::NUM sLen = this->load_node_name(pBuf, sSize, x + 2, sRet);
						if(sLen > 0)
						{
							pBegin = pBuf + x + 2;
							pEnd = pBegin + sLen;
							nt = GAIA::XML::XML_NODE_MULTICONTAINER;
							bBegin = GAIA::False;
							return sRet;
						}
						else
							return GINVALID;
					}
					else if(pBuf[x + 1] == '!') // Commen node.
					{
						if(x + 6 >= sSize)
							return GINVALID;
						if(pBuf[x + 2] != '-' || pBuf[x + 3] != '-')
							return GINVALID;
						GAIA::NUM sRet;
						GAIA::NUM sLen = this->load_comment(pBuf, sSize, x + 4, sRet);
						if(sLen > 0)
						{
							pBegin = pBuf + x + 4;
							pEnd = pBegin + sLen;
							nt = GAIA::XML::XML_NODE_COMMENT;
							bBegin = GAIA::True;
							return sRet;
						}
						else
							return GINVALID;
					}
					else // Container or Multi-Container node begin.
					{
						GAIA::NUM sRet;
						GAIA::NUM sLen = this->load_node_name(pBuf, sSize, x + 1, sRet);
						if(sLen > 0)
						{
							pBegin = pBuf + x + 1;
							pEnd = pBegin + sLen;
							nt = GAIA::XML::XML_NODE_MULTICONTAINER;
							bBegin = GAIA::True;
							return sRet;
						}
						else
							return GINVALID;
					}
				}
				else if(pBuf[x] == '/') // Container node end.
				{
					if(x + 1 >= sSize)
						return GINVALID;
					if(pBuf[x + 1] != '>')
						return GINVALID;
					pBegin = pEnd = GNIL;
					bBegin = GAIA::False;
					nt = GAIA::XML::XML_NODE_CONTAINER;
					return x + 2;
				}
				else if(pBuf[x] == '\"') // Value node.
				{
					if(x + 1 >= sSize)
						return GINVALID;
					GAIA::NUM sRet;
					GAIA::NUM sLen = this->load_value(pBuf, sSize, x + 1, sRet);
					if(sLen > 0)
					{
						pBegin = pBuf + x + 1;
						pEnd = pBegin + sLen;
						nt = GAIA::XML::XML_NODE_VALUE;
						bBegin = GAIA::True;
						return sRet;
					}
					else
						return GINVALID;
				}
				else if(GAIA::ALGO::isblank(pBuf[x]) || GAIA::ALGO::isspecial(pBuf[x]))
				{
				}
				else if(pBuf[x] == '>') // Multi container end.
				{
				}
				else // Name node.
				{
					GAIA::NUM sRet;
					GAIA::NUM sLen = this->load_name(pBuf, sSize, x, sRet);
					if(sLen > 0)
					{
						pBegin = pBuf + x;
						pEnd = pBegin + sLen;
						nt = GAIA::XML::XML_NODE_NAME;
						bBegin = GAIA::True;
						return sRet;
					}
					else
						return GINVALID;
				}
			}
			return GINVALID;
		}
		GINL GAIA::NUM Xml::load_head(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const
		{
			for(GAIA::NUM x = sOffset; x < sSize; ++x)
			{
				if(pBuf[x] == '>')
				{
					if(x - sOffset > 1)
					{
						if(pBuf[x - 1] == '?')
						{
							sNext = x + 1;
							return x - sOffset - 1;
						}
					}
				}
			}
			return GINVALID;
		}
		GINL GAIA::NUM Xml::load_comment(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const
		{
			for(GAIA::NUM x = sOffset; x < sSize; ++x)
			{
				if(pBuf[x] == '>')
				{
					if(x - sOffset > 1)
					{
						if(pBuf[x - 1] == '-' && pBuf[x - 2] == '-')
						{
							sNext = x + 1;
							return x - sOffset - 2;
						}
					}
				}
			}
			return GINVALID;
		}
		GINL GAIA::NUM Xml::load_node_name(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const
		{
			for(GAIA::NUM x = sOffset; x < sSize; ++x)
			{
				if(GAIA::ALGO::isblank(pBuf[x]) || GAIA::ALGO::isspecial(pBuf[x]))
				{
					sNext = x + 1;
					return x - sOffset;
				}
			}
			return GINVALID;
		}
		GINL GAIA::NUM Xml::load_name(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const
		{
			for(GAIA::NUM x = sOffset; x < sSize; ++x)
			{
				if(GAIA::ALGO::isblank(pBuf[x]) || GAIA::ALGO::isspecial(pBuf[x]) || pBuf[x] == '=')
				{
					sNext = x + 1;
					return x - sOffset;
				}
			}
			return GINVALID;
		}
		GINL GAIA::NUM Xml::load_value(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const
		{
			for(GAIA::NUM x = sOffset; x < sSize; ++x)
			{
				if(pBuf[x] == '\"')
				{
					sNext = x + 1;
					return x - sOffset;
				}
			}
			return GINVALID;
		}
		GINL GAIA::GVOID Xml::read_string(const GAIA::U8* pBegin, const GAIA::U8* pEnd, GAIA::CTN::WString& strResult) const
		{
			GAST(pEnd - pBegin > 0);
			strResult.resize(GSCAST(GAIA::NUM)(pEnd - pBegin));
			GAIA::NUM sLenWChar = GAIA::LOCALE::m2w(pBegin, GSCAST(GAIA::NUM)(pEnd - pBegin), strResult.fptr(), strResult.size(), GAIA::CHARSET_TYPE_UTF8);
			strResult.resize(sLenWChar);
		}
	}
}

#endif
