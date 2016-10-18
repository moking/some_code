/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  use as the interface to test code
 *
 *        Version:  1.0
 *        Created:  10/18/2016 10:30:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "utils.h"
#include <stdio.h>

int main(int argc, char *argv[]){
	int i=1;
    mfence();
    clflush(&i);

		printf("%llu\n",	get_time_in_ns());
		print_current_time();
		nf_assert(1);
		nf_assert(0);
    return 0;
}
