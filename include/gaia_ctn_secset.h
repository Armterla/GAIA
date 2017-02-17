#ifndef		__GAIA_CTN_SECSET_H__
#define		__GAIA_CTN_SECSET_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_set.h"

namespace GAIA
{
	namespace CTN
	{
		/*!
			@brief Index Section Set Tool Class(Secset).
		*/
		template<typename _DataType, typename _SizeType, typename _HeightType, typename _ExtendType> class BasicSecset : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _HeightType _heighttype;
			typedef _ExtendType _extendtype;
		public:
			class Node : public GAIA::Base
			{
			public:
				GCLASS_COMPARE(b, Node)
			public:
				_DataType b;
				_DataType e;
			};
		public:
			typedef BasicSecset<_DataType, _SizeType, _HeightType, _ExtendType> __MyType;
			typedef GAIA::CTN::BasicSet<Node, _SizeType, _HeightType, _ExtendType> __SetType;
			typedef typename __SetType::const_it const_it;
			typedef typename __SetType::it it;
		public:
			GINL BasicSecset(){this->init();}
			GINL BasicSecset(const __MyType& src){this->init(); this->operator = (src);}

			/*!
				@brief Get the index section set is empty or not.
					
				@remarks if a index section set had been inserted and not destroyed or cleared, it will return GAIA::True.
			*/
			GINL GAIA::BL empty() const{return m_secs.empty();}
			/*!
				@brief Get the how many index sections which can't be combined in current container.
			*/
			GINL _SizeType size() const{return m_secs.size();}
			GINL GAIA::GVOID clear(){m_secs.clear();}
			GINL GAIA::GVOID destroy(){m_secs.destroy();}
			/*!
				@brief Insert a index section set[b, e].

				@param b [in] Specify the begin index of the section set.

				@param e [in] Specify the end index of the section set.
					the end index is included in the result section set. is [b, e], not [b, e).

				@param bCompatible [in]
					- If bCompatible == GAIA::False, and current operation overlap the old(existed) section set, it will cause failed.
					- If bCompatible == GAIA::True, even if current operation overlap the old(existed) section set, it will not cause failed!

				@return If insert success, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL insert(const _DataType& b, const _DataType& e, GAIA::BL bCompatible)
			{
				//
				GAST(b <= e);
				if(b > e)
					return GAIA::False;
				
				//
				Node n;
				n.b = b;
				n.e = e;

				//
				Node* pPrev;
				it itprev = m_secs.lower_equal(n);
				if(itprev.empty())
					pPrev = GNIL;
				else
					pPrev = &*itprev;

				//
				Node* pNext;
				it itnext = m_secs.upper_equal(n);
				if(itnext.empty())
					pNext = GNIL;
				else
					pNext = &*itnext;

				//
				if(!bCompatible)
				{
					if(pPrev != GNIL && n.b <= pPrev->e)
						return GAIA::False;
					if(pNext != GNIL && n.e >= pNext->b)
						return GAIA::False;
				}

				//
				if(pPrev != GNIL && n.b <= pPrev->e)
					n.b = pPrev->e + 1;
				if(pNext != GNIL && pNext->b <= n.e)
					n.e = pNext->b - 1;
				if(n.b > n.e)
					return GAIA::True;

				//
				if(pPrev != GNIL && n.b == pPrev->e + 1 && pNext != GNIL && pNext->b == n.e + 1){
					n.b = pPrev->b;
					n.e = pNext->e;
					m_secs.erase(*pPrev);
					m_secs.erase(*pNext);
					m_secs.insert(n);
				}
				else if(pPrev != GNIL && n.b == pPrev->e + 1){
					pPrev->e = n.e;
				}
				else if(pNext != GNIL && pNext->b == n.e + 1){
					n.e = pNext->e;
					m_secs.erase(*pNext);
					m_secs.insert(n);
				}
				else
					m_secs.insert(n);

				return GAIA::True;
			}
			/*!
				@brief Erase index section set which specified by param b and param e[b, e].

				@param b [in] Specify the begin index of the section set.

				@param e Specify the end index of the section set.
					the end index is included in the result section set. is [b, e], not [b, e).

				@param bCompatible [in]
					- If bCompatible == GAIA::False, and current operation overlap the old(existed) section set, it will cause failed.
					- If bCompatible == GAIA::True, even if current operation overlap the old(existed) section set, it will not cause failed!

				@return If any index has been erased, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL erase(const _DataType& b, const _DataType& e, GAIA::BL bCompatible)
			{
				//
				GAST(b <= e);
				if(b > e)
					return GAIA::False;
				if(this->empty())
					return GAIA::False;

				//
				Node n;
				n.b = b;
				if(!bCompatible)
				{
					Node* pFinded = m_secs.find(n);
					if(pFinded == GNIL)
						return GAIA::False;
					if(pFinded->e != e)
						return GAIA::False;
					return m_secs.erase(n);
				}

				it it = m_secs.lower_equal(n);
				if(it.empty())
					it = m_secs.frontit();
				for(; !it.empty(); )
				{
					Node& temp = *it;
					if(temp.b > e)
						break;
					else if(temp.e < b)
					{
						++it;
						continue;
					}

					GAIA::BL bLeftIn = temp.b >= b;
					GAIA::BL bRightIn = temp.e <= e;
					GAIA::BL bErased = GAIA::False;
					if(bLeftIn && bRightIn)
					{
						bErased = GAIA::True;
						if(!it.erase())
							return GAIA::False;
					}

					if(bLeftIn)
					{
						n.b = e + 1;
						n.e = temp.e;
						GAST(n.b <= n.e);
						if(!m_secs.insert(n))
							return GAIA::False;
						bErased = GAIA::True;
						if(!it.erase())
							return GAIA::False;
					}
					else if(bRightIn)
					{
						GAST(b >= 1);
						GAST(b - 1 >= temp.b);
						temp.e = b - 1;
					}
					else
					{
						n.b = e + 1;
						n.e = temp.e;
						GAST(n.b <= n.e);
						if(!m_secs.insert(n))
							return GAIA::False;
						GAST(b >= 1);
						GAST(b - 1 >= temp.b);
						temp.e = b - 1;
					}
					if(!bErased)
						++it;
				}

				return GAIA::True;
			}
			/*!
				@brief Erase index section set from position to +infinity[t, +inf).

				@param t [in] Specify a index of position.

				@param bCompatible [in]
					- If bCompatible == GAIA::False, and current operation overlap the old(existed) section set, it will cause failed.
					- If bCompatible == GAIA::True, even if current operation overlap the old(existed) section set, it will not cause failed!

				@return If any index has been erased, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL erase_next(const _DataType& t, GAIA::BL bCompatible)
			{
				if(this->empty())
					return GAIA::False;
				Node n;
				n.b = t;
				if(!bCompatible)
				{
					it itprev = m_secs.lower_equal(n);
					if(itprev.empty())
					{
						if(m_secs.empty())
							return GAIA::False;
						m_secs.clear();
						return GAIA::True;
					}
					else
					{
						if((*itprev).e >= t && (*itprev).b < t)
							return GAIA::False;
					}
					it itnext;
					if((*itprev).b == t)
						itnext = itprev;
					else
						itnext = m_secs.upper_equal(n);
					GAIA::BL bRet = GAIA::False;
					for(; !itnext.empty(); )
					{
						if(!itnext.erase())
							return GAIA::False;
						bRet = GAIA::True;
					}
					return bRet;
				}
				else
				{
					it it = m_secs.lower_equal(n);
					if(it.empty())
						it = m_secs.upper_equal(n);
					if(it.empty())
						return GAIA::False;
					GAIA::BL bRet = GAIA::False;
					for(; !it.empty(); )
					{
						Node& temp = *it;
						if(temp.b < t)
						{
							if(temp.e >= t)
							{
								temp.e = t - 1;
								bRet = GAIA::True;
							}
							++it;
						}
						else
						{
							if(!it.erase())
								return GAIA::False;
							bRet = GAIA::True;
						}
					}
					return bRet;
				}
			}
			/*!
				@brief Erase index section set from -infinity to  position(-inf, t].

				@param t [in] Specify a index of position.

				@param bCompatible [in]
					- If bCompatible == GAIA::False, and current operation overlap the old(existed) section set, it will cause failed.
					- If bCompatible == GAIA::True, even if current operation overlap the old(existed) section set, it will not cause failed!

				@return If any index has been erased, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL erase_prev(const _DataType& t, GAIA::BL bCompatible)
			{
				if(this->empty())
					return GAIA::False;
				Node n;
				n.b = t;
				if(!bCompatible)
				{
					it itprev = m_secs.lower_equal(n);
					if(itprev.empty())
						return GAIA::False;
					else
					{
						if((*itprev).e > t)
							return GAIA::False;
					}
					it it = m_secs.frontit();
					GAIA::BL bRet = GAIA::False;
					for(; !it.empty(); )
					{
						Node& temp = *it;
						if(temp.b > t)
							break;
						if(!it.erase())
							return GAIA::False;
						bRet = GAIA::True;
					}
					return bRet;
				}
				else
				{
					it it = m_secs.frontit();
					GAIA::BL bRet = GAIA::False;
					for(; !it.empty(); )
					{
						Node& temp = *it;
						if(temp.e > t)
							break;
						else
						{
							if(!it.erase())
								return GAIA::False;
							bRet = GAIA::True;
						}
					}
					if(!it.empty())
					{
						Node& temp = *it;
						n.b = t + 1;
						n.e = temp.e;
						if(!it.erase())
							return GAIA::False;
						if(!m_secs.insert(n))
							return GAIA::False;
						bRet = GAIA::True;
					}
					return bRet;
				}
			}
			/*!
				@brief Get the first not combined section's const iterator.

				@code
					GAIA::STREAM::STDStream stm;
					typedef GAIA::CTN::Secset<GAIA::NUM> __SecsetType;
					__SecsetType st;
					st.insert(5, 10, GAIA::False);
					st.insert(8, 12, GAIA::False);
					__SecsetType::const_it it = st.const_frontit();
					for(; !it.empty(); ++it)
					{
						const __SecsetType::Node& n = *it;
						stm << n.b << " " << n.e << "\n";
					}
				@endcode
			*/
			GINL const_it const_frontit() const{return m_secs.const_frontit();}
			GINL __MyType& operator = (const __MyType& src){m_secs = src.m_secs; return *this;}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				typename __SetType::const_it itsrc = this->const_frontit();
				typename __SetType::const_it itdst = src.const_frontit();
				for(;;)
				{
					if(itsrc.empty() && itdst.empty())
						return 0;
					else if(itsrc.empty() && !itdst.empty())
						return -1;
					else if(!itsrc.empty() && itdst.empty())
						return +1;
					else
					{
						const Node& n = *itsrc;
						const Node& nsrc = *itdst;
						if(n < nsrc)
							return -1;
						else if(n > nsrc)
							return +1;
					}
					++itsrc;
					++itdst;
				}
			}
			GCLASS_COMPARE_BYCOMPARE(__MyType)
		private:
			GINL GAIA::GVOID init(){}
		private:
			__SetType m_secs;
		};
		template<typename _DataType> class Secset : public BasicSecset<_DataType, GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
