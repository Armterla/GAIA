#ifndef		__GAIA_CTN_BUFFERRW_H__
#define		__GAIA_CTN_BUFFERRW_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_buffer.h"
#include "gaia_ctn_list.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _SizeType, typename _ExtendType, _SizeType _BlockSize> class BasicBufferRW : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
			typedef GAIA::U8 _datatype;
		public:
			static const _SizeType _blocksize = _BlockSize;
		public:
			typedef BasicBufferRW<_SizeType, _ExtendType, _BlockSize> __MyType;
		public:
			GINL BasicBufferRW(){this->init();}
			GINL BasicBufferRW(const __MyType& src){this->init(); this->operator = (src);}
			GINL ~BasicBufferRW(){this->destroy();}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_size;}
			GINL _SizeType capacity() const
			{
				_SizeType ret = m_bufs.size() * _BlockSize;
				if(m_bufs.size() > 0)
				{
					ret -= m_bufs.front().pBuf->read_size();
					GAST(_BlockSize >= m_bufs.back().pBuf->capacity());
					ret -= _BlockSize - m_bufs.back().pBuf->capacity();
				}
				return ret;
			}
			GINL GAIA::GVOID clear(){this->destroy();}
			GINL GAIA::GVOID destroy()
			{
				for(typename __BufferListType::it it = m_bufs.frontit(); !it.empty(); ++it)
				{
					Node& n = *it;
					m_bufpool.release(n.pBuf);
				}
				m_bufs.destroy();
				this->init();
			}
			GINL _SizeType peek(GAIA::GVOID* p, const _SizeType& size) const
			{
				GAST(!!p);
				if(p == GNIL)
					return 0;
				GAST(size != 0);
				if(size <= 0)
					return 0;
				_SizeType size1 = size;
				GAIA::U8* p1 = (GAIA::U8*)p;
				for(typename __BufferListType::const_it it = m_bufs.const_frontit(); !it.empty(); ++it)
				{
					const Node& n = *it;
					_SizeType sizepeek = GAIA::ALGO::gmin(size1, n.pBuf->remain());
					n.pBuf->peek(p1, sizepeek);
					p1 += sizepeek;
					size1 -= sizepeek;
					if(size1 == 0)
						break;
				}
				return size - size1;

			}
			GINL _SizeType drop(const _SizeType& size)
			{
				GAST(size != 0);
				if(size <= 0)
					return 0;
				for(_SizeType x = 0; x < size; )
				{
					if(m_bufs.empty())
					{
						GAST(m_size == 0);
						return x;
					}
					Node& n = m_bufs.front();
					_SizeType r = GAIA::ALGO::gmin(size - x, n.pBuf->remain());
					
					n.pBuf->seek_read(r, GAIA::SEEK_TYPE_FORWARD);
					
					x += r;
					m_size -= r;
					
					if(n.pBuf->remain() == 0)
					{
						m_bufpool.release(n.pBuf);
						typename __BufferListType::it itfront = m_bufs.frontit();
						m_bufs.erase(itfront);
					}
				}
				return size;
			}
			GINL GAIA::GVOID write(const GAIA::GVOID* p, const _SizeType& size)
			{
				GAST(!!p);
				if(p == GNIL)
					return;
				GAST(size != 0);
				if(size <= 0)
					return;
				for(_SizeType x = 0; x < size; )
				{
					Node n;
					if(m_bufs.empty() || m_bufs.back().pBuf->write_size() >= _BlockSize)
					{
						n.pBuf = m_bufpool.alloc();
						n.pBuf->clear();
						m_bufs.push_back(n);
					}
					else
						n.pBuf = m_bufs.back().pBuf;

					_SizeType w = GAIA::ALGO::gmin(size - x, _BlockSize - n.pBuf->write_size());
					if(w > n.pBuf->capacity() - n.pBuf->write_size())
					{
						_SizeType newsize = n.pBuf->write_size() + w;
						_ExtendType increaser;
						newsize = GAIA::ALGO::gmax(increaser.Increase(n.pBuf->write_size()), newsize);
						newsize = GAIA::ALGO::gmin(newsize, _BlockSize);
						n.pBuf->reserve_keep(newsize);
					}

					n.pBuf->write(GSCAST(const GAIA::U8*)(p) + x, w);

					m_size += w;
					x += w;
				}
			}
			GINL _SizeType read(GAIA::GVOID* p, const _SizeType& size)
			{
				GAST(!!p);
				if(p == GNIL)
					return 0;
				GAST(size != 0);
				if(size <= 0)
					return 0;
				for(_SizeType x = 0; x < size; )
				{
					if(m_bufs.empty())
					{
						GAST(m_size == 0);
						return x;
					}
					Node& n = m_bufs.front();
					_SizeType r = GAIA::ALGO::gmin(size - x, n.pBuf->remain());

					n.pBuf->read(GSCAST(GAIA::U8*)(p) + x, r);

					x += r;
					m_size -= r;

					if(n.pBuf->remain() == 0)
					{
						m_bufpool.release(n.pBuf);
						typename __BufferListType::it itfront = m_bufs.frontit();
						m_bufs.erase(itfront);
					}
				}
				return size;
			}
			GINL GAIA::GVOID write(const __MyType& src)
			{
				if(src.empty())
					return;
				for(typename __BufferListType::it it = src.m_bufs.frontit(); !it.empty(); ++it)
				{
					Node& n = *it;
					_SizeType s = n.pBuf->remain();
					if(s > 0)
						this->write(n.pBuf->read_ptr(), s);
				}
			}
			GINL _SizeType read(__MyType& src)
			{
				src.write(*this);
				return src.size();
			}
			template<typename _ParamDataType> GAIA::GVOID write(const _ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return;
				_SizeType bytes = GAIA::ALGO::gstrlen(psz) * sizeof(_ParamDataType) + sizeof(_ParamDataType);
				this->write(psz, bytes);
			}
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return GAIA::False;
				_ParamDataType* p = psz;
				while(this->size() >= sizeof(_ParamDataType))
				{
					this->read(*p);
					if(*p == 0)
						return GAIA::True;
					++p;
				}
				*p = '\0';
			}
			template<typename _ParamDataType> GAIA::GVOID write(const _ParamDataType& obj){this->write(&obj, sizeof(obj));}
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType& obj){return this->read(&obj, sizeof(obj)) == sizeof(obj);}
			template<typename _ParamLenType, typename _ParamDataType> GAIA::GVOID writex(const _ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return;
				_ParamLenType l = GAIA::ALGO::gstrlen(psz);
				this->write(l);
				if(l > 0)
					this->write(psz, l * sizeof(_ParamDataType));
			}
			template<typename _ParamLenType, typename _ParamDataType> GAIA::GVOID writex(const _ParamDataType* psz, const _ParamLenType& l)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return;
				if(l < 0)
					return;
				this->write(l);
				if(l > 0)
					this->write(psz, l * sizeof(_ParamDataType));
			}
			template<typename _ParamDataType> _ParamDataType readx()
			{
				_ParamDataType t;
				if(!this->read(t))
					GTHROW_RET(ExceedContent, GNIL);
				return t;
			}
			template<typename _ParamLenType, typename _ParamDataType> _ParamLenType readx(_ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return (_ParamLenType)GINVALID;
				_ParamLenType l;
				if(!this->read(l))
					return (_ParamLenType)GINVALID;
				if(l < 0)
					return (_ParamLenType)GINVALID;
				if(l > 0)
				{
					if(!this->read(psz, l * sizeof(_ParamDataType)))
						return (_ParamLenType)GINVALID;
				}
				psz[l] = '\0';
				return l;
			}
			template<typename _ParamLenType, typename _ParamDataType> _ParamLenType readx(_ParamDataType* psz, const _ParamLenType& size)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return (_ParamLenType)GINVALID;
				_ParamLenType l;
				if(!this->read(l))
					return (_ParamLenType)GINVALID;
				if(l < 0)
					return (_ParamLenType)GINVALID;
				if(l > 0)
				{
					if(l >= size)
						return (_ParamLenType)GINVALID;
					if(!this->read(psz, l * sizeof(_ParamDataType)))
						return (_ParamLenType)GINVALID;
				}
				psz[l] = '\0';
				return l;
			}
			GINL GAIA::BL operator == (const __MyType& src) const
			{
				if(this->size() != src.size())
					return GAIA::False;
				typename __BufferListType::const_it srcit = src.m_bufs.const_frontit();
				typename __BufferListType::const_it dstit = m_bufs.const_frontit();
				for(;;)
				{
					if(srcit.empty() && dstit.empty())
						break;
					else if(!srcit.empty() && dstit.empty())
						return GAIA::False;
					else if(srcit.empty() && !dstit.empty())
						return GAIA::False;
					const Node& nsrc = *srcit;
					const Node& ndst = *dstit;
					_SizeType s = GAIA::ALGO::gmin(nsrc.pBuf->remain(),
						ndst.pBuf->remain());
					if(s != 0)
					{
						GAIA::N32 cmp = GAIA::ALGO::gmemcmp(nsrc.pBuf->read_ptr(), ndst.pBuf->read_ptr(), s);
						if(cmp != 0)
							return GAIA::False;
					}
					if(nsrc.pBuf->remain() == s)
						++srcit;
					if(ndst.pBuf->remain() == s)
						++dstit;
				}
				return GAIA::True;
			}
			GINL GAIA::BL operator != (const __MyType& src) const{return !this->operator == (src);}
			GINL GAIA::BL operator >= (const __MyType& src) const
			{
				if(this->size() < src.size())
					return GAIA::False;
				typename __BufferListType::const_it srcit = src.m_bufs.const_frontit();
				typename __BufferListType::const_it dstit = m_bufs.const_frontit();
				for(;;)
				{
					if(srcit.empty() && dstit.empty())
						break;
					else if(!srcit.empty() && dstit.empty())
						return GAIA::True;
					else if(srcit.empty() && !dstit.empty())
						return GAIA::False;
					const Node& nsrc = *srcit;
					const Node& ndst = *dstit;
					_SizeType s = GAIA::ALGO::gmin(nsrc.pBuf->remain(),
						ndst.pBuf->remain());
					if(s != 0)
					{
						GAIA::N32 cmp = GAIA::ALGO::gmemcmp(nsrc.pBuf->read_ptr(), ndst.pBuf->read_ptr(), s);
						if(cmp < 0)
							return GAIA::False;
					}
					if(nsrc.pBuf->remain() == s)
						++srcit;
					if(ndst.pBuf->remain() == s)
						++dstit;
				}
				return GAIA::True;
			}
			GINL GAIA::BL operator <= (const __MyType& src) const
			{
				if(this->size() > src.size())
					return GAIA::False;
				typename __BufferListType::const_it srcit = src.m_bufs.const_frontit();
				typename __BufferListType::const_it dstit = m_bufs.const_frontit();
				for(;;)
				{
					if(srcit.empty() && dstit.empty())
						break;
					else if(!srcit.empty() && dstit.empty())
						return GAIA::False;
					else if(srcit.empty() && !dstit.empty())
						return GAIA::True;
					const Node& nsrc = *srcit;
					const Node& ndst = *dstit;
					_SizeType s = GAIA::ALGO::gmin(nsrc.pBuf->remain(),
						ndst.pBuf->remain());
					if(s != 0)
					{
						GAIA::N32 cmp = GAIA::ALGO::gmemcmp(nsrc.pBuf->read_ptr(), ndst.pBuf->read_ptr(), s);
						if(cmp > 0)
							return GAIA::False;
					}
					if(nsrc.pBuf->remain() == s)
						++srcit;
					if(ndst.pBuf->remain() == s)
						++dstit;
				}
				return GAIA::True;
			}
			GINL GAIA::BL operator > (const __MyType& src) const{return !this->operator <= (src);}
			GINL GAIA::BL operator < (const __MyType& src) const{return !this->operator >= (src);}
			template<typename _ParamDataType> __MyType& operator << (const _ParamDataType* psz){this->write(psz); return *this;}
			template<typename _ParamDataType> __MyType& operator >> (_ParamDataType* psz){this->read(psz); return *this;}
			template<typename _ParamDataType> __MyType& operator << (const _ParamDataType& obj){this->write(obj); return *this;}
			template<typename _ParamDataType> __MyType& operator >> (_ParamDataType& obj){this->read(obj); return *this;}
			GINL __MyType& operator = (const __MyType& src)
			{
				this->destroy();
				for(typename __BufferListType::const_it it = src.m_bufs.const_frontit(); !it.empty(); ++it)
				{
					const Node& n = *it;
					_SizeType s = n.pBuf->remain();
					if(s > 0)
						this->write(n.pBuf->read_ptr(), s);
				}
				return *this;
			}
		private:
			GINL GAIA::GVOID init()
			{
				m_size = 0;
			}
	#ifdef GAIA_DEBUG_MACHINELENGTH
		private: // Protect for 32-64bit error.
			GINL GAIA::GVOID write(const GAIA::NM& obj){}
			GINL GAIA::BL read(GAIA::NM& obj){return GAIA::False;}
			GINL GAIA::GVOID write(const GAIA::UM& obj){}
			GINL GAIA::BL read(GAIA::UM& obj){return GAIA::False;}
			GINL GAIA::GVOID write(const GAIA::WCH& obj){}
			GINL GAIA::BL read(GAIA::WCH& obj){return GAIA::False;}
			GINL GAIA::GVOID write(const GAIA::NM* psz){}
			GINL GAIA::BL read(GAIA::NM* psz){return GAIA::False;}
			GINL GAIA::GVOID write(const GAIA::UM* psz){}
			GINL GAIA::BL read(GAIA::UM* psz){return GAIA::False;}
			GINL __MyType& operator << (const GAIA::NM& obj){return *this;}
			GINL __MyType& operator >> (GAIA::NM& obj){return *this;}
			GINL __MyType& operator << (const GAIA::UM& obj){return *this;}
			GINL __MyType& operator >> (GAIA::UM& obj){return *this;}
			GINL __MyType& operator << (const GAIA::NM* obj){return *this;} GINL __MyType& operator << (GAIA::NM* obj){return *this;}
			GINL __MyType& operator >> (GAIA::NM* obj){return *this;}
			GINL __MyType& operator << (const GAIA::UM* obj){return *this;} GINL __MyType& operator << (GAIA::UM* obj){return *this;}
			GINL __MyType& operator >> (GAIA::UM* obj){return *this;}
	#endif
		private:
			typedef GAIA::CTN::BasicBuffer<_SizeType, _ExtendType> __BufferType;
		private:
			class Node : public GAIA::Base
			{
			public:
				__BufferType* pBuf;
			};
		private:
			typedef GAIA::CTN::BasicPool<__BufferType, _SizeType, _ExtendType> __BufferPoolType;
			typedef GAIA::CTN::BasicList<Node, _SizeType, _ExtendType> __BufferListType;
		private:
			__BufferPoolType m_bufpool;
			__BufferListType m_bufs;
			_SizeType m_size;
		};
		class BufferRW : public BasicBufferRW<GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM>, 1024 * 16>{public:};
	}
}

#endif
