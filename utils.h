/*
 * =====================================================================================
 *
 *       Filename:  utils.h
 *
 *    Description:  decleration of some functions
 *
 *        Version:  1.0
 *        Created:  10/18/2016 10:32:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef NF_UTILS_H
#define NF_UTILS_H
#include <time.h>
#define Kernel_mode 0

#if !Kernel_mode 
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#endif

typedef unsigned char uint8_t; 
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef enum{
	true=1,
	false=0,
} bool;



extern void mfence(void);
extern void clflush(void *p);
extern uint64_t get_time_in_ns(void);

#if !Kernel_mode 
extern void print_current_time(void);
extern void nf_assert(bool);
#endif
#endif
