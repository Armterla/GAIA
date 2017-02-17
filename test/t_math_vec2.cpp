#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_vec2(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::MATH::VEC2<GAIA::REAL> __RealVecType;
		typedef GAIA::MATH::VEC2<GAIA::NM> __NMVecType;

		__RealVecType rv, rv1;
		__NMVecType nmv;
		if(rv.size() != 2)
			TERROR;
		rv = 10.0F;
		if(rv != 10.0F)
			TERROR;
		if(rv != rv)
			TERROR;
		rv = rv;
		if(rv != 10.0F)
			TERROR;
		if(rv != 10)
			TERROR;
		if(rv != rv)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv = rv;
		if(nmv.x != 1 || nmv.y != 2)
			TERROR;
		TAST(rv == nmv);
		if(rv != nmv)
			TERROR;
		TAST(rv >= nmv);
		TAST(rv <= nmv);
		if(rv > nmv)
			TERROR;
		if(rv < nmv)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv = rv;
		rv.x = 3.0F;
		rv.y = 4.0F;
		rv = nmv.fptr();
		if(rv.x != 1.0F || rv.y != 2.0F)
			TERROR;
		TAST(rv == nmv);
		if(rv != nmv)
			TERROR;
		TAST(rv >= nmv);
		TAST(rv <= nmv);
		if(rv > nmv)
			TERROR;
		if(rv < nmv)
			TERROR;

		rv.x = 3.0F;
		rv.y = 4.0F;
		if(rv.length() != 5.0F)
			TERROR;
		if(rv.lengthsq() != 25.0F)
			TERROR;

		nmv = rv;
		if(nmv.length() != 5)
			TERROR;
		if(nmv.lengthsq() != 25)
			TERROR;

		if(rv.minimize() != 3.0F)
			TERROR;
		if(rv.maximize() != 4.0F)
			TERROR;

		if(nmv.minimize() != 3)
			TERROR;
		if(nmv.maximize() != 4)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv.x = 0;
		nmv.y = 3;
		rv.maximize(nmv);
		if(rv.minimize() != 1.0F)
			TERROR;
		if(rv.maximize() != 3.0F)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv.x = 0;
		nmv.y = 3;
		rv.minimize(nmv);
		if(rv.minimize() != 0.0F)
			TERROR;
		if(rv.maximize() != 2.0F)
			TERROR;

		nmv.x = 1;
		nmv.y = 2;
		rv.x = 0.0F;
		rv.y = 3.0F;
		nmv.maximize(rv);
		if(nmv.minimize() != 1)
			TERROR;
		if(nmv.maximize() != 3)
			TERROR;

		nmv.x = 1;
		nmv.y = 2;
		rv.x = 0.0F;
		rv.y = 3.0F;
		nmv.minimize(rv);
		if(nmv.minimize() != 0)
			TERROR;
		if(nmv.maximize() != 2)
			TERROR;

		rv.x = 3.0F;
		rv.y = 4.0F;
		if(rv.isnormalized())
			TERROR;
		rv.normalize();
		TAST(rv.isnormalized());

		rv = 0;
		TAST(rv.iszero());
		rv = 1.0F;
		if(rv.iszero())
			TERROR;

		rv = 4.0F;
		rv.inverse();
		if(rv != 1.0F / 4.0F)
			TERROR;

		rv.neg();
		if(rv != -1.0F / 4.0F)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv.x = 3;
		nmv.y = 4;
		__RealVecType::_datatype dotr = rv.dot(nmv);
		if(dotr != 11.0F)
			TERROR;

		rv1 = rv.num(nmv);
		if(rv1.x != 3.0F || rv1.y != 8.0F)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv1.x = 3.0F;
		rv1.y = 4.0F;
		nmv = rv + rv1;
		if(nmv.x != 4 || nmv.y != 6)
			TERROR;
		nmv = rv - rv1;
		if(nmv.x != -2 || nmv.y != -2)
			TERROR;
		nmv = rv * rv1;
		if(nmv.x != 3 || nmv.y != 8)
			TERROR;
		nmv = rv / rv1;
		if(nmv.x != 0 || nmv.y != 0)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv.x = 3;
		nmv.y = 4;
		rv += nmv;
		if(rv.x != 4.0F || rv.y != 6.0F)
			TERROR;
		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv.x = 3;
		nmv.y = 4;
		rv -= nmv;
		if(rv.x != -2.0F || rv.y != -2.0F)
			TERROR;
		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv.x = 3;
		nmv.y = 4;
		rv *= nmv;
		if(rv.x != 3.0F || rv.y != 8.0F)
			TERROR;
		rv.x = 1.0F;
		rv.y = 2.0F;
		nmv.x = 3;
		nmv.y = 4;
		rv /= nmv;
		if(rv.x != 1.0F / 3.0F || rv.y != 2.0F / 4.0F)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		if(rv == 1.0F)
			TERROR;
		TAST(rv != 1.0F);
		TAST(rv >= 1.0F);
		if(rv <= 1.0F)
			TERROR;
		TAST(rv > 1);
		if(rv < 1)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		if(rv[0] != 1.0F || rv[1] != 2.0F)
			TERROR;

		rv = 0.0F;
		if(rv != 0.0F)
			TERROR;
		TAST(rv.isfinited());
		rv /= 0.0F;
		if(rv.isfinited())
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv = rv.yx();
		TAST(GAIA::MATH::gequal(rv.x, 2.0F));
		TAST(GAIA::MATH::gequal(rv.y, 1.0F));

		rv = 1.0F;
		rv.lerp(2.0F, 0.5F);
		TAST(GAIA::MATH::gequal(rv.x, 1.5F));
		TAST(GAIA::MATH::gequal(rv.y, 1.5F));
		rv = 1.0F;
		rv.lerp(rv + 1.0F, 0.5F);
		TAST(GAIA::MATH::gequal(rv.x, 1.5F));
		TAST(GAIA::MATH::gequal(rv.y, 1.5F));
		rv = 1.0F;
		rv.lerp(rv + 1.0F, rv);
		TAST(GAIA::MATH::gequal(rv.x, 2.0F));
		TAST(GAIA::MATH::gequal(rv.y, 2.0F));

		rv = 0.0F;
		rv.try_normalize();
		if(rv.x != 0.0F || rv.y != 0.0F)
			TERROR;
	}
}
