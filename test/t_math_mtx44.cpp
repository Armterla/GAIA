#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_mtx44(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::REAL __DataType;
		typedef GAIA::MATH::VEC3<__DataType> __VecType;
		typedef GAIA::MATH::MTX44<__DataType> __MtxType;

		static const __DataType DATA_LIST[] =
		{
			0.0F, 1.0F, 2.0F, 3.0F,
			4.0F, 5.0F, 6.0F, 7.0F,
			8.0F, 9.0F, 10.0F, 11.0F,
			12.0F, 13.0F, 14.0F, 15.0F,
		};

		__VecType pos;
		__MtxType mtx, mtx1, mtxResult;
		if(mtx.size() != 16)
			TERROR;
		if(mtx.sizex() != 4)
			TERROR;
		if(mtx.sizey() != 4)
			TERROR;
		if(mtx.isidentity())
			TERROR;
		mtx.identity();
		if(!mtx.isfinited())
			TERROR;
		if(!mtx.isidentity())
			TERROR;

		mtx = GAIA::MATH::gsqrt(-1.0F);
		if(mtx.isfinited())
			TERROR;

		mtx = 10.0F;
		for(GAIA::NUM x = 0; x < mtx.size(); ++x)
		{
			if(mtx[x] != 10.0F)
			{
				TERROR;
				break;
			}
		}
		mtx1 = mtx;
		for(GAIA::NUM x = 0; x < mtx1.size(); ++x)
		{
			if(mtx1[x] != 10.0F)
			{
				TERROR;
				break;
			}
		}

		mtx = DATA_LIST;
		for(GAIA::NUM x = 0; x < sizeofarray(DATA_LIST); ++x)
		{
			if(mtx[x] != DATA_LIST[x])
			{
				TERROR;
				break;
			}
		}
		mtx1 = mtx;
		for(GAIA::NUM x = 0; x < sizeofarray(DATA_LIST); ++x)
		{
			if(mtx1[x] != DATA_LIST[x])
			{
				TERROR;
				break;
			}
		}

		mtx = DATA_LIST;
		mtx.transpose();
		if(mtx == DATA_LIST)
			TERROR;
		mtx.transpose();
		if(mtx != DATA_LIST)
			TERROR;

		mtx.identity();
		mtx.translate(1.1F, 2.2F, 3.3F);
		if(mtx.isidentity())
			TERROR;
		pos = mtx.position();
		if(pos.x != 1.1F || pos.y != 2.2F || pos.z != 3.3F)
			TERROR;
		mtx1.isidentity();
		mtx1.translate(-1.1F, -2.2F, -3.3F);
		mtxResult = mtx * mtx1;
		if(!mtxResult.isalmostidentity())
			TERROR;

		mtx.identity();
		mtx.rotatex(1.23F);
		if(mtx.isidentity())
			TERROR;
		pos = mtx.position();
		if(pos != 0.0F)
			TERROR;
		mtx1.isidentity();
		mtx1.rotatex(-1.23F);
		mtxResult = mtx * mtx1;
		if(!mtxResult.isalmostidentity())
			TERROR;

		mtx.identity();
		mtx.rotatey(1.23F);
		if(mtx.isidentity())
			TERROR;
		pos = mtx.position();
		if(pos != 0.0F)
			TERROR;
		mtx1.isidentity();
		mtx1.rotatey(-1.23F);
		mtxResult = mtx * mtx1;
		if(!mtxResult.isalmostidentity())
			TERROR;

		mtx.identity();
		mtx.rotatez(1.23F);
		if(mtx.isidentity())
			TERROR;
		pos = mtx.position();
		if(pos != 0.0F)
			TERROR;
		mtx1.isidentity();
		mtx1.rotatez(-1.23F);
		mtxResult = mtx * mtx1;
		if(!mtxResult.isalmostidentity())
			TERROR;

		mtx.identity();
		mtx.rotate(__VecType(1.1F, 2.2F, 3.3F), 4.56F);
		if(mtx.isidentity())
			TERROR;
		pos = mtx.position();
		if(pos != 0.0F)
			TERROR;
		mtx.position(__VecType(1.1F, 2.2F, 3.3F));
		mtx1.scale(1.1F, 2.2F, 3.3F);
		mtx *= mtx1;
		mtx1 = mtx;
		mtx1.inverse();
		mtxResult = mtx * mtx1;
		if(!mtxResult.isalmostidentity())
			TERROR;

		mtx.identity();
		mtx.scale(1.1F, 2.2F, 3.3F);
		if(mtx.isidentity())
			TERROR;
		mtx1.isidentity();
		mtx1.scale(1.0F / 1.1F, 1.0F / 2.2F, 1.0F / 3.3F);
		mtxResult = mtx * mtx1;
		if(!mtxResult.isalmostidentity())
			TERROR;

		mtx = DATA_LIST;
		mtx1 = DATA_LIST;
		mtx1[2] *= 2.0F;
		if(mtx == mtx1)
			TERROR;
		if(mtx >= mtx1)
			TERROR;
		if(mtx > mtx1)
			TERROR;

		if(mtx == mtx1.fptr())
			TERROR;
		if(mtx >= mtx1.fptr())
			TERROR;
		if(mtx > mtx1.fptr())
			TERROR;
		mtx.ortho(123.0F, 456.0F, 1.23F, 4.56F);
		if(!mtx.isortho())
			TERROR;
		if(!GAIA::MATH::gequal(mtx.ortho_width(), 123.0F) || 
			!GAIA::MATH::gequal(mtx.ortho_height(), 456.0F) || 
			!GAIA::MATH::gequal(mtx.ortho_znear(), 1.23F) || 
			!GAIA::MATH::gequal(mtx.ortho_zfar(), 4.56F))
			TERROR;
		mtx.perspective(123.0F, 456.0F, 1.23F, 4.56F);
		if(mtx.isortho())
			TERROR;
		if(!GAIA::MATH::gequal(mtx.perspective_width(), 123.0F))
			TERROR;
		if(!GAIA::MATH::gequal(mtx.perspective_height(), 456.0F))
			TERROR;
		if(!GAIA::MATH::gequal(mtx.perspective_znear(), 1.23F))
			TERROR;
		if(!GAIA::MATH::gequal(mtx.perspective_zfar(), 4.56F))
			TERROR;
		mtx.perspectivefov(1.11F, 2.22F, 1.23F, 4.56F);
		if(mtx.isortho())
			TERROR;
		if(!GAIA::MATH::gequal(mtx.perspective_fovy(), 1.11F))
			TERROR;
		if(!GAIA::MATH::gequal(mtx.perspective_aspect(), 2.22F))
			TERROR;
		if(!GAIA::MATH::gequal(mtx.perspective_znear(), 1.23F))
			TERROR;
		if(!GAIA::MATH::gequal(mtx.perspective_zfar(), 4.56F))
			TERROR;
	}
}
