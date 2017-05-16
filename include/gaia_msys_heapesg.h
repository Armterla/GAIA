#ifndef		__GAIA_MSYS_HEAPESG_H__
#define		__GAIA_MSYS_HEAPESG_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_msys_base.h"
#include "gaia_sync_atomic.h"
#include "gaia_sync_lockpure.h"
#include "gaia_algo_memory.h"
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
				#ifdef GAIA_HEAP_THREADSAFE
					m_uPerfCRTAllocCount++;
				#endif
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
				#ifdef GAIA_HEAP_THREADSAFE
					m_uPerfESGAllocCount++;
				#endif
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
						#ifdef GAIA_DEBUG_PERF
							m_uPerfFindMinSizeOBCount++;
						#endif
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
						if(m_uCacheSize > m_uMaxCacheSize)
							this->RecycleCacheInternal(m_uCacheSize - m_uMaxCacheSize);
						
					#ifdef GAIA_DEBUG_PERF
						m_uPerfAllocOBCount++;
					#endif
						GAIA::U16 uOriginBufferIndex;
						if(hs.uFreeStackSize == 0)
						{
							OriginBuffer newob;
							newob.buf = GNIL;
							newob.freestack = GNIL;
							newob.uFreeStackSize = 0;
							newob.uFreeStackCapacity = 0;
							newob.uCacheIndex = GINVALID;
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
						#ifdef GAIA_DEBUG_PERF
							m_uPerfFillNewOBFreeStackCount++;
						#endif
						}
						pRet = newobref.freestack[newobref.uFreeStackSize - 1];
						--newobref.uFreeStackSize;
						hs.uMinFreeIndex = uOriginBufferIndex;
						hs.uMinFreeSize = uSectionPatchCount - 1;
					}
					else
					{
					#ifdef GAIA_DEBUG_PERF
						m_uPerfUseExistOBCount++;
					#endif
						OriginBuffer& ob = hs.oblist[hs.uMinFreeIndex];
						if(ob.uCacheIndex != GINVALID)
						{
							this->PopFromCache(uSectionIndex, hs.uMinFreeIndex, ob.uCacheIndex);
							ob.uCacheIndex = GINVALID;
						}
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
				*GSCAST(ALLOC_SIZE_TYPE*)(pRet) = *GRCAST(const ALLOC_SIZE_TYPE*)(&uSize);
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
				#ifdef GAIA_HEAP_THREADSAFE
					m_uPerfCRTFreeCount++;
				#endif
					m_capacity.Add(-(GAIA::N64)this->memory_size(p) - (GAIA::N64)HEAP_BUFFER_HEADERSIZE);
					m_size.Add(-(GAIA::N64)this->memory_size(p) - (GAIA::N64)HEAP_BUFFER_HEADERSIZE);
					m_usesize.Add(-(GAIA::N64)this->memory_size(p));
					m_piecesize.Decrease();
					free(pOriginP);
					return;
				}
				else
				{
				#ifdef GAIA_HEAP_THREADSAFE
					m_uPerfESGFreeCount++;
				#endif
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
						ob.uCacheIndex = this->PushToCache(uSectionIndex, uOBIndex);
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
			GINL virtual GAIA::U64 recycle_cache(GAIA::UM uRecycleSize = GINVALID)
			{
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Enter();
			#endif
				GAIA::U64 uRet = this->RecycleCacheInternal();
			#ifdef GAIA_HEAP_THREADSAFE
				m_lr.Leave();
			#endif
				return uRet;
			}
		private:
			class OriginBuffer
			{
			public:
				GAIA::U8* buf;
				GAIA::U8** freestack;
				GAIA::UM uFreeStackSize;
				GAIA::UM uFreeStackCapacity;
				GAIA::UM uCacheIndex;
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
			class Cache
			{
			public:
				GAIA::U16 uSectionIndex;
				GAIA::U16 uOBIndex;
			};
		private:
			GINL GAIA::GVOID init()
			{
				m_seclist = GNIL;
				m_uSecListSize = 0;
				m_secsizelist = GNIL;
				m_uSecSizeListSize = 0;
				m_uCacheSize = 0;
				m_uMaxCacheSize = 1024 * 1024 * 10;
				m_pCacheHashTable = GNIL;
				m_cachelist = GNIL;
				m_uCacheListSize = 0;
				m_uCacheListCapacity = 0;
				m_cachefreestack = GNIL;
				m_uCacheFreeStackSize = 0;
				m_uCacheFreeStackCapacity = 0;
			#ifdef GAIA_DEBUG_PERF
				m_uPerfCRTAllocCount = 0;
				m_uPerfESGAllocCount = 0;
				m_uPerfCRTFreeCount = 0;
				m_uPerfESGFreeCount = 0;
				m_uPerfFindMinSizeOBCount = 0;
				m_uPerfAllocOBCount = 0;
				m_uPerfUseExistOBCount = 0;
				m_uPerfFillNewOBFreeStackCount = 0;
				m_uPerfPushCacheCount = 0;
				m_uPerfPopCacheCount = 0;
				m_uPerfRecycleCacheCount = 0;
			#endif
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
					GAST((HEAP_SECTION_COUNT * 65536 % 8) == 0);
					m_pCacheHashTable = (GAIA::U8*)malloc(HEAP_SECTION_COUNT * 65536 / 8); // 800KB.
					GAIA::ALGO::gmemset(m_pCacheHashTable, 0, HEAP_SECTION_COUNT * 65536 / 8);
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
					this->RecycleCacheInternal();
					if(m_pCacheHashTable != GNIL)
					{
						free(m_pCacheHashTable);
						m_pCacheHashTable = GNIL;
					}
					if(m_cachelist != GNIL)
					{
						free(m_cachelist);
						m_cachelist = GNIL;
						m_uCacheListSize = 0;
						m_uCacheListCapacity = 0;
					}
					if(m_cachefreestack != GNIL)
					{
						free(m_cachefreestack);
						m_cachefreestack = GNIL;
						m_uCacheFreeStackSize = 0;
						m_uCacheFreeStackCapacity = 0;
					}
					
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
			GINL GAIA::UM PushToCache(GAIA::UM uSectionIndex, GAIA::UM uOBIndex) // Return the cache index.
			{
			#ifdef GAIA_DEBUG_PERF
				m_uPerfPushCacheCount++;
			#endif
				
				GAST(uSectionIndex < 100);
				GAST(uOBIndex < 65536);
				GAIA::UM uIndex = uSectionIndex * 65536 + uOBIndex;
				GAIA::UM uByteIndex = uIndex / 8;
				GAIA::UM uBitIndex = uIndex % 8;
				if(m_pCacheHashTable[uByteIndex] & (1 << uBitIndex))
				{
					GASTFALSE;
					return GINVALID;
				}
				m_pCacheHashTable[uByteIndex] |= (GAIA::U8)((1 << uBitIndex) & 0xFF);
				
				Cache c;
				c.uSectionIndex = (GAIA::U16)uSectionIndex;
				c.uOBIndex = (GAIA::U16)uOBIndex;
				GAIA::UM uRet;
				if(m_uCacheFreeStackSize == 0)
				{
					uRet = m_uCacheListSize;
					this->push(c, m_cachelist, m_uCacheListSize, m_uCacheListCapacity);
				}
				else
				{
					m_uCacheFreeStackSize--;
					uRet = m_cachefreestack[m_uCacheFreeStackSize];
					m_cachelist[uRet] = c;
				}
				
				GAIA::UM uSectionPatchCount = this->GetSectionPatchCount(uSectionIndex);
				GAIA::UM uSectionPatchSize = m_secsizelist[uSectionIndex];
				GAIA::UM uBufSize = uSectionPatchCount * uSectionPatchSize;
				m_uCacheSize += uBufSize;
				
				return uRet;
			}
			GINL GAIA::BL PopFromCache(GAIA::UM uSectionIndex, GAIA::UM uOBIndex, GAIA::UM uCacheIndex)
			{
			#ifdef GAIA_DEBUG_PERF
				m_uPerfPopCacheCount++;
			#endif
				
				GAST(uSectionIndex < 100);
				GAST(uOBIndex < 65536);
				GAIA::UM uIndex = uSectionIndex * 65536 + uOBIndex;
				GAIA::UM uByteIndex = uIndex / 8;
				GAIA::UM uBitIndex = uIndex % 8;
				if(!(m_pCacheHashTable[uByteIndex] & (1 << uBitIndex)))
				{
					GASTFALSE;
					return GAIA::False;
				}
				m_pCacheHashTable[uByteIndex] &= ~((GAIA::U8)((1 << uBitIndex) & 0xFF));
				
				GAST(uCacheIndex >= 0 && uCacheIndex < m_uCacheListSize);
				Cache& c = m_cachelist[uCacheIndex];
				c.uSectionIndex = GINVALID;
				c.uOBIndex = GINVALID;
				this->push(uCacheIndex, m_cachefreestack, m_uCacheFreeStackSize, m_uCacheFreeStackCapacity);
				
				GAIA::UM uSectionPatchCount = this->GetSectionPatchCount(uSectionIndex);
				GAIA::UM uSectionPatchSize = m_secsizelist[uSectionIndex];
				GAIA::UM uBufSize = uSectionPatchCount * uSectionPatchSize;
				GAST(m_uCacheSize >= uBufSize);
				m_uCacheSize -= uBufSize;
				
				return GAIA::True;
			}
			GINL GAIA::U64 RecycleCacheInternal(GAIA::UM uRecycleSize = GINVALID)
			{
			#ifdef GAIA_DEBUG_PERF
				m_uPerfRecycleCacheCount++;
			#endif
				
				GAIA::U64 uRet = 0;
				for(GAIA::UM x = 0; x < m_uCacheListSize; ++x)
				{
					Cache& c = m_cachelist[x];
					if(c.uSectionIndex == (GAIA::U16)GINVALID && c.uOBIndex == (GAIA::U16)GINVALID)
						continue;
					GAST(c.uSectionIndex < 100);
					{
						GAIA::UM uIndex = c.uSectionIndex * 65536 + c.uOBIndex;
						GAIA::UM uByteIndex = uIndex / 8;
						GAIA::UM uBitIndex = uIndex % 8;
						GAST(m_pCacheHashTable[uByteIndex] & (1 << uBitIndex));
						m_pCacheHashTable[uByteIndex] &= ~((GAIA::U8)((1 << uBitIndex) & 0xFF));
					}
					Section& hs = m_seclist[c.uSectionIndex];
					OriginBuffer& ob = hs.oblist[c.uOBIndex];
					GAIA::N64 lCacheSize = (GAIA::N64)m_secsizelist[c.uSectionIndex] * (GAIA::N64)ob.uFreeStackSize;
					GAST(lCacheSize <= m_uCacheSize);
					m_uCacheSize -= lCacheSize;
					m_capacity.Add(-lCacheSize);
					if(c.uOBIndex == hs.uMinFreeIndex)
					{
						hs.uMinFreeIndex = (GAIA::UM)GINVALID;
						hs.uMinFreeSize = (GAIA::UM)GINVALID;
					}
					if(ob.freestack != GNIL)
					{
						free(ob.freestack);
						ob.freestack = GNIL;
					}
					ob.uFreeStackSize = 0;
					ob.uFreeStackCapacity = 0;
					if(ob.buf != GNIL)
					{
						free(ob.buf);
						ob.buf = GNIL;
					}
					ob.uCacheIndex = GINVALID;
					this->push(c.uOBIndex, hs.freestack, hs.uFreeStackSize, hs.uFreeStackCapacity);
					uRet += lCacheSize;
					c.uSectionIndex = GINVALID;
					c.uOBIndex = GINVALID;
					if(uRet >= uRecycleSize)
						break;
				}
				GAIA::UM uCursor = 0;
				for(GAIA::UM x = 0; x < m_uCacheListSize; ++x)
				{
					Cache& c = m_cachelist[x];
					if(c.uSectionIndex != (GAIA::U16)GINVALID && c.uOBIndex != (GAIA::U16)GINVALID)
					{
						Section& hs = m_seclist[c.uSectionIndex];
						OriginBuffer& ob = hs.oblist[c.uOBIndex];
						ob.uCacheIndex = uCursor;
						m_cachelist[uCursor++] = c;
					}
				}
				m_uCacheListSize = uCursor;
				m_uCacheFreeStackSize = 0;
				return uRet;
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
			GAIA::UM m_uCacheSize;
			GAIA::UM m_uMaxCacheSize;
			GAIA::U8* m_pCacheHashTable;
			Cache* m_cachelist;
			GAIA::UM m_uCacheListSize;
			GAIA::UM m_uCacheListCapacity;
			GAIA::UM* m_cachefreestack;
			GAIA::UM m_uCacheFreeStackSize;
			GAIA::UM m_uCacheFreeStackCapacity;
			GAIA::SYNC::Atomic m_capacity;
			GAIA::SYNC::Atomic m_size;
			GAIA::SYNC::Atomic m_usesize;
			GAIA::SYNC::Atomic m_piecesize;
			GAIA::SYNC::Atomic m_alloctimes;
			
		#ifdef GAIA_DEBUG_PERF
			GAIA::U64 m_uPerfCRTAllocCount;
			GAIA::U64 m_uPerfESGAllocCount;
			GAIA::U64 m_uPerfCRTFreeCount;
			GAIA::U64 m_uPerfESGFreeCount;
			GAIA::U64 m_uPerfFindMinSizeOBCount;
			GAIA::U64 m_uPerfAllocOBCount;
			GAIA::U64 m_uPerfUseExistOBCount;
			GAIA::U64 m_uPerfFillNewOBFreeStackCount;
			GAIA::U64 m_uPerfPushCacheCount;
			GAIA::U64 m_uPerfPopCacheCount;
			GAIA::U64 m_uPerfRecycleCacheCount;
		#endif
			
		#ifdef GAIA_HEAP_THREADSAFE
			GAIA::SYNC::LockPure m_lr;
		#endif
		};
	}
}

#endif
