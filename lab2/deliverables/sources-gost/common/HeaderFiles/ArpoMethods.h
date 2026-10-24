/* Fixed contract. Implement only SourceFiles/BucketMethods.c.
 * Ascending bucket sort; count <= 1000000; buckets in [1,1024].
 * Int/Long: full signed 32/64-bit domain. Float/Double/LongDouble:
 * finite values in [-1000000,1000000], including endpoints and signed zero.
 * Return 0 on success, -1 on invalid input, -2 on allocator failure.
 * me must be nonnull even for count=0. data may be null only for count=0.
 * Invalid input and allocation failure must leave data unchanged.
 * Preserve original numeric values exactly. No libc, math.h or third-party calls.
 * All temporary heap memory uses lab_allocator(me)->pVTbl->Alloc/Free.
 * ACOM lifetime, factory, allocator access and console client are supplied.
 */
#ifndef ARPO_METHODS_H
#define ARPO_METHODS_H
#include "IEcoLab65.h"
#include "IEcoMemoryAllocator1.h"
IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me);
int16_t ECOCALLMETHOD lab_sort_int(IEcoLab65* me, int32_t* data, uint32_t count, uint32_t buckets);
int16_t ECOCALLMETHOD lab_sort_long(IEcoLab65* me, int64_t* data, uint32_t count, uint32_t buckets);
int16_t ECOCALLMETHOD lab_sort_float(IEcoLab65* me, float* data, uint32_t count, uint32_t buckets);
int16_t ECOCALLMETHOD lab_sort_double(IEcoLab65* me, double* data, uint32_t count, uint32_t buckets);
int16_t ECOCALLMETHOD lab_sort_long_double(IEcoLab65* me, long double* data, uint32_t count, uint32_t buckets);
#endif
