#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_graph(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicGraph<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __GraphType;

		static const GAIA::NUM SAMPLE_COUNT = 10;

		__GraphType g;
		TAST(g.empty());
		if(g.size() != 0)
			TERROR;
		if(g.capacity() != 0)
			TERROR;

		__GraphType::Node* pLastNode = GNIL;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			__GraphType::Node* pNewNode = g.insert(x, pLastNode);
			if(pNewNode == GNIL)
			{
				TERROR;
				break;
			}
			if(pLastNode != GNIL)
			{
				if(!g.unlink(*pLastNode, *pNewNode))
				{
					TERROR;
					break;
				}
				if(!g.link(*pLastNode, *pNewNode))
				{
					TERROR;
					break;
				}
			}
			pLastNode = pNewNode;
		}

		if(g.empty())
			TERROR;
		if(g.size() != SAMPLE_COUNT)
			TERROR;

		g.clear();
		g.destroy();

		TAST(g.empty());
		if(g.size() != 0)
			TERROR;
		if(g.capacity() != 0)
			TERROR;
	}
}
