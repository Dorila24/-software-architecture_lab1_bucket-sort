#include "ArpoMethods.h"

/* Common macro for sorting logic */
#define SORT_BODY(type, map_func, is_float) \
{ \
    IEcoMemoryAllocator1* allocator; \
    type* temp_array; \
    volatile uint32_t* populations; \
    uint32_t* starts; \
    uint32_t* cursors; \
    uint32_t i, j, k; \
    type key; \
    uint32_t bucket_index; \
    uint32_t temp_size; \
    int16_t status = 0; \
\
    if (!me || buckets < 1 || buckets > 1024 || (count > 0 && !data)) { \
        return -1; \
    } \
\
    if (count == 0) { \
        return 0; \
    } \
\
    if (count > 1000000) { \
        return -1; \
    } \
\
    if (is_float) { \
        for (i = 0; i < count; ++i) { \
            key = data[i]; \
            if ((key != key) || (key < -1000000) || (key > 1000000)) { \
                return -1; \
            } \
        } \
    } \
\
    allocator = lab_allocator(me); \
    if (!allocator) { \
        return -1; \
    } \
\
    temp_size = count * sizeof(type); \
    temp_array = (type*)allocator->pVTbl->Alloc(allocator, temp_size); \
    populations = (volatile uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
    starts = (uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
    cursors = (uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
\
    if (!temp_array || !populations || !starts || !cursors) { \
        status = -2; \
        goto cleanup; \
    } \
\
    for (i = 0; i < buckets; ++i) { \
        populations[i] = 0; \
    } \
\
    for (i = 0; i < count; ++i) { \
        key = data[i]; \
        bucket_index = map_func(key, buckets); \
        populations[bucket_index]++; \
    } \
\
    starts[0] = 0; \
    for (i = 1; i < buckets; ++i) { \
        starts[i] = starts[i - 1] + populations[i - 1]; \
    } \
\
    for (i = 0; i < buckets; ++i) { \
        cursors[i] = starts[i]; \
    } \
\
    for (i = 0; i < count; ++i) { \
        key = data[i]; \
        bucket_index = map_func(key, buckets); \
        temp_array[cursors[bucket_index]] = key; \
        cursors[bucket_index]++; \
    } \
\
    for (k = 0; k < buckets; ++k) { \
        for (j = starts[k] + 1; j < starts[k] + populations[k]; ++j) { \
            key = temp_array[j]; \
            while (j > starts[k] && temp_array[j - 1] > key) { \
                temp_array[j] = temp_array[j - 1]; \
                j--; \
            } \
            temp_array[j] = key; \
        } \
    } \
\
    for (i = 0; i < count; ++i) { \
        data[i] = temp_array[i]; \
    } \
\
cleanup: \
    if (temp_array) { \
        allocator->pVTbl->Free(allocator, temp_array); \
    } \
    if (populations) { \
        allocator->pVTbl->Free(allocator, (void*)populations); \
    } \
    if (starts) { \
        allocator->pVTbl->Free(allocator, starts); \
    } \
    if (cursors) { \
        allocator->pVTbl->Free(allocator, cursors); \
    } \
\
    return status; \
}

/* Mapping functions for different types */
static uint32_t map_int32(int32_t value, uint32_t buckets) {
    uint32_t key = (uint32_t)value ^ 0x80000000;
    if (buckets == 1) {
        return 0;
    }
    return key / (((~(uint32_t)0) / buckets) + 1);
}

static uint32_t map_int64(int64_t value, uint32_t buckets) {
    uint64_t key = (uint64_t)value ^ ((uint64_t)1 << 63);
    if (buckets == 1) {
        return 0;
    }
    return key / (((~(uint64_t)0) / buckets) + 1);
}

static uint32_t map_float(float value, uint32_t buckets) {
    uint32_t index = (uint32_t)((value + 1000000) * buckets / 2000000);
    if (index >= buckets) {
        index = buckets - 1;
    }
    return index;
}

static uint32_t map_double(double value, uint32_t buckets) {
    uint32_t index = (uint32_t)((value + 1000000) * buckets / 2000000);
    if (index >= buckets) {
        index = buckets - 1;
    }
    return index;
}

static uint32_t map_long_double(long double value, uint32_t buckets) {
    uint32_t index = (uint32_t)((value + 1000000) * buckets / 2000000);
    if (index >= buckets) {
        index = buckets - 1;
    }
    return index;
}

/* Instantiate the five exported functions */
int16_t ECOCALLMETHOD lab_sort_int(IEcoLab65* me, int32_t* data, uint32_t count, uint32_t buckets) {
    SORT_BODY(int32_t, map_int32, 0)
}

int16_t ECOCALLMETHOD lab_sort_long(IEcoLab65* me, int64_t* data, uint32_t count, uint32_t buckets) {
    SORT_BODY(int64_t, map_int64, 0)
}

int16_t ECOCALLMETHOD lab_sort_float(IEcoLab65* me, float* data, uint32_t count, uint32_t buckets) {
    SORT_BODY(float, map_float, 1)
}

int16_t ECOCALLMETHOD lab_sort_double(IEcoLab65* me, double* data, uint32_t count, uint32_t buckets) {
    SORT_BODY(double, map_double, 1)
}

int16_t ECOCALLMETHOD lab_sort_long_double(IEcoLab65* me, long double* data, uint32_t count, uint32_t buckets) {
    SORT_BODY(long double, map_long_double, 1)
}
