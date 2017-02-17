#ifndef		__GAIA_TYPE_AUTO_H__
#define		__GAIA_TYPE_AUTO_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	class autorel : public GAIA::Base
	{
	public:
		GINL autorel(GAIA::RefObject* p)
		{
			GAST(p != GNIL);
			m_pRefObj = p;
		}
		GINL ~autorel()
		{
			if(m_pRefObj != GNIL)
				m_pRefObj->drop_ref();
		}
	private:
		GAIA::RefObject* m_pRefObj;
	};

	class autofree : public GAIA::Base
	{
	public:
		GINL autofree(GAIA::GVOID* p)
		{
			GAST(p != GNIL);
			m_p = p;
		}
		GINL ~autofree()
		{
			if(m_p != GNIL)
				gdel[] (GAIA::U8*)m_p;
		}
	private:
		GAIA::GVOID* m_p;
	};

	template<typename _DataType> class autodel : public GAIA::Base
	{
	public:
		GINL autodel(_DataType* p)
		{
			GAST(p != GNIL);
			m_p = p;
		}
		GINL ~autodel()
		{
			if(m_p != GNIL)
				gdel m_p;
		}
	private:
		_DataType* m_p;
	};

	template<typename _DataType> class autodels : public GAIA::Base
	{
	public:
		GINL autodels(_DataType* p)
		{
			GAST(p != GNIL);
			m_p = p;
		}
		GINL ~autodels()
		{
			if(m_p != GNIL)
				gdel[] m_p;
		}
	private:
		_DataType* m_p;
	};
}

#endif
