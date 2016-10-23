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
#include <malloc.h>
#endif

typedef char int8_t; 
typedef unsigned char uint8_t; 
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long size_t;
typedef unsigned long long uint64_t;
typedef enum{
	true=1,
	false=0,
} bool;



extern void mfence(void);
extern void clflush(void *p);
extern void flush(void*p);
extern uint64_t get_time_in_ns(void);
//read ticks
extern uint64_t rdtsc(void);

extern uint32_t jenkins1_hash(char *key, size_t len);
extern uint32_t MurmurHash3_x86_32 ( const void * key, size_t length);
#if !Kernel_mode 
extern void print_current_time(void);
extern void nf_assert(bool);
extern void *nf_malloc(uint64_t size);
extern void nf_free(void *p);
extern void deinit_malloc(void);
#endif
#endif
