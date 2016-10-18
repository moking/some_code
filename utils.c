/*
 * =====================================================================================
 *
 *       Filename:  utils.c
 *
 *    Description:  definition of some ofen used functions
 *
 *        Version:  1.0
 *        Created:  10/18/2016 10:31:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  nifan.man@gmail.com
 *   Organization:  UTA
 *
 * =====================================================================================
 */

#include "utils.h"

inline void 
mfence(void){
  __builtin_ia32_mfence();
}

inline void 
clflush(void *p) 
{
  __builtin_ia32_clflush(p);
}

uint64_t 
get_time_in_ns(void){
	struct timespec ts; 
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	return ts.tv_sec*1e9+ts.tv_nsec;
}



#if !Kernel_mode
void
print_current_time(void){
		time_t rawtime;
		struct tm * timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		fprintf (stderr, "%s", asctime (timeinfo) );
}

void 
nf_assert(bool bVal){
	volatile int cond=1;
	if(bVal == 0){
		fprintf(stderr, "Assert Failed: %s:%d. Use [gdb -p %d prog_name] to debug\n", __FILE__, __LINE__, getpid());
		while(cond==1)
			;
	}
}
#endif


