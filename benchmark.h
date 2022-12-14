#include <chrono>
#include <stdint.h>

typedef std::chrono::high_resolution_clock Clock;

#define RDTSC_START(cycles)                                                    \
  do {                                                                         \
    unsigned cyc_high, cyc_low;                                                \
    __asm volatile("cpuid\n\t"                                                 \
                   "rdtsc\n\t"                                                 \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

#define RDTSC_FINAL(cycles)                                                    \
  do {                                                                         \
    unsigned cyc_high, cyc_low;                                                \
    __asm volatile("rdtscp\n\t"                                                \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   "cpuid\n\t"                                                 \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

/*
 * Prints the best number of operations per cycle where
 * test is the function call, answer is the expected answer generated by
 * test, repeat is the number of times we should repeat and size is the
 * number of operations represented by test.
 */
#define BEST_TIME(test, pre, repeat, size)                                     \
  do {                                                                         \
    printf("%-60s: ", #test);                                                  \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    for (int i = 0; i < repeat; i++) {                                         \
      pre;                                                                     \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      RDTSC_START(cycles_start);                                               \
      test;                                                                    \
      RDTSC_FINAL(cycles_final);                                               \
      cycles_diff = (cycles_final - cycles_start);                             \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
    }                                                                          \
    uint64_t S = size;                                                         \
    float ns_per_op = (min_diff) / (double)S;                               \
    printf(" %.2f cycles per input key ", ns_per_op);                       \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)
#define BEST_TIME_NS(test, pre, repeat, size, verbose)                         \
  do {                                                                         \
    if (verbose)                                                               \
      printf("%-60s: ", #test);                                                \
    fflush(NULL);                                                              \
    int64_t min_diff = INT64_MAX;                                              \
    for (int i = 0; i < repeat; i++) {                                         \
      pre;                                                                     \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      auto t0 = Clock::now();                                                  \
      test;                                                                    \
      auto t1 = Clock::now();                                                  \
      auto cycles_diff =                                                       \
          std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0)        \
              .count();                                                        \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
    }                                                                          \
    uint64_t S = size;                                                         \
    double gb =   size / (1024*1024*1024.0) * (1000*1000*1000.0) / min_diff;\
    if (verbose)                                                               \
      printf(" %d ns total, ", (int)min_diff);                                 \
    float ns_per_op = (min_diff) / (double)S;                               \
    if (verbose)                                                               \
      printf(" %.2f ns per byte ", ns_per_op);                         \
    if (verbose)                                                               \
      printf(" or %.2f GB/s ", gb);                         \
    if (verbose)                                                               \
      printf("\n");                                                            \
    if (!verbose)                                                              \
      printf("  %.2f ", ns_per_op);                                         \
    fflush(NULL);                                                              \
  } while (0)
  