#ifndef		__GAIA_MSYS_HEAPESG_H__
#define		__GAIA_MSYS_HEAPESG_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_msys_base.h"
#include "gaia_sync_atomic.h"
#include "gaia_sync_lockpure.h"
#include <stdlib.h>

namespace GAIA
{
	namespace MSYS
	{
		class HeapESG : public GAIA::MSYS::Heap
		{
		private:
		#if defined(GAIA_HEAP_LIMIT4G) || GAIA_MACHINE == GAIA_MACHINE32
			typedef GAIA::U32 ALLOC_SIZE_TYPE;
		#else
			typedef GAIA::U64 ALLOC_SIZE_TYPE;
		#endif
		public:
			GINL HeapESG(){this->init(); this->InitHeap();}
			GINL ~HeapESG(){this->ReleaseHeap();}
			GINL virtual GAIA::GVOID* memory_alloc(const GAIA::UM& uSize)
			{
			#if defined(GAIA_HEAP_LIMIT4G) && GAIA_MACHINE == GAIA_MACHINE64
				GAST(uSize <= GAIA::U32MAX);
			#endif
				m_piecesize.Increase();
				m_alloctimes.Increase();

				GAIA::UM uSectionIndex = this->GetSectionIndex(uSize + HEAP_BUFFER_HEADERSIZE);
				if(uSectionIndex == (GAIA::UM)GINVALID)
				{
					m_capacity.Add(uSize + HEAP_BUFFER_HEADERSIZE);
					m_size.Add(uSize + HEAP_BUFFER_HEADERSIZE);
					m_usesize.Add(uSize);

					GAIA::GVOID* pTemp = (GAIA::U8*)malloc(uSize + HEAP_BUFFER_HEADERSIZE);
					*GSCAST(ALLOC_SIZE_TYPE*)(pTemp) = GSCAST(ALLOC_SIZE_TYPE)(uSize);
					*GRCAST(GAIA::U16*)(GSCAST(GAIA::U8*)(pTemp) + sizeof(ALLOC_SIZE_TYPE)) = (GAIA::U16)GINVALID;
					return GSCAST(GAIA::U8*)(pTemp) + HEAP_BUFFER_HEADERSIZE;
				}
				else
				{
					m_size.Add(m_secsizelist[uSectionIndex]);
					m_usesize.Add(uSize);
				}
				GAIA::GVOID* pRet;
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Enter();
			#endif
				{
					Section& hs = m_seclist[uSectionIndex];
					if(hs.uMinFreeIndex == (GAIA::UM)GINVALID)
					{
						hs.uMinFreeSize = (GAIA::UM)GINVALID;
						for(GAIA::UM x = 0; x < hs.uObListSize; ++x)
						{
							OriginBuffer& ob = hs.oblist[x];
							if(ob.buf != GNIL && ob.uFreeStackSize != 0 && ob.uFreeStackSize < hs.uMinFreeSize)
							{
								hs.uMinFreeSize = (GAIA::UM)ob.uFreeStackSize;
								hs.uMinFreeIndex = x;
							}
						}
					}
					if(hs.uMinFreeIndex == (GAIA::UM)GINVALID)
					{
						GAIA::U16 uOriginBufferIndex;
						if(hs.uFreeStackSize == 0)
						{
							OriginBuffer newob;
							newob.buf = GNIL;
							newob.freestack = GNIL;
							newob.uFreeStackSize = 0;
							newob.uFreeStackCapacity = 0;
							uOriginBufferIndex = (GAIA::U16)hs.uObListSize;
							this->push(newob, hs.oblist, hs.uObListSize, hs.uObListCapacity);
						}
						else
						{
							uOriginBufferIndex = (GAIA::U16)hs.freestack[hs.uFreeStackSize - 1];
							--hs.uFreeStackSize;
						}
						OriginBuffer& newobref = hs.oblist[uOriginBufferIndex];
						GAIA::UM uSectionPatchCount = this->GetSectionPatchCount(uSectionIndex);
						GAIA::UM uSectionPatchSize = m_secsizelist[uSectionIndex];
						newobref.buf = (GAIA::U8*)malloc(uSectionPatchSize * uSectionPatchCount);
						m_capacity.Add(uSectionPatchSize * uSectionPatchCount);
						for(GAIA::UM x = 0; x < uSectionPatchCount; ++x)
						{
							GAIA::U8* pTemp = (GAIA::U8*)newobref.buf + uSectionPatchSize * x;
							*GRCAST(ALLOC_SIZE_TYPE*)(pTemp) = GSCAST(ALLOC_SIZE_TYPE)(uSectionPatchSize);
							*GRCAST(GAIA::U16*)(pTemp + sizeof(ALLOC_SIZE_TYPE)) = uOriginBufferIndex;
							this->push(pTemp, newobref.freestack, newobref.uFreeStackSize, newobref.uFreeStackCapacity);
						}
						pRet = newobref.freestack[newobref.uFreeStackSize - 1];
						--newobref.uFreeStackSize;
						hs.uMinFreeIndex = uOriginBufferIndex;
						hs.uMinFreeSize = uSectionPatchCount - 1;
					}
					else
					{
						OriginBuffer& ob = hs.oblist[hs.uMinFreeIndex];
						pRet = ob.freestack[ob.uFreeStackSize - 1];
						--ob.uFreeStackSize;
						--hs.uMinFreeSize;
						if(hs.uMinFreeSize == 0)
						{
							hs.uMinFreeIndex = (GAIA::UM)GINVALID;
							hs.uMinFreeSize = (GAIA::UM)GINVALID;
						}
					}
				}
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Leave();
			#endif
				*GSCAST(ALLOC_SIZE_TYPE*)(pRet) = *GRCAST(ALLOC_SIZE_TYPE*)(&uSize);
				pRet = GSCAST(GAIA::U8*)(pRet) + HEAP_BUFFER_HEADERSIZE;
				GAST(((GAIA::U64)pRet) % sizeof(GAIA::GVOID*) == 0); // Must aligned by machine word length.
				return pRet;
			}
			GINL virtual GAIA::GVOID memory_release(GAIA::GVOID* p)
			{
				GAST(((GAIA::U64)p) % sizeof(GAIA::GVOID*) == 0); // Must aligned by machine word length.
				GAIA::U8* pOriginP = GSCAST(GAIA::U8*)(p) - HEAP_BUFFER_HEADERSIZE;
				GAIA::U16 uOBIndex = *GRCAST(GAIA::U16*)(pOriginP + sizeof(ALLOC_SIZE_TYPE));
				if(uOBIndex == (GAIA::U16)GINVALID)
				{
					m_capacity.Add(-(GAIA::N64)this->memory_size(p) - (GAIA::N64)HEAP_BUFFER_HEADERSIZE);
					m_size.Add(-(GAIA::N64)this->memory_size(p) - (GAIA::N64)HEAP_BUFFER_HEADERSIZE);
					m_usesize.Add(-(GAIA::N64)this->memory_size(p));
					m_piecesize.Decrease();
					free(pOriginP);
					return;
				}
				GAIA::UM uSectionIndex = this->GetSectionIndex(*GRCAST(ALLOC_SIZE_TYPE*)(pOriginP) + HEAP_BUFFER_HEADERSIZE);
				GAST(uSectionIndex != (GAIA::UM)GINVALID);
				m_size.Add(-(GAIA::N64)m_secsizelist[uSectionIndex]);
				m_usesize.Add(-(GAIA::N64)this->memory_size(p));
				m_piecesize.Decrease();
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Enter();
			#endif
				{
					Section& hs = m_seclist[uSectionIndex];
					OriginBuffer& ob = hs.oblist[uOBIndex];
					this->push(pOriginP, ob.freestack, ob.uFreeStackSize, ob.uFreeStackCapacity);
					if(ob.uFreeStackSize == this->GetSectionPatchCount(uSectionIndex))
					{
						m_capacity.Add(-((GAIA::N64)m_secsizelist[uSectionIndex] * (GAIA::N64)ob.uFreeStackSize));
						if(uOBIndex == hs.uMinFreeIndex)
						{
							hs.uMinFreeIndex = (GAIA::UM)GINVALID;
							hs.uMinFreeSize = (GAIA::UM)GINVALID;
						}
						free(ob.freestack);
						ob.freestack = GNIL;
						ob.uFreeStackSize = 0;
						ob.uFreeStackCapacity = 0;
						free(ob.buf);
						ob.buf = GNIL;
						this->push(uOBIndex, hs.freestack, hs.uFreeStackSize, hs.uFreeStackCapacity);
					}
				}
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Leave();
			#endif
			}
			GINL virtual GAIA::UM memory_size(GAIA::GVOID* p)
			{
				return *GRCAST(ALLOC_SIZE_TYPE*)(GSCAST(GAIA::U8*)(p) - HEAP_BUFFER_HEADERSIZE);
			}
			GINL virtual GAIA::UM capacity(){return (GAIA::UM)m_capacity;}
			GINL virtual GAIA::UM size(){return (GAIA::UM)m_size;}
			GINL virtual GAIA::UM use_size(){return (GAIA::UM)m_usesize;}
			GINL virtual GAIA::UM piece_size(){return (GAIA::UM)m_piecesize;}
			GINL virtual GAIA::U64 alloc_times(){return m_alloctimes;}
		private:
			class OriginBuffer
			{
			public:
				GAIA::U8* buf;
				GAIA::U8** freestack;
				GAIA::UM uFreeStackSize;
				GAIA::UM uFreeStackCapacity;
			};
			class Section
			{
			public:
				OriginBuffer* oblist;
				GAIA::UM uObListSize;
				GAIA::UM uObListCapacity;
				GAIA::U16* freestack;
				GAIA::UM uFreeStackSize;
				GAIA::UM uFreeStackCapacity;
				GAIA::UM uMinFreeIndex;
				GAIA::UM uMinFreeSize;
			};
		private:
			GINL GAIA::GVOID init()
			{
				m_seclist = GNIL;
				m_uSecListSize = 0;
				m_secsizelist = GNIL;
				m_uSecSizeListSize = 0;
			}
			GINL GAIA::UM GetSectionPatchSize(GAIA::UM uIndex) const{return 32 + 32 * uIndex * uIndex;}
			GINL GAIA::UM GetSectionPatchCount(GAIA::UM uIndex) const{if(uIndex == 0) uIndex = 1; return 40000 / (uIndex * uIndex);}
			GINL GAIA::UM GetSectionIndex(GAIA::UM uSize) const
			{
				GAIA::UM x;
				for(x = 0; x < HEAP_SECTION_COUNT; x += 10)
				{
					GAIA::UM uTemp = m_secsizelist[x];
					if(uTemp < uSize)
						continue;
					if(uTemp == uSize)
						return x;
					if(x > 0)
					{
						for(GAIA::UM y = 0; y < 10; y++)
						{
							if(m_secsizelist[x + y - 10] >= uSize)
								return x + y - 10;
						}
					}
					return x;
				}
				x -= 10;
				for(; x < HEAP_SECTION_COUNT; ++x)
				{
					if(m_secsizelist[x] >= uSize)
						return x;
				}
				return (GAIA::UM)GINVALID;
			}
			GINL GAIA::GVOID InitHeap()
			{
				GAST(HEAP_SECTION_COUNT == 100);
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Enter();
			#endif
				{
					m_seclist = (Section*)malloc(sizeof(Section) * HEAP_SECTION_COUNT);
					m_secsizelist = (GAIA::UM*)malloc(sizeof(GAIA::UM) * HEAP_SECTION_COUNT);
					for(GAIA::UM x = 0; x < HEAP_SECTION_COUNT; ++x)
					{
						Section temp;
						temp.oblist = GNIL;
						temp.uObListSize = 0;
						temp.uObListCapacity = 0;
						temp.freestack = GNIL;
						temp.uFreeStackSize = 0;
						temp.uFreeStackCapacity = 0;
						temp.uMinFreeIndex = (GAIA::UM)GINVALID;
						temp.uMinFreeSize = (GAIA::UM)GINVALID;
						m_seclist[x] = temp;
						m_secsizelist[x] = this->GetSectionPatchSize(x);
					}
				}
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Leave();
			#endif
			}
			GINL GAIA::GVOID ReleaseHeap()
			{
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Enter();
			#endif
				{
					if(m_seclist != GNIL)
					{
						for(GAIA::UM x = 0; x < HEAP_SECTION_COUNT; ++x)
						{
							Section& sec = m_seclist[x];
							if(sec.oblist != GNIL)
							{
								for(GAIA::UM y = 0; y < sec.uObListSize; ++y)
								{
									OriginBuffer& ob = sec.oblist[y];
									if(ob.freestack != GNIL)
										free(ob.freestack);
									if(ob.buf != GNIL)
										free(ob.buf);
								}
								free(sec.oblist);
							}
							if(sec.freestack != GNIL)
								free(sec.freestack);
						}
						free(m_seclist);
						m_seclist = GNIL;
						m_uSecListSize = 0;
					}
					if(m_secsizelist != GNIL)
					{
						free(m_secsizelist);
						m_secsizelist = GNIL;
						m_uSecSizeListSize = 0;
					}
				}
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Leave();
			#endif
			}
			template<typename _DataType> GAIA::GVOID push(const _DataType& t, _DataType*& p, GAIA::UM& uSize, GAIA::UM& uCapacity)
			{
				if(uSize == uCapacity)
				{
					GAIA::UM uNewCapacity = uCapacity * 2 + 1;
					_DataType* pNew = (_DataType*)malloc(sizeof(_DataType) * uNewCapacity);
					if(p != GNIL)
					{
						for(GAIA::UM x = 0; x < uSize; ++x)
							pNew[x] = p[x];
						free(p);
					}
					p = pNew;
					uCapacity = uNewCapacity;
				}
				GAST(uSize < uCapacity);
				p[uSize++] = t;
			}
		private:
			static const GAIA::UM HEAP_SECTION_COUNT = 100;

			// HEAP_BUFFER_HEADERSIZE % sizeof(GAIA::GVOID*) must equal zero.
			// First sizeof is used for save the memory alloced size,
			// second sizeof is origin buffer index, other is used for align to machine length.
		#if defined(GAIA_HEAP_LIMIT4G) || GAIA_MACHINE == GAIA_MACHINE32
			static const GAIA::UM HEAP_BUFFER_HEADERSIZE = sizeof(ALLOC_SIZE_TYPE) + sizeof(GAIA::U16) + sizeof(GAIA::U16);
		#else
			static const GAIA::UM HEAP_BUFFER_HEADERSIZE = sizeof(ALLOC_SIZE_TYPE) + sizeof(GAIA::U16) + sizeof(GAIA::U16) + sizeof(GAIA::U32);
		#endif

		private:
			Section* m_seclist;
			GAIA::UM m_uSecListSize;
			GAIA::UM* m_secsizelist;
			GAIA::UM m_uSecSizeListSize;
			GAIA::SYNC::Atomic m_capacity;
			GAIA::SYNC::Atomic m_size;
			GAIA::SYNC::Atomic m_usesize;
			GAIA::SYNC::Atomic m_piecesize;
			GAIA::SYNC::Atomic m_alloctimes;
		#ifdef GAIA_HEAP_THREADSAFE
			GAIA::SYNC::LockPure m_lr;
		#endif
		};
	}
}

#endif
