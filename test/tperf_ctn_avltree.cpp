#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID tperf_ctn_avltree(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 100000;

		typedef GAIA::CTN::BasicAVLTree<GAIA::NUM, GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AVLTreeType;

		__AVLTreeType avl;
		GAIA::MATH::RandomLCG lcg;

		lcg.seed(0);
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
			avl.insert(lcg.random());

		lcg.seed(0);
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
			avl.find(lcg.random());

		lcg.seed(0);
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
			avl.erase(lcg.random());

		GAST(avl.size() == 0);
	}
}
