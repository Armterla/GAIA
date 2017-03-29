#ifndef		__GAIA_ALLOCATOR_LEAKDETECTOR_H__
#define		__GAIA_ALLOCATOR_LEAKDETECTOR_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_internal.h"

#include <stdlib.h>

#include <set>

namespace GAIA
{
	namespace MSYS
	{
		class LeakDetector : public GAIA::Base
		{
		private:
			template<typename _DataType> class STLAllocator : public std::allocator<_DataType>
			{
			public:
				typedef size_t size_type;
				typedef typename std::allocator<_DataType>::pointer pointer;
				typedef typename std::allocator<_DataType>::value_type value_type;
				typedef typename std::allocator<_DataType>::const_pointer const_pointer;
				typedef typename std::allocator<_DataType>::reference reference;
				typedef typename std::allocator<_DataType>::const_reference const_reference;
				template<class _Other> struct rebind{typedef STLAllocator<_Other> other;};
			public:
			#if GAIA_COMPILER == GAIA_COMPILER_VC
				STLAllocator() throw(){}
			#else
				STLAllocator(){}
			#endif
				STLAllocator(const STLAllocator& a) throw() : std::allocator<_DataType>(a){}
				template<typename _ParamDataType> STLAllocator(const STLAllocator<_ParamDataType>&) throw(){}
				pointer allocate(size_type count, const void* pHint = 0){return (pointer)malloc(count * sizeof(value_type));}
				void deallocate(pointer p, size_type count){free(p);}
			};
		public:
			class Status
			{
			public:
				GINL GAIA::GVOID reset()
				{
					nAllocSizeSum = 0;
					nAllocTimesSum = 0;
					nReleaseSizeSum = 0;
					nReleaseTimesSum = 0;
					nMinAllocSize = +(GAIA::N64)0x4000000000000000;
					nMaxAllocSize = -(GAIA::N64)0x4000000000000000;
				}
				GINL GAIA::BL isreset() const
				{
					if(nAllocSizeSum != 0)
						return GAIA::False;
					if(nAllocTimesSum != 0)
						return GAIA::False;
					if(nReleaseSizeSum != 0)
						return GAIA::False;
					if(nReleaseTimesSum != 0)
						return GAIA::False;
					if(nMinAllocSize != +(GAIA::N64)0x4000000000000000)
						return GAIA::False;
					if(nMaxAllocSize != -(GAIA::N64)0x4000000000000000)
						return GAIA::False;
					return GAIA::True;
				}
				GINL GAIA::GVOID diff(const GAIA::MSYS::LeakDetector::Status& older)
				{
					nAllocSizeSum -= older.nAllocSizeSum;
					nAllocTimesSum -= older.nAllocTimesSum;
					nReleaseSizeSum -= older.nReleaseSizeSum;
					nReleaseTimesSum -= older.nReleaseTimesSum;
					nMinAllocSize = 0;
					nMaxAllocSize = 0;
				}
			public:
				GAIA::N64 nAllocSizeSum;
				GAIA::N64 nAllocTimesSum;
				GAIA::N64 nReleaseSizeSum;
				GAIA::N64 nReleaseTimesSum;
				GAIA::N64 nMinAllocSize;
				GAIA::N64 nMaxAllocSize;
			};
			class NodeBase
			{
			public:
				GINL GAIA::GVOID reset()
				{
					pszFileName = GNIL;
					nCodeLine = -1;
				}
			public:
				GAIA::CH* pszFileName;
				GAIA::N32 nCodeLine;
			};
			class Node : public NodeBase
			{
			public:
				GINL GAIA::GVOID reset(){NodeBase::reset(); status.reset();}
				GINL GAIA::N32 compare(const Node& src) const
				{
					if(nCodeLine < src.nCodeLine)
						return -1;
					else if(nCodeLine > src.nCodeLine)
						return +1;
					return GAIA_INTERNAL_NAMESPACE::gstrcmp(pszFileName, src.pszFileName);
				}
				GCLASS_COMPARE_BYCOMPARE(Node)
			public:
				Status status;
			};
			class CallBack
			{
			public:
				virtual GAIA::BL OnDumpStatus(const GAIA::MSYS::LeakDetector::Status& s){return GAIA::True;}
				virtual GAIA::BL OnDumpNode(const GAIA::MSYS::LeakDetector::Node& n){return GAIA::True;}
			};
			class CallBackDefault : public CallBack
			{
			public:
				GINL CallBackDefault(){m_bGlobalStatus = GAIA::True;}
				GINL GAIA::BL OnDumpStatus(const GAIA::MSYS::LeakDetector::Status& s)
				{
					if(m_bGlobalStatus)
						m_stm << "Global Memory Status:\n";
					m_stm << "\t" << "Leak count and size = " << s.nAllocTimesSum - s.nReleaseTimesSum << "/" << s.nAllocSizeSum - s.nReleaseSizeSum << "\n";
					m_stm << "\t" << "Alloc count and size = " << s.nAllocTimesSum << "/" << s.nAllocSizeSum << "\n";
					m_stm << "\t" << "Release count and size = " << s.nReleaseTimesSum << "/" << s.nReleaseSizeSum << "\n";
					if(s.nMinAllocSize <= s.nMaxAllocSize)
						m_stm << "\t" << "Min = " << s.nMinAllocSize << " Max = " << s.nMaxAllocSize << "\n";
					else
						m_stm << "\t" << "No min and max info" << "\n";
					m_stm << "\n";
					return GAIA::True;
				}
				virtual GAIA::BL OnDumpNode(const GAIA::MSYS::LeakDetector::Node& n)
				{
					if(n.status.nAllocTimesSum != n.status.nReleaseTimesSum)
					{
						m_stm << n.pszFileName << "(" << n.nCodeLine << ")\n";
						m_bGlobalStatus = GAIA::False;
						if(!this->OnDumpStatus(n.status))
						{
							m_bGlobalStatus = GAIA::True;
							return GAIA::False;
						}
						m_bGlobalStatus = GAIA::True;
					}
					return GAIA::True;
				}
			private:
				GAIA::STREAM::STDStream m_stm;
				GAIA::BL m_bGlobalStatus;
			};
			class AllocNode : public NodeBase
			{
			public:
				GCLASS_COMPARE(p, AllocNode)
			public:
				const GAIA::GVOID* p;
				GAIA::UM size;
			};
			typedef std::set<Node, std::less<Node>, STLAllocator<Node> > __NodeList;
			typedef std::set<AllocNode, std::less<AllocNode>, STLAllocator<AllocNode> > __AllocNodeList;
			class Snapshot
			{
			public:
				GINL GAIA::GVOID diff(const GAIA::MSYS::LeakDetector::Snapshot& older)
				{
					for(__NodeList::iterator it = nodes.begin(); it != nodes.end(); )
					{
						Node& finder = (Node&)*it;
						__NodeList::const_iterator itfinded = older.nodes.find(finder);
						if(itfinded != older.nodes.end())
						{
							const Node& finded = *itfinded;
							finder.status.diff(finded.status);
							if(finder.status.isreset())
							{
								it = nodes.erase(it);
								continue;
							}
						}
						++it;
					}
					for(__AllocNodeList::iterator it = allocnodes.begin(); it != allocnodes.end(); )
					{
						AllocNode& finder = (AllocNode&)*it;
						__AllocNodeList::const_iterator itfinded = older.allocnodes.find(finder);
						if(itfinded != older.allocnodes.end())
						{
							it = allocnodes.erase(it);
							continue;
						}
						++it;
					}
					gstatus.diff(older.gstatus);
				}
			public:
				__NodeList nodes;
				__AllocNodeList allocnodes;
				Status gstatus; // Global status.
			};
		public:
			GINL LeakDetector(){this->reset();}
			GINL ~LeakDetector(){this->reset();}
			GINL GAIA::GVOID reset()
			{
			#ifdef GAIA_HEAP_THREADSAFE
				GAIA::SYNC::Autolock al(m_lr);
			#endif
				// Release all node.
				for(__NodeList::iterator it = m_snapshot.nodes.begin(); it != m_snapshot.nodes.end(); ++it)
				{
					const Node& n = *it;
					free(n.pszFileName);
				}
				m_snapshot.nodes.clear();

				// Release all alloc node.
				m_snapshot.allocnodes.clear();

				// Reset global status.
				m_snapshot.gstatus.reset();
			}
			GINL GAIA::GVOID alloc(const GAIA::GVOID* p, const GAIA::CH* pszFileName, GAIA::N32 nCodeLine, const GAIA::UM& uSize)
			{
			#ifdef GAIA_HEAP_THREADSAFE
				GAIA::SYNC::Autolock al(m_lr);
			#endif

				// Find node, if not exist, create new.
				Node finder;
				finder.pszFileName = (GAIA::CH*)pszFileName;
				finder.nCodeLine = nCodeLine;
				__NodeList::iterator it = m_snapshot.nodes.find(finder);
				if(it == m_snapshot.nodes.end())
				{
					finder.reset();
					finder.pszFileName = (GAIA::CH*)malloc(GAIA_INTERNAL_NAMESPACE::gstrlen(pszFileName) + sizeof(GAIA::CH));
					GAIA_INTERNAL_NAMESPACE::gstrcpy(finder.pszFileName, pszFileName);
					finder.nCodeLine = nCodeLine;
					std::pair<__NodeList::iterator, bool> pr = m_snapshot.nodes.insert(finder);
					it = pr.first;
				}

				// Insert alloc node.
				Node& n = GCCAST(Node&)(*it);
				AllocNode an;
				an.p = p;
				an.size = uSize;
				an.pszFileName = n.pszFileName;
				an.nCodeLine = n.nCodeLine;
				m_snapshot.allocnodes.insert(an);

				// Update node status.
				n.status.nAllocSizeSum += uSize;
				n.status.nAllocTimesSum++;
				if((GAIA::N64)uSize < n.status.nMinAllocSize)
					n.status.nMinAllocSize = uSize;
				if((GAIA::N64)uSize > n.status.nMaxAllocSize)
					n.status.nMaxAllocSize = uSize;

				// Update global status.
				m_snapshot.gstatus.nAllocSizeSum += uSize;
				m_snapshot.gstatus.nAllocTimesSum++;
				if((GAIA::N64)uSize < m_snapshot.gstatus.nMinAllocSize)
					m_snapshot.gstatus.nMinAllocSize = uSize;
				if((GAIA::N64)uSize > m_snapshot.gstatus.nMaxAllocSize)
					m_snapshot.gstatus.nMaxAllocSize = uSize;
			}
			GINL GAIA::GVOID release(const GAIA::GVOID* p)
			{
			#ifdef GAIA_HEAP_THREADSAFE
				GAIA::SYNC::Autolock al(m_lr);
			#endif

				// Find alloc node.
				AllocNode an;
				an.p = p;
				__AllocNodeList::iterator it = m_snapshot.allocnodes.find(an);
				if(it == m_snapshot.allocnodes.end())
					return;
				AllocNode& anref = GCCAST(AllocNode&)(*it);

				// Update node status.
				Node finder;
				finder.pszFileName = anref.pszFileName;
				finder.nCodeLine = anref.nCodeLine;
				__NodeList::iterator itnode = (__NodeList::iterator)m_snapshot.nodes.find(finder);
				Node& n = (Node&)(*itnode);
				n.status.nReleaseSizeSum += anref.size;
				n.status.nReleaseTimesSum++;
				if((GAIA::N64)anref.size < n.status.nMinAllocSize)
					n.status.nMinAllocSize = anref.size;
				if((GAIA::N64)anref.size > n.status.nMaxAllocSize)
					n.status.nMaxAllocSize = anref.size;

				// Update global status.
				m_snapshot.gstatus.nReleaseSizeSum += anref.size;
				m_snapshot.gstatus.nReleaseTimesSum++;
				if((GAIA::N64)anref.size < m_snapshot.gstatus.nMinAllocSize)
					m_snapshot.gstatus.nMinAllocSize = anref.size;
				if((GAIA::N64)anref.size > m_snapshot.gstatus.nMaxAllocSize)
					m_snapshot.gstatus.nMaxAllocSize = anref.size;

				//
				m_snapshot.allocnodes.erase(it);
			}
			GINL GAIA::GVOID snapshot(GAIA::MSYS::LeakDetector::Snapshot& s)
			{
			#ifdef GAIA_HEAP_THREADSAFE
				GAIA::SYNC::Autolock al(m_lr);
			#endif
				s.nodes = m_snapshot.nodes;
				s.allocnodes = m_snapshot.allocnodes;
				s.gstatus = m_snapshot.gstatus;
			}
			GINL GAIA::GVOID dump(GAIA::MSYS::LeakDetector::Snapshot& s, GAIA::MSYS::LeakDetector::CallBack& cb)
			{
				cb.OnDumpStatus(s.gstatus);
				for(__NodeList::iterator it = s.nodes.begin(); it != s.nodes.end(); ++it)
				{
					const Node& n = *it;
					if(!cb.OnDumpNode(n))
						break;
				}
			}
			GINL GAIA::GVOID dump(GAIA::MSYS::LeakDetector::Snapshot& s)
			{
				CallBackDefault cb;
				this->dump(s, cb);
			}
			GINL GAIA::GVOID dump(GAIA::MSYS::LeakDetector::CallBack& cb)
			{
			#ifdef GAIA_HEAP_THREADSAFE
				GAIA::SYNC::Autolock al(m_lr);
			#endif
				this->dump(m_snapshot, cb);
			}
			GINL GAIA::GVOID dump()
			{
			#ifdef GAIA_HEAP_THREADSAFE
				GAIA::SYNC::Autolock al(m_lr);
			#endif
				this->dump(m_snapshot);
			}
			GINL const Status& globalstatus() const{return m_snapshot.gstatus;}
		private:
			Snapshot m_snapshot;
		#ifdef GAIA_HEAP_THREADSAFE
			GAIA::SYNC::Lock m_lr;
		#endif
		};
	}
}

#endif
