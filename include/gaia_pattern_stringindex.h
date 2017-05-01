#ifndef 	__GAIA_PATTERN_STRINGINDEX_H__
#define 	__GAIA_PATTERN_STRINGINDEX_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_search.h"
#include "gaia_ctn_pair.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_pool.h"

namespace GAIA
{
	namespace PATTERN
	{
		class StringIndex : public GAIA::Base
		{
		private:
			class Node : public GAIA::Base
			{
			public:
				GAIA::NUM sIndex;
				GAIA::NUM sKeyOffset;
				GAIA::NUM sKeyLen;
				const GAIA::GVOID* pData;
			};
			class Weight : public GAIA::Base
			{
			public:
				GAIA::N32 compare(const Weight& src) const
				{
					if(sWeight < src.sWeight)
						return +1;
					else if(sWeight > src.sWeight)
						return -1;
					return 0;
				}
				GCLASS_COMPARE_BYCOMPARE(Weight)
			public:
				Node* pNode;
				GAIA::NUM sWeight;
			};
		public:
			class FindResult : public GAIA::Base
			{
				friend class StringIndex;
			private:
				GAIA::GVOID clear()
				{
					listSite.clear();
					listConn.clear();
					listNode.clear();
					listNodeTemp.clear();
					listWeight.clear();
					listResult.clear();
				}
			private:
				GAIA::CTN::Vector<GAIA::CTN::Pair<GAIA::NUM, GAIA::NUM> > listSite; // First element is the node count, second is the site hash(site index)
				GAIA::CTN::Vector<GAIA::CTN::Pair<GAIA::NUM, GAIA::NUM> > listConn; // First element is the node count, second is the conn hash(conn index)
				GAIA::CTN::Vector<Node*> listNode;
				GAIA::CTN::Vector<Node*> listNodeTemp;
				GAIA::CTN::Vector<Weight> listWeight;
			public:
				GAIA::CTN::Vector<GAIA::NUM> listResult;
			};
			
		public:
			StringIndex()
			{
				m_site.resize(65536);
				m_conn.resize(65536);
				m_bOptimized = GAIA::True;
			}
			~StringIndex(){}
			
			GAIA::BL empty() const{return this->size() == 0;}
			GAIA::NUM size() const{return m_nodes.size();}
			GAIA::GVOID clear()
			{
				m_keybuf.clear();
				m_nodepool.clear();
				m_nodes.clear();
				for(GAIA::NUM x = 0; x < m_site.size(); ++x)
					m_site[x].clear();
				for(GAIA::NUM x = 0; x < m_conn.size(); ++x)
					m_conn[x].clear();
				m_bOptimized = GAIA::True;
			}
			GAIA::GVOID destroy()
			{
				m_keybuf.destroy();
				m_nodepool.destroy();
				m_nodes.destroy();
				for(GAIA::NUM x = 0; x < m_site.size(); ++x)
					m_site[x].destroy();
				for(GAIA::NUM x = 0; x < m_conn.size(); ++x)
					m_conn[x].destroy();
				m_bOptimized = GAIA::True;
			}
			
			GAIA::NUM insert(const GAIA::U32* pKey, GAIA::NUM sKeyLen, const GAIA::GVOID* pData)
			{
				Node* pNode = m_nodepool.alloc();
				pNode->sIndex = m_nodes.size();
				pNode->sKeyOffset = m_keybuf.size();
				pNode->sKeyLen = sKeyLen;
				pNode->pData = pData;
				m_nodes.push_back(pNode);
				m_keybuf.push_back(pKey, sKeyLen);
				
				for(GAIA::NUM x = 0; x < sKeyLen; ++x)
				{
					GAIA::NUM sSiteHash = this->sitehash(pKey[x]);
					m_site[sSiteHash].push_back(pNode);
				}
				
				GAIA::NUM sConnLen = sKeyLen - 1;
				for(GAIA::NUM x = 0; x < sConnLen; ++x)
				{
					GAIA::NUM sConnHash = this->connhash(pKey[x], pKey[x + 1]);
					m_conn[sConnHash].push_back(pNode);
				}
				
				m_bOptimized = GAIA::False;
				return pNode->sIndex;
			}
			GAIA::GVOID find(const GAIA::U32* pMatcher, GAIA::NUM sMatcherLen, FindResult& r) const
			{
				GCCAST(StringIndex*)(this)->optimize();
				
				r.clear();
				
				// Collect sites.
				for(GAIA::NUM x = 0; x < sMatcherLen; ++x)
				{
					GAIA::NUM sSiteHash = this->sitehash(pMatcher[x]);
					const GAIA::CTN::Vector<Node*>& v = m_site[sSiteHash];
					if(v.empty())
						continue;
					GAIA::CTN::Pair<GAIA::NUM, GAIA::NUM> pr(v.size(), sSiteHash);
					r.listSite.push_back(pr);
				}
				r.listSite.sort();
				r.listSite.unique();
				if(r.listSite.empty())
					return;
				
				// Collect conns.
				GAIA::NUM sConnLen = sMatcherLen - 1;
				for(GAIA::NUM x = 0; x < sConnLen; ++x)
				{
					GAIA::NUM sConnHash = this->connhash(pMatcher[x], pMatcher[x + 1]);
					const GAIA::CTN::Vector<Node*>& v = m_conn[sConnHash];
					if(v.empty())
						continue;
					GAIA::CTN::Pair<GAIA::NUM, GAIA::NUM> pr(v.size(), sConnHash);
					r.listConn.push_back(pr);
				}
				r.listConn.sort();
				r.listConn.unique();
				
				// Solve andset.
				GAIA::BL bAndset = GAIA::False;
				GAIA::NUM sSwapCursor = 0;
				for(GAIA::NUM x = 0; x < r.listConn.size(); ++x)
				{
					const GAIA::CTN::Vector<Node*>& cur = m_conn[r.listConn[x].back()];
					if(!bAndset)
					{
						r.listNode = cur;
						bAndset = GAIA::True;
						continue;
					}
					if((sSwapCursor++) % 2 == 0)
						this->andset(r.listNode, cur, r.listNodeTemp);
					else
						this->andset(r.listNodeTemp, cur, r.listNode);
				}
				for(GAIA::NUM x = 0; x < r.listSite.size(); ++x)
				{
					const GAIA::CTN::Vector<Node*>& cur = m_site[r.listSite[x].back()];
					if(!bAndset)
					{
						r.listNode = cur;
						bAndset = GAIA::True;
						continue;
					}
					if((sSwapCursor++) % 2 == 0)
						this->andset(r.listNode, cur, r.listNodeTemp);
					else
						this->andset(r.listNodeTemp, cur, r.listNode);
				}
				if(sSwapCursor % 2 == 1)
					r.listNode = r.listNodeTemp;
				r.listNode.sort();
				r.listNode.unique();
				
				// Calculate weight.
				for(GAIA::NUM x = 0; x < r.listNode.size(); ++x)
				{
					Node* pNode = r.listNode[x];
					Weight w;
					w.pNode = pNode;
					w.sWeight = this->weight(m_keybuf.fptr() + pNode->sKeyOffset, pNode->sKeyLen, pMatcher, sMatcherLen);
					if(w.sWeight > 0)
						r.listWeight.push_back(w);
				}
				
				// Generate result.
				r.listWeight.sort();
				for(GAIA::NUM x = 0; x < r.listWeight.size(); ++x)
				{
					const Weight& w = r.listWeight[x];
					if(w.sWeight <= 0)
						break;
					r.listResult.push_back(w.pNode->sIndex);
				}
			}
			const GAIA::GVOID* get(GAIA::NUM sIndex, GAIA::U32** ppKey = GNIL, GAIA::NUM* pKeyLen = GNIL) const
			{
				if(sIndex < 0 || sIndex >= m_nodes.size())
					return GNIL;
				Node* pNode = m_nodes[sIndex];
				if(ppKey != GNIL)
					*ppKey = (GAIA::U32*)(m_keybuf.fptr() + pNode->sKeyOffset);
				if(pKeyLen != GNIL)
					*pKeyLen = pNode->sKeyLen;
				return pNode->pData;
			}
			
		private:
			GAIA::GVOID optimize()
			{
				if(m_bOptimized)
					return;
				for(GAIA::NUM x = 0; x < m_site.size(); ++x)
					m_site[x].sort();
				for(GAIA::NUM x = 0; x < m_conn.size(); ++x)
					m_conn[x].sort();
				m_bOptimized = GAIA::True;
			}
			GAIA::NUM sitehash(GAIA::U32 u) const
			{
				return u % 65536;
			}
			GAIA::NUM connhash(GAIA::U32 uPrev, GAIA::U32 uNext) const
			{
				return (((GAIA::U8)(uPrev % 256)) << 8) | 
				(((GAIA::U8)(uNext % 256)) << 0);
			}
			GAIA::GVOID andset(const GAIA::CTN::Vector<Node*>& s1, const GAIA::CTN::Vector<Node*>& s2, GAIA::CTN::Vector<Node*>& r) const
			{
				r.clear();
				if(s1.empty())
					return;
				if(s1.size() > 1)
				{
					const Node** pEnd = (const Node**)GAIA::ALGO::lower_equal(s2.fptr(), s2.bptr(), s1.back());
					const Node** pBegin = (const Node**)GAIA::ALGO::upper_equal(s2.fptr(), s2.bptr(), s1.front());
					if(pBegin != GNIL && pEnd != GNIL)
					{
						GAST(pBegin <= pEnd);
						for(GAIA::NUM x = 0; x < s1.size(); ++x)
						{
							Node* pNode = s1[x];
							if(GAIA::ALGO::gbinary_search(pBegin, pEnd, pNode) != GNIL)
								r.push_back(pNode);
						}
					}
				}
				else
				{
					Node* pNode = s1[0];
					if(s2.binary_search(pNode) >= 0)
						r.push_back(pNode);
				}
			}
			GAIA::NUM weight(const GAIA::U32* pKey, GAIA::NUM sKeyLen, const GAIA::U32* pMatcher, GAIA::NUM sMatcherLen) const
			{
				GAIA::NUM sRet = 0;
				for(GAIA::NUM y = 0; y < sKeyLen; ++y)
				{
					for(GAIA::NUM x = 0; x < sMatcherLen; ++x)
					{
						if(y + x >= sKeyLen)
							break;
						if(pMatcher[x] == pKey[y + x])
						{
							GAIA::NUM sMove = x > 8 ? 8 : x;
							sRet += (1 << sMove);
						}
						else
							break;
					}
				}
				return sRet;
			}
			
		private:
			GAIA::CTN::Vector<GAIA::U32> m_keybuf;
			GAIA::CTN::Pool<Node> m_nodepool;
			GAIA::CTN::Vector<Node*> m_nodes;
			GAIA::CTN::Vector<GAIA::CTN::Vector<Node*> > m_site;
			GAIA::CTN::Vector<GAIA::CTN::Vector<Node*> > m_conn;
			GAIA::BL m_bOptimized;
		};
	}
}

#endif
