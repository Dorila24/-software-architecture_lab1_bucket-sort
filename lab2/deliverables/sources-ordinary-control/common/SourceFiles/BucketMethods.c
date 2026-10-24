#include "ArpoMethods.h"

#define BUCKET_SORT_BODY(type, index_func) \
    { \
        IEcoMemoryAllocator1* allocator; \
        type* temp_data = NULL; \
        volatile uint32_t* populations = NULL; \
        uint32_t* starts = NULL; \
        uint32_t* cursors = NULL; \
        uint32_t i, j, k; \
        type key; \
        int16_t status = 0; \
        allocator = lab_allocator(me); \
        if (!allocator || count > 1000000 || buckets < 1 || buckets > 1024 || (count > 0 && !data)) { \
            return -1; \
        } \
        if (count == 0) { \
            return 0; \
        } \
        temp_data = (type*)allocator->pVTbl->Alloc(allocator, count * sizeof(type)); \
        populations = (volatile uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
        starts = (uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
        cursors = (uint32_t*)allocator->pVTbl->Alloc(allocator, buckets * sizeof(uint32_t)); \
        if (!temp_data || !populations || !starts || !cursors) { \
            status = -2; \
            goto cleanup; \
        } \
        for (i = 0; i < buckets; ++i) { \
            populations[i] = 0; \
        } \
        for (i = 0; i < count; ++i) { \
            key = data[i]; \
            populations[index_func(key, buckets)]++; \
        } \
        starts[0] = 0; \
        for (i = 1; i < buckets; ++i) { \
            starts[i] = starts[i - 1] + populations[i - 1]; \
        } \
        for (i = 0; i < buckets; ++i) { \
            cursors[i] = starts[i]; \
        } \
        for (i = 0; i < count; ++i) { \
            key = data[i]; \
            temp_data[cursors[index_func(key, buckets)]++] = key; \
        } \
        for (i = 0; i < buckets; ++i) { \
            for (j = starts[i] + 1; j < starts[i] + populations[i]; ++j) { \
                key = temp_data[j]; \
                k = j; \
                while (k > starts[i] && key < temp_data[k - 1]) { \
                    temp_data[k] = temp_data[k - 1]; \
                    k--; \
                } \
                temp_data[k] = key; \
            } \
        } \
        for (i = 0; i < count; ++i) { \
            data[i] = temp_data[i]; \
        } \
cleanup: \
        if (temp_data) { \
            allocator->pVTbl->Free(allocator, temp_data); \
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
        return status; \
    }

static uint32_t int32_index(int32_t value, uint32_t buckets) {
    uint32_t key = (uint32_t)value ^ ((uint32_t)1 << 31);
    if (buckets == 1) {
        return 0;
    }
    return key / ((~(uint32_t)0) / buckets + 1);
}

static uint32_t int64_index(int64_t value, uint32_t buckets) {
    uint64_t key = (uint64_t)value ^ ((uint64_t)1 << 63);
    if (buckets == 1) {
        return 0;
    }
    return key / ((~(uint64_t)0) / buckets + 1);
}

static uint32_t float_index(float value, uint32_t buckets) {
    uint32_t index;
    if (value != value || value < -1000000 || value > 1000000) {
        return -1;
    }
    index = (uint32_t)(((value + 1000000) / 2000000) * buckets);
    if (index >= buckets) {
        index = buckets - 1;
    }
    return index;
}

static uint32_t double_index(double value, uint32_t buckets) {
    uint32_t index;
    if (value != value || value < -1000000 || value > 1000000) {
        return -1;
    }
    index = (uint32_t)(((value + 1000000) / 2000000) * buckets);
    if (index >= buckets) {
        index = buckets - 1;
    }
    return index;
}

static uint32_t long_double_index(long double value, uint32_t buckets) {
    uint32_t index;
    if (value != value || value < -1000000 || value > 1000000) {
        return -1;
    }
    index = (uint32_t)(((value + 1000000) / 2000000) * buckets);
    if (index >= buckets) {
        index = buckets - 1;
    }
    return index;
}

int16_t ECOCALLMETHOD lab_sort_int(IEcoLab65* me, int32_t* data, uint32_t count, uint32_t buckets) {
    BUCKET_SORT_BODY(int32_t, int32_index)
}

int16_t ECOCALLMETHOD lab_sort_long(IEcoLab65* me, int64_t* data, uint32_t count, uint32_t buckets) {
    BUCKET_SORT_BODY(int64_t, int64_index)
}

int16_t ECOCALLMETHOD lab_sort_float(IEcoLab65* me, float* data, uint32_t count, uint32_t buckets) {
    uint32_t i;
    if (!me || count > 1000000 || buckets < 1 || buckets > 1024 || (count > 0 && !data)) {
        return -1;
    }
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; ++i) {
        if (data[i] != data[i] || data[i] < -1000000 || data[i] > 1000000) {
            return -1;
        }
    }
    BUCKET_SORT_BODY(float, float_index)
}

int16_t ECOCALLMETHOD lab_sort_double(IEcoLab65* me, double* data, uint32_t count, uint32_t buckets) {
    uint32_t i;
    if (!me || count > 1000000 || buckets < 1 || buckets > 1024 || (count > 0 && !data)) {
        return -1;
    }
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; ++i) {
        if (data[i] != data[i] || data[i] < -1000000 || data[i] > 1000000) {
            return -1;
        }
    }
    BUCKET_SORT_BODY(double, double_index)
}

int16_t ECOCALLMETHOD lab_sort_long_double(IEcoLab65* me, long double* data, uint32_t count, uint32_t buckets) {
    uint32_t i;
    if (!me || count > 1000000 || buckets < 1 || buckets > 1024 || (count > 0 && !data)) {
        return -1;
    }
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; ++i) {
        if (data[i] != data[i] || data[i] < -1000000 || data[i] > 1000000) {
            return -1;
        }
    }
    BUCKET_SORT_BODY(long double, long_double_index)
}
