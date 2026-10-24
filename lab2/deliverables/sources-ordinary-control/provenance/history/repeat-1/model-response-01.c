#include "ReuseContract.h"
#include "SdkAbiReference.md"
#include "IEcoLab65.h"
#include "IdEcoLab65.h"
#include "IEcoLab2.h"
#include "IdEcoLab2.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoMemoryAllocator1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

typedef struct Lab65Impl {
    IEcoUnknownVTbl* pVtbl;
    uint32_t refCount;
    IEcoSystem1* system;
    IEcoInterfaceBus1* bus;
    IEcoMemoryAllocator1* allocator;
    IEcoCalculatorX* calculatorX;
    IEcoCalculatorY* calculatorY;
} Lab65Impl;

typedef struct Lab2Impl {
    IEcoUnknownVTbl* pVtbl;
    uint32_t refCount;
    IEcoSystem1* system;
    IEcoInterfaceBus1* bus;
    IEcoMemoryAllocator1* allocator;
    IEcoCalculatorY* calculatorY;
    Lab65Impl* aggregatedLab65;
} Lab2Impl;

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65Impl* impl = (Lab65Impl*)me;
    if (!ppv) return -1;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = me;
        impl->pVtbl->AddRef(me);
        return 0;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (IEcoUnknownPtr_t)me;
        impl->pVtbl->AddRef(me);
        return 0;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = (IEcoUnknownPtr_t)impl->calculatorX;
        impl->pVtbl->AddRef(me);
        return 0;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (IEcoUnknownPtr_t)impl->calculatorY;
        impl->pVtbl->AddRef(me);
        return 0;
    } else {
        *ppv = NULL;
        return -1;
    }
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoUnknownPtr_t me) {
    Lab65Impl* impl = (Lab65Impl*)me;
    return ++impl->refCount;
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoUnknownPtr_t me) {
    Lab65Impl* impl = (Lab65Impl*)me;
    if (--impl->refCount == 0) {
        if (impl->calculatorX) impl->calculatorX->pVtbl->Release(impl->calculatorX);
        if (impl->calculatorY) impl->calculatorY->pVtbl->Release(impl->calculatorY);
        if (impl->allocator) impl->allocator->pVtbl->Release(impl->allocator);
        if (impl->bus) impl->bus->pVtbl->Release(impl->bus);
        if (impl->system) impl->system->pVtbl->Release(impl->system);
        free(impl);
        return 0;
    }
    return impl->refCount;
}

static int16_t ECOCALLMETHOD Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double((IEcoLab65*)me, data, count, buckets);
}

static IEcoUnknownVTbl Lab65_Vtbl = {
    Lab65_QueryInterface,
    Lab65_AddRef,
    Lab65_Release
};

static IEcoLab65VTbl Lab65_Lab65Vtbl = {
    &Lab65_Vtbl,
    Lab65_SortInt,
    Lab65_SortLong,
    Lab65_SortFloat,
    Lab65_SortDouble,
    Lab65_SortLongDouble
};

static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2Impl* impl = (Lab2Impl*)me;
    if (!ppv) return -1;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = me;
        impl->pVtbl->AddRef(me);
        return 0;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = (IEcoUnknownPtr_t)me;
        impl->pVtbl->AddRef(me);
        return 0;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (IEcoUnknownPtr_t)impl->aggregatedLab65;
        impl->pVtbl->AddRef(me);
        return 0;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (IEcoUnknownPtr_t)impl->calculatorY;
        impl->pVtbl->AddRef(me);
        return 0;
    } else {
        *ppv = NULL;
        return -1;
    }
}

static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoUnknownPtr_t me) {
    Lab2Impl* impl = (Lab2Impl*)me;
    return ++impl->refCount;
}

static uint32_t ECOCALLMETHOD Lab2_Release(IEcoUnknownPtr_t me) {
    Lab2Impl* impl = (Lab2Impl*)me;
    if (--impl->refCount == 0) {
        if (impl->calculatorY) impl->calculatorY->pVtbl->Release(impl->calculatorY);
        if (impl->aggregatedLab65) impl->aggregatedLab65->pVtbl->Release(impl->aggregatedLab65);
        if (impl->allocator) impl->allocator->pVtbl->Release(impl->allocator);
        if (impl->bus) impl->bus->pVtbl->Release(impl->bus);
        if (impl->system) impl->system->pVtbl->Release(impl->system);
        free(impl);
        return 0;
    }
    return impl->refCount;
}

static uint32_t ECOCALLMETHOD Lab2_GetVersion(IEcoLab2Ptr_t me) {
    return 2;
}

static IEcoUnknownVTbl Lab2_Vtbl = {
    Lab2_QueryInterface,
    Lab2_AddRef,
    Lab2_Release
};

static IEcoLab2VTbl Lab2_Lab2Vtbl = {
    &Lab2_Vtbl,
    Lab2_GetVersion
};

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65Impl* impl = (Lab65Impl*)me;
    return impl->allocator;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !system || !iid) {
        if (out) *out = NULL;
        return -1;
    }

    Lab65Impl* impl = (Lab65Impl*)malloc(sizeof(Lab65Impl));
    if (!impl) {
        *out = NULL;
        return -2;
    }

    impl->pVtbl = &Lab65_Vtbl;
    impl->refCount = 1;
    impl->system = NULL;
    impl->bus = NULL;
    impl->allocator = NULL;
    impl->calculatorX = NULL;
    impl->calculatorY = NULL;

    int16_t status = system->pVtbl->QueryInterface(system, &IID_IEcoSystem1, (voidptr_t*)&impl->system);
    if (status != 0) goto cleanup;

    status = system->pVtbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&impl->bus);
    if (status != 0) goto cleanup;

    status = impl->bus->pVtbl->QueryComponent(impl->bus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&impl->allocator);
    if (status != 0) goto cleanup;

    status = impl->bus->pVtbl->QueryComponent(impl->bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&impl->calculatorX);
    if (status != 0) goto cleanup;

    status = impl->bus->pVtbl->QueryComponent(impl->bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&impl->calculatorY);
    if (status != 0) goto cleanup;

    *out = (void*)impl;
    return 0;

cleanup:
    if (impl->allocator) impl->allocator->pVtbl->Release(impl->allocator);
    if (impl->bus) impl->bus->pVtbl->Release(impl->bus);
    if (impl->system) impl->system->pVtbl->Release(impl->system);
    free(impl);
    *out = NULL;
    return status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !system || !iid) {
        if (out) *out = NULL;
        return -1;
    }

    Lab2Impl* impl = (Lab2Impl*)malloc(sizeof(Lab2Impl));
    if (!impl) {
        *out = NULL;
        return -2;
    }

    impl->pVtbl = &Lab2_Vtbl;
    impl->refCount = 1;
    impl->system = NULL;
    impl->bus = NULL;
    impl->allocator = NULL;
    impl->calculatorY = NULL;
    impl->aggregatedLab65 = NULL;

    int16_t status = system->pVtbl->QueryInterface(system, &IID_IEcoSystem1, (voidptr_t*)&impl->system);
    if (status != 0) goto cleanup;

    status = system->pVtbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&impl->bus);
    if (status != 0) goto cleanup;

    status = impl->bus->pVtbl->QueryComponent(impl->bus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&impl->allocator);
    if (status != 0) goto cleanup;

    status = impl->bus->pVtbl->QueryComponent(impl->bus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&impl->calculatorY);
    if (status != 0) goto cleanup;

    status = createEcoLab65(system, NULL, &IID_IEcoUnknown, (void**)&impl->aggregatedLab65);
    if (status != 0) goto cleanup;

    *out = (void*)impl;
    return 0;

cleanup:
    if (impl->aggregatedLab65) impl->aggregatedLab65->pVtbl->Release(impl->aggregatedLab65);
    if (impl->calculatorY) impl->calculatorY->pVtbl->Release(impl->calculatorY);
    if (impl->allocator) impl->allocator->pVtbl->Release(impl->allocator);
    if (impl->bus) impl->bus->pVtbl->Release(impl->bus);
    if (impl->system) impl->system->pVtbl->Release(impl->system);
    free(impl);
    *out = NULL;
    return status;
}
