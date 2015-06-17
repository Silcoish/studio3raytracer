#ifndef KF_TIME_HEADER
#define KF_TIME_HEADER

#include "kf/kf_types.h"
#include "intrin.h"

namespace kf
{
	__forceinline u64 getCpuTime(void)
	{
#ifdef _WIN64
		return __rdtsc();
#else
		__asm rdtsc
#endif
	}
}
#endif
