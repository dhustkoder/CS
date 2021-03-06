#ifndef CS_ALGORITHMS_SEARCHING_LINEAR_SEARCH_H_
#define CS_ALGORITHMS_SEARCHING_LINEAR_SEARCH_H_
#include "utils.h"


static inline ConstIterator linear_search(const ConstIterator begin,
                                          const ConstIterator end,
				          const void* const target,
				          const CmpFun cmp,
					  const ConstAdvanceFun advance)
{
	for (ConstIterator it = begin; it.index < end.index; it = advance(it, 1))
		if (cmp(target, it.ptr) == 0)
			return it;

	return end;
}




#endif
