#include "preheader.h"
#include "t_common.h"

namespace TEST
{
    extern GAIA::GVOID t_iterator(GAIA::LOG::Log& logobj)
    {
        GAIA::CTN::Vector<GAIA::NUM> v;
        for(GAIA::NUM x = 0; x < 100; ++x)
            v.push_back(x);
        GAIA::CTN::Vector<GAIA::NUM>::it it = v.frontit();
        GAIA::ITERATOR::Iterator<GAIA::NUM>& itbase = it;
        for(GAIA::NUM x = 0; x < v.size(); ++x)
        {
            if(*itbase != x)
            {
                TERROR;
                break;
            }
            ++itbase;
        }
        itbase.clear();
        if(!it.empty())
            TERROR;
        if(!itbase.empty())
            TERROR;
        GAIA::ITERATOR::InverseIterator<GAIA::NUM, GAIA::CTN::Vector<GAIA::NUM>::it> invit = v.backit();
        for(GAIA::NUM x = v.size() - 1; x >= 0; --x)
        {
            if(*invit != x)
            {
                TERROR;
                break;
            }
            ++invit;
        }
        invit.clear();
        if(!invit.empty())
            TERROR;
    }
}
