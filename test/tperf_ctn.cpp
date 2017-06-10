#include "preheader.h"
#include "t_common.h"

#include <map>

namespace TEST
{
	template<typename _DataType> class STLAllocator : public std::allocator<_DataType>
	{
	public:
		typedef size_t size_type;
		typedef typename std::allocator<_DataType>::pointer pointer;
		typedef typename std::allocator<_DataType>::value_type value_type;
		typedef typename std::allocator<_DataType>::const_pointer const_pointer;
		typedef typename std::allocator<_DataType>::reference reference;
		typedef typename std::allocator<_DataType>::const_reference const_reference;
		template<class _Other> struct rebind{typedef STLAllocator<_Other> other;};
	public:
	#if GAIA_COMPILER == GAIA_COMPILER_VC
		STLAllocator() throw(){}
	#else
		STLAllocator(){}
	#endif
		STLAllocator(const STLAllocator& a) throw() : std::allocator<_DataType>(a){}
		template<typename _ParamDataType> STLAllocator(const STLAllocator<_ParamDataType>&) throw(){}
		pointer allocate(size_type count, const void* pHint = 0){return (pointer)malloc(count * sizeof(value_type));}
		void deallocate(pointer p, size_type count){free(p);}
	};

	extern GAIA::GVOID tperf_ctn(GAIA::LOG::Log& logobj)
	{
		// Container's performance compare internal.
		{
			logobj << "\t\tBegin compare Map and HashMap..." << logobj.End();

			static const GAIA::NUM SAMPLE_COUNT = 10000;

			GAIA::CTN::Vector<GAIA::X128> listUUID;
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
			{
				GAIA::X128 uid;
				uid.uuid();
				listUUID.push_back(uid);
			}
			
			GAIA::CTN::Map<GAIA::X128, GAIA::N32> m;
			GAIA::CTN::HashMap<GAIA::X128, GAIA::N32> hm;

			GAIA::U64 uMapStartTime = GAIA::TIME::tick_time();
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				m.insert(listUUID[x], -x);
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				m.find(listUUID[x]);
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				m.erase(listUUID[x]);
			GAIA::U64 uMapEndTime = GAIA::TIME::tick_time();

			GAIA::U64 uHashMapStartTime = GAIA::TIME::tick_time();
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				hm.insert(listUUID[x], -x);
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				hm.find(listUUID[x]);
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				hm.erase(listUUID[x]);
			GAIA::U64 uHashMapEndTime = GAIA::TIME::tick_time();

			logobj << "\t\tMap Time = " << uMapEndTime - uMapStartTime << "(us)" << logobj.End();
			logobj << "\t\tHashMap Time = " << uHashMapEndTime - uHashMapStartTime << "(us)"  << logobj.End();
			logobj << "\n" << logobj.End();
		}

		// Container's performance compare to stl.
		{
			logobj << "\t\tBegin compare Map and STLMap..." << logobj.End();

			static const GAIA::NUM SAMPLE_COUNT = 10000;
			
			GAIA::CTN::Vector<GAIA::X128> listUUID;
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
			{
				GAIA::X128 uid;
				uid.uuid();
				listUUID.push_back(uid);
			}

			GAIA::CTN::Map<GAIA::X128, GAIA::N32> m;
			std::map<GAIA::X128, GAIA::N32, std::less<GAIA::X128>, STLAllocator<GAIA::N32> > stdm;

			GAIA::U64 uMapStartTime = GAIA::TIME::tick_time();
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				m.insert(listUUID[x], -x);
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				m.find(listUUID[x]);
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				m.erase(listUUID[x]);
			GAIA::U64 uMapEndTime = GAIA::TIME::tick_time();

			GAIA::U64 uSTDMapStartTime = GAIA::TIME::tick_time();
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				stdm.insert(std::pair<GAIA::X128, int>(listUUID[x], -x));
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				stdm.find(listUUID[x]);
			for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				stdm.erase(listUUID[x]);
			GAIA::U64 uSTDMapEndTime = GAIA::TIME::tick_time();

			logobj << "\t\tMap Time = " << uMapEndTime - uMapStartTime << "(us)" << logobj.End();
			logobj << "\t\tStdMap Time = " << uSTDMapEndTime - uSTDMapStartTime << "(us)"  << logobj.End();
			logobj << "\n" << logobj.End();
		}
	}
}
