#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_stackbitset(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicStackBitset<GAIA::U32, 255> __StackBitsetType;
		__StackBitsetType sb;
		if(sb.empty())
			TERROR;
		if(sb.size() != 255 * 8)
			TERROR;
		if(sb.capacity() != 255 * 8)
			TERROR;
		sb.clear();
		for(__StackBitsetType::_sizetype x = 0; x < sb.size(); ++x)
		{
			if(sb.exist(x))
			{
				TERROR;
				break;
			}
		}
		for(__StackBitsetType::_sizetype x = 0; x < sb.size(); ++x)
		{
			__StackBitsetType::_sizetype index = x;
			sb.set(index);
			if(!sb.exist(index))
			{
				TERROR;
				break;
			}
			sb.reset(index);
			if(sb.exist(index))
			{
				TERROR;
				break;
			}
			sb.inverse(index);
			if(!sb.exist(index))
			{
				TERROR;
				break;
			}
		}
		if(sb.size() != 255 * 8)
			TERROR;
		if(sb.capacity() != 255 * 8)
			TERROR;
		sb.clear();
		for(__StackBitsetType::_sizetype x = 0; x < sb.size(); ++x)
		{
			if(sb.exist(x))
			{
				TERROR;
				break;
			}
		}
		sb = ~sb;
		__StackBitsetType sb1 = sb;
		for(__StackBitsetType::_sizetype x = 0; x < sb.size(); ++x)
		{
			if(sb.exist(x) != sb1.exist(x))
			{
				TERROR;
				break;
			}
		}
		if(sb.bptr() - sb.fptr() != 254)
			TERROR;
		sb.clear();
		sb1.clear();
		sb1 = sb;
		TAST(sb1 == sb && sb1 >= sb && sb1 <= sb);
		if(sb1 != sb || sb1 > sb || sb1 < sb)
			TERROR;
		sb ^= sb1;
		sb ^= sb1;
		if(sb != sb1)
			TERROR;
		sb.clear();
		sb1.clear();
		sb ^= sb;
		if(sb != sb1)
			TERROR;
		sb.clear();
		sb1.clear();
		sb = ~sb;
		sb &= sb1;
		if(sb != sb1)
			TERROR;
		sb.clear();
		sb1.clear();
		sb = ~sb;
		sb &= sb1;
		if(sb != sb1)
			TERROR;
		sb.clear();
		sb1.clear();
		sb = ~sb;
		sb1 |= sb;
		if(sb1 != sb)
			TERROR;
		sb.clear();
		sb1.clear();
		sb = ~sb;
		sb |= sb;
		if(sb != sb)
			TERROR;
		sb.clear();
		sb1.clear();
		TAST(sb.zero());
		if(sb.one())
			TERROR;
		sb = ~sb;
		if(sb.zero())
			TERROR;
		TAST(sb.one());
		sb.reset(10);
		if(sb.zero())
			TERROR;
		if(sb.one())
			TERROR;
	}
}
