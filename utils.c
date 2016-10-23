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

inline void
flush(void*p){
	mfence();
	clflush(p);
	mfence();
}	

uint64_t 
get_time_in_ns(void){
	struct timespec ts; 
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	return ts.tv_sec*1e9+ts.tv_nsec;
}

inline uint64_t
rdtsc() {
    unsigned long a, d;
    asm volatile ("cpuid; rdtsc" : "=a" (a), "=d" (d) : : "ebx", "ecx");
    return a | ((uint64_t)d << 32);
}

static inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}

static inline uint32_t getblock32 ( const uint32_t * p, int i )
{
  return p[i];
}

static inline uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}


uint32_t MurmurHash3_x86_32 ( const void * key, size_t length)
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = length / 4;

  uint32_t h1 = 0;

  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

  for(int i = -nblocks; i; i++)
  {
    uint32_t k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = rotl32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = rotl32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;

  switch(length & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= length;

  h1 = fmix32(h1);

  //*(uint32_t*)out = h1;
  return h1;
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
nf_assert(bool  bVal){
	volatile int cond=1;
	if(bVal == 0){
		fprintf(stderr, "Assert Failed: %s:%d. Use [gdb -p %d prog_name] to debug\n", __FILE__, __LINE__, getpid());
		while(cond==1)
			;
	}
}

#define malloc_profile_nf
#ifdef malloc_profile_nf
static struct malloc_info_st{
	void **malloc_array;
	uint32_t item_allocted;
	uint32_t max_item_count;
} malloc_info_st = {NULL, 0, 128};
#endif

void *nf_malloc(uint64_t size){
	void *p = NULL;
	if(size == 0){
		fprintf(stderr, "Cannot malloc with size = 0\n");
		nf_assert(0);
	}
	p = malloc(size);
	if(p ==NULL){
		fprintf(stderr, "malloc %d-byte failed\n", size);
		nf_assert(0);
	}

	#ifdef malloc_profile_nf
	if(malloc_info_st.malloc_array == NULL){
		malloc_info_st.malloc_array = (void**)malloc(sizeof(void*)*malloc_info_st.max_item_count);
		nf_assert(malloc_info_st.malloc_array != NULL);
		malloc_info_st.item_allocted = 0;
	}else if(malloc_info_st.item_allocted == malloc_info_st.max_item_count){
		malloc_info_st.max_item_count *=  2;
		malloc_info_st.malloc_array = realloc(malloc_info_st.malloc_array, malloc_info_st.max_item_count*sizeof(void*));
	}
	malloc_info_st.malloc_array[malloc_info_st.item_allocted++ ] = p;
	//fprintf(stderr, "malloc %d-byte at 0x%x\n", size,p);
	#endif
	
	return p;
}

void nf_free(void *p){
	uint32_t i;
	if(p){
		#ifdef  malloc_profile_nf
		for(i=0; i<malloc_info_st.item_allocted; i++){
			if(malloc_info_st.malloc_array[i] == p){
				malloc_info_st.malloc_array[i] = NULL;
				break;
			}
		}
		if(i==malloc_info_st.item_allocted){
			fprintf(stderr, "Invalid free at 0x%x\n", p);
			nf_assert(0);
		}
		#endif
		
		//	fprintf(stderr, "[%d]:free at 0x%x\n", i, p);
		free(p);
	}
}

void deinit_malloc(void){
	#ifdef malloc_profile_nf
	uint32_t i=0, num_freed = 0;
	for(i=0; i<malloc_info_st.item_allocted; i++){
		if(malloc_info_st.malloc_array[i]){
			//fprintf(stderr, "[%d]: free at 0x%x\n", i, malloc_info_st.malloc_array[i]);
			free(malloc_info_st.malloc_array[i]);
			num_freed ++;
		}
	}
	fprintf(stderr, "Total %d items are freed by %s\n", num_freed, __FUNCTION__);
	free(malloc_info_st.malloc_array);
	#endif
}
#endif


