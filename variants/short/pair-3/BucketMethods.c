#include "ArpoMethods.h"

/* Helper for int32_t bucket index calculation */
static uint32_t int32_bucket_index(int32_t value, uint32_t buckets) {
    uint32_t key = (uint32_t)value ^ ((uint32_t)1 << 31);
    if (buckets == 1) {
        return 0;
    }
    return key / ((~(uint32_t)0) / buckets + 1);
}

/* Helper for int64_t bucket index calculation */
static uint32_t int64_bucket_index(int64_t value, uint32_t buckets) {
    uint64_t key = (uint64_t)value ^ ((uint64_t)1 << 63);
    if (buckets == 1) {
        return 0;
    }
    return (uint32_t)(key / ((~(uint64_t)0) / buckets + 1));
}

/* Helper for float bucket index calculation */
static uint32_t float_bucket_index(float value, uint32_t buckets) {
    uint32_t index;
    if (value != value || value < -1000000 || value > 1000000) {
        return buckets - 1;
    }
    index = (uint32_t)(((value + 1000000) / 2000000) * buckets);
    if (index >= buckets) {
        return buckets - 1;
    }
    return index;
}

/* Helper for double bucket index calculation */
static uint32_t double_bucket_index(double value, uint32_t buckets) {
    uint32_t index;
    if (value != value || value < -1000000 || value > 1000000) {
        return buckets - 1;
    }
    index = (uint32_t)(((value + 1000000) / 2000000) * buckets);
    if (index >= buckets) {
        return buckets - 1;
    }
    return index;
}

/* Helper for long double bucket index calculation */
static uint32_t long_double_bucket_index(long double value, uint32_t buckets) {
    uint32_t index;
    if (value != value || value < -1000000 || value > 1000000) {
        return buckets - 1;
    }
    index = (uint32_t)(((value + 1000000) / 2000000) * buckets);
    if (index >= buckets) {
        return buckets - 1;
    }
    return index;
}

/* Macro for bucket sort body */
#define BUCKET_SORT_BODY(type, mapper, floating_flag) \
    IEcoMemoryAllocator1* allocator; \
    type* temp; \
    uint32_t* populations; \
    uint32_t* starts; \
    uint32_t* cursors; \
    uint32_t i, b, j, k; \
    type key; \
    int16_t status = 0; \
\
    if (me == 0 || count > 1000000 || buckets < 1 || buckets > 1024 || (count > 0 && data == 0)) { \
        return -1; \
    } \
\
    if (floating_flag) { \
        for (i = 0; i < count; ++i) { \
            if (data[i] != data[i] || data[i] < -1000000 || data[i] > 1000000) { \
                return -1; \
            } \
        } \
    } \
\
    if (count == 0) { \
        return 0; \
    } \
\
    allocator = lab_allocator(me); \
    if (allocator == 0) { \
        return -2; \
    } \
\
    temp = (type*)allocator->pVTbl->Alloc(allocator, count * sizeof(type)); \
    populations = (uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
    starts = (uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
    cursors = (uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
\
    if (temp == 0 || populations == 0 || starts == 0 || cursors == 0) { \
        if (temp != 0) allocator->pVTbl->Free(allocator, temp); \
        if (populations != 0) allocator->pVTbl->Free(allocator, populations); \
        if (starts != 0) allocator->pVTbl->Free(allocator, starts); \
        if (cursors != 0) allocator->pVTbl->Free(allocator, cursors); \
        return -2; \
    } \
\
    for (i = 0; i < buckets; ++i) { \
        ((volatile uint32_t*)populations)[i] = 0; \
    } \
\
    for (i = 0; i < count; ++i) { \
        b = mapper(data[i], buckets); \
        populations[b]++; \
    } \
\
    starts[0] = 0; \
    for (i = 1; i < buckets; ++i) { \
        starts[i] = starts[i - 1] + populations[i - 1]; \
    } \
\
    for (b = 0; b < buckets; ++b) { \
        cursors[b] = starts[b]; \
    } \
\
    for (i = 0; i < count; ++i) { \
        b = mapper(data[i], buckets); \
        temp[cursors[b]] = data[i]; \
        cursors[b]++; \
    } \
\
    for (b = 0; b < buckets; ++b) { \
        for (i = starts[b] + 1; i < starts[b] + populations[b]; ++i) { \
            key = temp[i]; \
            j = i; \
            while (j > starts[b] && temp[j - 1] > key) { \
                temp[j] = temp[j - 1]; \
                j--; \
            } \
            temp[j] = key; \
        } \
    } \
\
    for (i = 0; i < count; ++i) { \
        data[i] = temp[i]; \
    } \
\
    allocator->pVTbl->Free(allocator, temp); \
    allocator->pVTbl->Free(allocator, populations); \
    allocator->pVTbl->Free(allocator, starts); \
    allocator->pVTbl->Free(allocator, cursors); \
\
    return 0;

int16_t ECOCALLMETHOD lab_sort_int(IEcoLab65* me, int32_t* data, uint32_t count, uint32_t buckets) {
    BUCKET_SORT_BODY(int32_t, int32_bucket_index, 0);
}

int16_t ECOCALLMETHOD lab_sort_long(IEcoLab65* me, int64_t* data, uint32_t count, uint32_t buckets) {
    BUCKET_SORT_BODY(int64_t, int64_bucket_index, 0);
}

int16_t ECOCALLMETHOD lab_sort_float(IEcoLab65* me, float* data, uint32_t count, uint32_t buckets) {
    BUCKET_SORT_BODY(float, float_bucket_index, 1);
}

int16_t ECOCALLMETHOD lab_sort_double(IEcoLab65* me, double* data, uint32_t count, uint32_t buckets) {
    BUCKET_SORT_BODY(double, double_bucket_index, 1);
}

int16_t ECOCALLMETHOD lab_sort_long_double(IEcoLab65* me, long double* data, uint32_t count, uint32_t buckets) {
    BUCKET_SORT_BODY(long double, long_double_bucket_index, 1);
}
