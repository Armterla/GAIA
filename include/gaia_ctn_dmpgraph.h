#ifndef		__GAIA_CTN_DMPGRAPH_H__
#define		__GAIA_CTN_DMPGRAPH_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_set.h"

/* DMPGraph is Directed-Multi-Path-Graph. */
namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _KeyType, typename _SizeType, typename _ExtendType> class BasicDMPGraph : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _KeyType _keytype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		private:
			class Link;
			class Path;
			class LinkedPath;
			class Obj;
			class Node;
		public:
			typedef BasicDMPGraph<_DataType, _KeyType, _SizeType, _ExtendType> __MyType;
			typedef GAIA::CTN::BasicSet<Link, _SizeType, _SizeType, _ExtendType> __LinkSetType;
			typedef GAIA::CTN::BasicSet<Path, _SizeType, _SizeType, _ExtendType> __PathSetType;
			typedef GAIA::CTN::BasicSet<LinkedPath, _SizeType, _SizeType, _ExtendType> __LinkedPathSetType;
			typedef GAIA::CTN::BasicSet<Obj, _SizeType, _SizeType, _ExtendType> __ObjSetType;
			typedef GAIA::CTN::BasicSet<GAIA::CTN::Ref<Node>, _SizeType, _SizeType, _ExtendType> __NodeSetType;
			typedef GAIA::CTN::BasicPool<Node, _SizeType, _ExtendType> __PoolType;
		public:
			class FindCallBack : public GAIA::Base
			{
			public:
				virtual GAIA::BL find(__MyType& g, const _KeyType* pKeys, const _SizeType& keysize, const _DataType& t){return GAIA::False;}
			};
		private:
			class Link : public GAIA::Base
			{
			public:
				GCLASS_COMPARE2(id, step, Link)
			public:
				_SizeType id;
				_SizeType step;
			};
			class Path : public GAIA::Base
			{
			public:
				GCLASS_COMPARE(pNode->key, Path)
			public:
				Node* pNode;
				__LinkSetType links;
			};
			class LinkedPath : public GAIA::Base
			{
			public:
				GCLASS_COMPARE(link, LinkedPath)
			public:
				Link link;
				Node* pNode;
			};
			class Obj : public GAIA::Base
			{
			public:
				GCLASS_COMPARE(link, Obj)
			public:
				Link link;
				_DataType t;
				Node* pFrontNode;
			};
			class Node : public GAIA::Base
			{
			public:
				GCLASS_COMPARE(key, Node)
			public:
				_KeyType key;
				__ObjSetType objs;
				__PathSetType nextpaths;
				__LinkedPathSetType nextlinkedpaths;
				_SizeType prevcount;
			};
			class MatchFindCallBack : public FindCallBack
			{
				friend class BasicDMPGraph;
			public:
				GINL MatchFindCallBack(){this->init();}
				virtual GAIA::BL find(__MyType& g, const _KeyType* pKeys, const _SizeType& keysize, const _DataType& t)
				{
					GAST(pResult == GNIL);
					pResult = &t;
					return GAIA::False;
				}
			private:
				GINL GAIA::GVOID init(){pResult = GNIL;}
			private:
				const _DataType* pResult;
			};
			class EraseCallBack : public GAIA::Base
			{
				friend class BasicDMPGraph;
			public:
				GINL GAIA::GVOID reset(){records.clear();}
				virtual GAIA::BL erase(const _DataType& t, const Node& n, const Link& l)
				{
					Record r;
					r.pNode = &n;
					r.type = t;
					r.link = l;
					records.push_back(r);
					return GAIA::True;
				}
			private:
				class Record : public GAIA::Base
				{
				public:
					_DataType type;
					Link link;
					const Node* pNode;
				};
			private:
				typedef GAIA::CTN::BasicVector<Record, _SizeType, _ExtendType> __RecordListType;
			private:
				__RecordListType records;
			};
		public:
			GINL BasicDMPGraph(){this->init();}
			GINL GAIA::GVOID clear(){m_pool.clear(); m_nodes.clear(); m_curlinkid = 0; m_size = 0;}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_size;}
			GINL GAIA::GVOID destroy(){m_pool.destroy(); m_nodes.destroy(); m_curlinkid = 0; m_size = 0;}
			GINL GAIA::GVOID insert(const _KeyType* pKeys, const _SizeType& keysize, const _DataType& t)
			{
				GAST(pKeys != GNIL);
				GAST(keysize > 0);
				
				// Create last context.
				Link lastlink;
				if(keysize == 1)
				{
					lastlink.id = GINVALID;
					lastlink.step = GINVALID;
				}
				else
				{
					lastlink.id = m_curlinkid++;
					lastlink.step = 0;
				}

				// Open every node and link it.
				Node* pFrontNode = GNIL;
				Node* pBackNode = GNIL;
				for(_SizeType x = 0; x < keysize; ++x)
				{
					Node& n = this->reqnode(pKeys[x]);
					if(pBackNode != GNIL)
						this->linknode(*pBackNode, n, lastlink.id, lastlink.step++, keysize);
					else
						pFrontNode = &n;
					pBackNode = &n;
				}

				// Insert object.
				Obj o;
				o.link = lastlink;
				o.t = t;
				o.pFrontNode = pFrontNode;
				pBackNode->objs.insert(o);

				//
				++m_size;
			}
			GINL GAIA::BL erase(const _KeyType* pKeys, const _SizeType& keysize, const _DataType& t)
			{
				GAST(pKeys != GNIL);
				GAST(keysize > 0);

				const Node* pFirstNode = this->opennode(pKeys[0]);
				if(pFirstNode == GNIL)
					return 0;

				m_erasecb.reset();
				MatchFindCallBack cb;
				this->find_recursive(pKeys, keysize, GAIA::True, GAIA::True, *pFirstNode, GINVALID, GINVALID, 0, cb, &m_erasecb);
				for(typename EraseCallBack::__RecordListType::it it = m_erasecb.records.frontit(); !it.empty(); ++it)
				{
					typename EraseCallBack::Record& r = *it;
					if(r.type == t)
					{
						Link l;
						l.id = r.link.id;
						l.step = 0;
						this->erase_recursive(GCCAST(Node&)(*r.pNode), l, r.link.step + 1);
						--m_size;
						return GAIA::True;
					}
				}

				return GAIA::False;
			}
			GINL _SizeType erase(const _KeyType* pKeys, const _SizeType& keysize)
			{
				GAST(pKeys != GNIL);
				GAST(keysize > 0);
				return this->erase(pKeys, keysize, GAIA::True, GAIA::True);
			}
			GINL _SizeType erase(const _KeyType* pKeys, const _SizeType& keysize, GAIA::BL bMatchLeft, GAIA::BL bMatchRight)
			{
				GAST(pKeys != GNIL);
				GAST(keysize > 0);

				const Node* pFirstNode = this->opennode(pKeys[0]);
				if(pFirstNode == GNIL)
					return 0;

				m_erasecb.reset();
				MatchFindCallBack cb;
				this->find_recursive(pKeys, keysize, bMatchLeft, bMatchRight, *pFirstNode, GINVALID, GINVALID, 0, cb, &m_erasecb);
				for(typename EraseCallBack::__RecordListType::it it = m_erasecb.records.frontit(); !it.empty(); ++it)
				{
					typename EraseCallBack::Record& r = *it;
					Link l;
					l.id = r.link.id;
					l.step = 0;
					this->erase_recursive(GCCAST(Node&)(*r.pNode), l, r.link.step + 1);
				}
				m_size -= m_erasecb.records.size();

				return m_erasecb.records.size();
			}
			GINL const _DataType* find(const _KeyType* pKeys, const _SizeType& keysize) const
			{
				GAST(pKeys != GNIL);
				GAST(keysize > 0);

				MatchFindCallBack cb;
				this->find(pKeys, keysize, GAIA::True, GAIA::True, cb);
				return cb.pResult;
			}
			GINL _SizeType find(const _KeyType* pKeys, const _SizeType& keysize, GAIA::BL bMatchLeft, GAIA::BL bMatchRight, FindCallBack& cb) const
			{
				GAST(pKeys != GNIL);
				GAST(keysize > 0);

				const Node* pFirstNode = this->opennode(pKeys[0]);
				if(pFirstNode == GNIL)
					return 0;

				return this->find_recursive(pKeys, keysize, bMatchLeft, bMatchRight, *pFirstNode, GINVALID, GINVALID, 0, cb, GNIL);
			}
		private:
			GINL GAIA::GVOID init(){m_curlinkid = 0; m_size = 0;}
			GINL const Node* opennode(const _KeyType& key) const
			{
				Node n;
				n.key = key;
				GAIA::CTN::Ref<Node> finder = &n;
				const GAIA::CTN::Ref<Node>* pFindedNodeRef = m_nodes.find(finder);
				if(pFindedNodeRef == GNIL)
					return GNIL;
				return &**pFindedNodeRef;
			}
			GINL const Path* openpath(const __PathSetType& paths, const Node& n) const
			{
				Path p;
				p.pNode = GCCAST(Node*)(&n);
				return paths.find(p);
			}
			GINL const Path* openpath(const __PathSetType& paths, const _KeyType& key) const
			{
				Node n;
				n.key = key;
				return this->openpath(paths, n);
			}
			GINL Node& reqnode(const _KeyType& key)
			{
				Node* pNode = GCCAST(Node*)(this->opennode(key));
				if(pNode == GNIL)
				{
					pNode = m_pool.alloc();
					pNode->key = key;
					pNode->prevcount = 0;

					GAIA::CTN::Ref<Node> temp = pNode;
					m_nodes.insert(temp);
					pNode = &**m_nodes.find(temp);
					GAST(pNode != GNIL);
				}
				return *pNode;
			}
			GINL Path& reqpath(__PathSetType& paths, Node& n)
			{
				Path* pPath = GCCAST(Path*)(this->openpath(paths, n));
				if(pPath == GNIL)
				{
					Path temp;
					temp.pNode = &n;
					paths.insert(temp);
					pPath = paths.find(temp);
				}
				return *pPath;
			}
			GINL GAIA::GVOID linknode(Node& nprev, Node& nnext, const _SizeType& id, const _SizeType& step, const _SizeType& size)
			{
				// Insert prev node's nextlinkedpath.
				{
					LinkedPath lp;
					lp.link.id = id;
					lp.link.step = step;
					lp.pNode = &nnext;
					nprev.nextlinkedpaths.insert(lp);
				}

				// Insert prev node's nextpath.
				{
					Path& p = this->reqpath(nprev.nextpaths, nnext);
					Link l;
					l.id = id;
					l.step = step;
					p.links.insert(l);
					++nnext.prevcount;
				}
			}
			GINL _SizeType try_callback_link(const _KeyType* pKeys, const _SizeType& keysize, const Node& n, const _SizeType& id, const _SizeType& step, FindCallBack& cb, EraseCallBack* pEraseCallBack) const
			{
				Obj ofinder;
				ofinder.link.id = id;
				ofinder.link.step = step;
				const Obj* pFindedObj = n.objs.find(ofinder);
				if(pFindedObj != GNIL)
				{
					if(pEraseCallBack != GNIL)
						pEraseCallBack->erase(pFindedObj->t, *pFindedObj->pFrontNode, ofinder.link);
					else
						cb.find(*GCCAST(__MyType*)(this), pKeys, keysize, pFindedObj->t);
					return 1;
				}
				return 0;
			}
			GINL _SizeType find_recursive(const _KeyType* pKeys, const _SizeType& keysize, GAIA::BL bMatchLeft, GAIA::BL bMatchRight, const Node& n, const _SizeType& id, const _SizeType& step, const _SizeType& cursor, FindCallBack& findcb, EraseCallBack* pEraseCallBack) const
			{
				GAST(pKeys != GNIL);
				GAST(keysize > 0);

				//
				_SizeType ret = 0;

				// 
				if(cursor >= keysize - 1)
					ret += this->try_callback_link(pKeys, keysize, n, id, step, findcb, pEraseCallBack);

				//
				if(bMatchRight && cursor == keysize - 1)
					return ret;

				// Recursive.
				if(id == GINVALID)
				{
					if(keysize > 1)
					{
						const Path* pPath = this->openpath(n.nextpaths, pKeys[1]);
						if(pPath == GNIL)
							return ret;
						for(typename __LinkSetType::const_it it = pPath->links.const_frontit(); !it.empty(); ++it)
						{
							const Link& l = *it;
							if(bMatchLeft && l.step != 0)
								continue;
							ret += this->find_recursive(pKeys, keysize, bMatchLeft, bMatchRight, n, l.id, l.step, 0, findcb, pEraseCallBack);
						}
					}
					else
					{
						for(typename __PathSetType::const_it pathit = n.nextpaths.const_frontit(); !pathit.empty(); ++pathit)
						{
							const Path& path = *pathit;
							for(typename __LinkSetType::const_it linkit = path.links.const_frontit(); !linkit.empty(); ++linkit)
							{
								const Link& l = *linkit;
								if(bMatchLeft && l.step != 0)
									continue;
								ret += this->find_recursive(pKeys, keysize, bMatchLeft, bMatchRight, n, l.id, l.step, 0, findcb, pEraseCallBack);
							}
						}
					}
				}
				else
				{
					LinkedPath lpfinder;
					lpfinder.link.id = id;
					lpfinder.link.step = step;
					const LinkedPath* pFindedLinkedPath = n.nextlinkedpaths.find(lpfinder);
					if(pFindedLinkedPath == GNIL)
						return ret;
					if(cursor < keysize - 1 && pFindedLinkedPath->pNode->key != pKeys[cursor + 1])
						return ret;
					ret += this->find_recursive(pKeys, keysize, bMatchLeft, bMatchRight, *pFindedLinkedPath->pNode, id, step + 1, cursor + 1, findcb, pEraseCallBack);
				}

				return ret;
			}
			GINL GAIA::GVOID erase_recursive(Node& n, const Link& l, const _SizeType& size)
			{
				if((l.id == GINVALID && l.step == GINVALID) || l.step == size - 1)
				{
					Obj o;
					o.link = l;
					n.objs.erase(o);
					return;
				}

				LinkedPath lpfinder;
				lpfinder.link = l;
				LinkedPath* pLPFinded = n.nextlinkedpaths.find(lpfinder);
				if(pLPFinded != GNIL)
				{
					LinkedPath linkedpath = *pLPFinded;

					// Unlink current to next.(Path)
					{
						Link templink;
						templink = l;
						++templink.step;
						this->erase_recursive(*linkedpath.pNode, templink, size);
						n.nextlinkedpaths.erase(lpfinder);
					}

					// Unlink current to next.(Node)
					{
						Path pfinder;
						pfinder.pNode = linkedpath.pNode;
						Path* pPathFinded = n.nextpaths.find(pfinder);
						pPathFinded->links.erase(l);

						if(pPathFinded->links.empty())
							n.nextpaths.erase(pfinder);

						if(n.nextpaths.empty() && n.prevcount == 0 && n.objs.empty())
						{
							GAIA::CTN::Ref<Node> noderef = &n;
							m_nodes.erase(noderef);
							m_pool.release(&n);
						}
					}

					// Unlink next to current.(Node)
					{
						--linkedpath.pNode->prevcount;
						if(linkedpath.pNode->nextpaths.empty() && linkedpath.pNode->prevcount == 0 && linkedpath.pNode->objs.empty())
						{
							GAIA::CTN::Ref<Node> noderef = linkedpath.pNode;
							m_nodes.erase(noderef);
							m_pool.release(linkedpath.pNode);
						}
					}
				}
			}
		private:
			__PoolType m_pool;
			__NodeSetType m_nodes;
			_SizeType m_curlinkid;
			_SizeType m_size;
			EraseCallBack m_erasecb;
		};
	}
}

#endif
