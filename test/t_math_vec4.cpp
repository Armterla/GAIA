#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_vec4(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::MATH::VEC4<GAIA::REAL> __RealVecType;
		typedef GAIA::MATH::VEC4<GAIA::NM> __NMVecType;

		__RealVecType rv, rv1;
		__NMVecType nmv;
		if(rv.size() != 4)
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
		rv.z = 0.0F;
		rv.w = 3.0F;
		nmv = rv;
		if(nmv.x != 1 || nmv.y != 2 || nmv.z != 0 || nmv.w != 3.0F)
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
		rv.z = 0.0F;
		rv.w = 3.0F;
		nmv = rv;
		rv.x = 3.0F;
		rv.y = 4.0F;
		rv.z = 5.0F;
		rv.w = 6.0F;
		rv = nmv.fptr();
		if(rv.x != 1.0F || rv.y != 2.0F || rv.z != 0.0F || rv.w != 3.0F)
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
		rv.z = 0.0F;
		rv.w = 1.0F;
		if(rv.length() != 5.0F)
			TERROR;
		if(rv.lengthsq() != 25.0F)
			TERROR;

		nmv = rv;
		if(nmv.length() != 5)
			TERROR;
		if(nmv.lengthsq() != 25)
			TERROR;

		if(rv.minimize() != 0.0F)
			TERROR;
		if(rv.maximize() != 4.0F)
			TERROR;

		if(nmv.minimize() != 0)
			TERROR;
		if(nmv.maximize() != 4)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		nmv.x = 0;
		nmv.y = 4;
		nmv.z = 5;
		rv.maximize(nmv);
		if(rv.minimize() != 1.0F)
			TERROR;
		if(rv.maximize() != 5.0F)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
		nmv.x = 0;
		nmv.y = 5;
		nmv.z = 6;
		nmv.w = 7;
		rv.minimize(nmv);
		if(rv.minimize() != 0.0F)
			TERROR;
		if(rv.maximize() != 4.0F)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
		nmv.x = 0;
		nmv.y = 5;
		nmv.z = 6;
		nmv.w = 7;
		nmv.maximize(rv);
		if(nmv.minimize() != 1)
			TERROR;
		if(nmv.maximize() != 7)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
		nmv.x = 0;
		nmv.y = 5;
		nmv.z = 6;
		nmv.w = 7;
		nmv.minimize(rv);
		if(nmv.minimize() != 0)
			TERROR;
		if(nmv.maximize() != 4)
			TERROR;

		rv.x = 3.0F;
		rv.y = 4.0F;
		rv.z = 0.0F;
		rv.w = 1.0F;
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
		rv.z = 3.0F;
		rv.w = 4.0F;
		nmv.x = 5;
		nmv.y = 6;
		nmv.z = 7;
		nmv.w = 8;
		__RealVecType::_datatype dotr = rv.dot(nmv);
		if(dotr != 1216.0F)
			TERROR;

		rv1 = rv.num(nmv);
		if(rv1.x != 5.0F || rv1.y != 12.0F || rv1.z != 21.0F || rv1.w != 32.0F)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
		rv1.x = 5.0F;
		rv1.y = 6.0F;
		rv1.z = 7.0F;
		rv1.w = 8.0F;
		nmv = rv + rv1;
		if(nmv.x != 6 || nmv.y != 8 || nmv.z != 10 || nmv.w != 12)
			TERROR;
		nmv = rv - rv1;
		if(nmv.x != -4 || nmv.y != -4 || nmv.z != -4 || nmv.w != -4)
			TERROR;
		nmv = rv * rv1;
		if(nmv.x != 5 || nmv.y != 12 || nmv.z != 21 || nmv.w != 32)
			TERROR;
		nmv = rv / rv1;
		if(nmv.x != 0 || nmv.y != 0)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
		nmv.x = 5;
		nmv.y = 6;
		nmv.z = 7;
		nmv.w = 8;
		rv += nmv;
		if(rv.x != 6.0F || rv.y != 8.0F || rv.z != 10.0F || rv.w != 12.0F)
			TERROR;
		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
		nmv.x = 5;
		nmv.y = 6;
		nmv.z = 7;
		nmv.w = 8;
		rv -= nmv;
		if(rv.x != -4.0F || rv.y != -4.0F || rv.z != -4.0F || rv.w != -4.0F)
			TERROR;
		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
		nmv.x = 5;
		nmv.y = 6;
		nmv.z = 7;
		nmv.w = 8;
		rv *= nmv;
		if(rv.x != 5.0F || rv.y != 12.0F || rv.z != 21.0F || rv.w != 32.0F)
			TERROR;
		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
		nmv.x = 5;
		nmv.y = 6;
		nmv.z = 7;
		nmv.w = 8;
		rv /= nmv;
		if(rv.x != 1.0F / 5.0F || rv.y != 2.0F / 6.0F || rv.z != 3.0F / 7.0F || rv.w != 4.0F / 8.0F)
			TERROR;

		rv.x = 1.0F;
		rv.y = 2.0F;
		rv.z = 3.0F;
		rv.w = 4.0F;
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
		rv.z = 3.0F;
		rv.w = 4.0F;
		if(rv[0] != 1.0F || rv[1] != 2.0F || rv[2] != 3.0F || rv[3] != 4.0F)
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
		rv.xy(rv.yx());
		TAST(GAIA::MATH::gequal(rv.x, 2.0F));
		TAST(GAIA::MATH::gequal(rv.y, 1.0F));

		rv = 1.0F;
		rv.lerp(2.0F, 0.5F);
		TAST(GAIA::MATH::gequal(rv.x, 1.5F));
		TAST(GAIA::MATH::gequal(rv.y, 1.5F));
		TAST(GAIA::MATH::gequal(rv.z, 1.5F));
		TAST(GAIA::MATH::gequal(rv.w, 1.5F));
		rv = 1.0F;
		rv.lerp(rv + 1.0F, 0.5F);
		TAST(GAIA::MATH::gequal(rv.x, 1.5F));
		TAST(GAIA::MATH::gequal(rv.y, 1.5F));
		TAST(GAIA::MATH::gequal(rv.z, 1.5F));
		TAST(GAIA::MATH::gequal(rv.w, 1.5F));
		rv = 1.0F;
		rv.lerp(rv + 1.0F, rv);
		TAST(GAIA::MATH::gequal(rv.x, 2.0F));
		TAST(GAIA::MATH::gequal(rv.y, 2.0F));
		TAST(GAIA::MATH::gequal(rv.z, 2.0F));
		TAST(GAIA::MATH::gequal(rv.w, 2.0F));

		rv = 0.0F;
		rv.try_normalize();
		if(rv.x != 0.0F || rv.y != 0.0F || rv.z != 0.0F || rv.w != 1.0F)
			TERROR;
	}
}
