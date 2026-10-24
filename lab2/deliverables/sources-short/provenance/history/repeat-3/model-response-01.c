#include "ReuseContract.h"
#include "ArpoMethods.h"
#include "IEcoLab65.h"
#include "IEcoLab2.h"
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

typedef struct {
    IEcoUnknownVtbl* pVtbl;
    uint32_t refCount;
    IEcoSystem1* system;
    IEcoMemoryAllocator1* allocator;
    IEcoCalculatorX* calculatorX;
    IEcoCalculatorY* calculatorY;
} EcoLab65Impl;

typedef struct {
    IEcoUnknownVtbl* pVtbl;
    uint32_t refCount;
    IEcoSystem1* system;
    IEcoMemoryAllocator1* allocator;
    IEcoLab65* lab65;
    IEcoCalculatorY* calculatorY;
} EcoLab2Impl;

static int16_t ECOCALLMETHOD EcoLab65_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) ||
        IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = me;
        impl->pVtbl->AddRef(me);
        return 0;
    }
    *ppv = NULL;
    return -1;
}

static uint32_t ECOCALLMETHOD EcoLab65_AddRef(IEcoUnknownPtr_t me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    return ++impl->refCount;
}

static uint32_t ECOCALLMETHOD EcoLab65_Release(IEcoUnknownPtr_t me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    if (--impl->refCount == 0) {
        if (impl->calculatorX) impl->calculatorX->pVtbl->Release(impl->calculatorX);
        if (impl->calculatorY) impl->calculatorY->pVtbl->Release(impl->calculatorY);
        if (impl->allocator) impl->allocator->pVtbl->Release(impl->allocator);
        if (impl->system) impl->system->pVtbl->Release(impl->system);
        free(impl);
        return 0;
    }
    return impl->refCount;
}

static int16_t ECOCALLMETHOD EcoLab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double((IEcoLab65*)me, data, count, buckets);
}

static IEcoUnknownVtbl EcoLab65Vtbl = {
    EcoLab65_QueryInterface,
    EcoLab65_AddRef,
    EcoLab65_Release
};

static IEcoLab65Vtbl EcoLab65Lab65Vtbl = {
    (int16_t(*)())EcoLab65_QueryInterface,
    EcoLab65_AddRef,
    EcoLab65_Release,
    EcoLab65_SortInt,
    EcoLab65_SortLong,
    EcoLab65_SortFloat,
    EcoLab65_SortDouble,
    EcoLab65_SortLongDouble
};

static int16_t ECOCALLMETHOD EcoLab2_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) ||
        IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = me;
        impl->pVtbl->AddRef(me);
        return 0;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = impl->lab65;
        impl->lab65->pVtbl->AddRef(impl->lab65);
        return 0;
    }
    *ppv = NULL;
    return -1;
}

static uint32_t ECOCALLMETHOD EcoLab2_AddRef(IEcoUnknownPtr_t me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    return ++impl->refCount;
}

static uint32_t ECOCALLMETHOD EcoLab2_Release(IEcoUnknownPtr_t me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    if (--impl->refCount == 0) {
        if (impl->calculatorY) impl->calculatorY->pVtbl->Release(impl->calculatorY);
        if (impl->lab65) impl->lab65->pVtbl->Release(impl->lab65);
        if (impl->allocator) impl->allocator->pVtbl->Release(impl->allocator);
        if (impl->system) impl->system->pVtbl->Release(impl->system);
        free(impl);
        return 0;
    }
    return impl->refCount;
}

static uint32_t ECOCALLMETHOD EcoLab2_GetVersion(IEcoLab2Ptr_t me) {
    return 2;
}

static IEcoUnknownVtbl EcoLab2Vtbl = {
    EcoLab2_QueryInterface,
    EcoLab2_AddRef,
    EcoLab2_Release
};

static IEcoLab2Vtbl EcoLab2Lab2Vtbl = {
    (int16_t(*)())EcoLab2_QueryInterface,
    EcoLab2_AddRef,
    EcoLab2_Release,
    EcoLab2_GetVersion
};

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    EcoLab65Impl* impl;
    IEcoInterfaceBus1* interfaceBus;
    IEcoInterfaceBus1MemExt* memoryExtension;
    int16_t status;

    if (outer || !system || !iid || !out) {
        *out = NULL;
        return -1;
    }

    impl = (EcoLab65Impl*)malloc(sizeof(EcoLab65Impl));
    if (!impl) {
        *out = NULL;
        return -1;
    }

    impl->pVtbl = &EcoLab65Vtbl;
    impl->refCount = 1;
    impl->system = (IEcoSystem1*)system;
    impl->allocator = NULL;
    impl->calculatorX = NULL;
    impl->calculatorY = NULL;

    status = system->pVtbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&interfaceBus);
    if (status != 0) {
        free(impl);
        *out = NULL;
        return -1;
    }

    status = interfaceBus->pVtbl->QueryInterface(interfaceBus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&memoryExtension);
    if (status != 0) {
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    status = memoryExtension->pVtbl->get_Manager(memoryExtension, (const UGUID**)&IID_IEcoMemoryAllocator1);
    if (status != 0) {
        memoryExtension->pVtbl->Release(memoryExtension);
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &IID_IEcoMemoryAllocator1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&impl->allocator);
    if (status != 0) {
        memoryExtension->pVtbl->Release(memoryExtension);
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &IID_IdEcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&impl->calculatorX);
    if (status != 0) {
        impl->allocator->pVtbl->Release(impl->allocator);
        memoryExtension->pVtbl->Release(memoryExtension);
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &IID_IdEcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&impl->calculatorY);
    if (status != 0) {
        impl->calculatorX->pVtbl->Release(impl->calculatorX);
        impl->allocator->pVtbl->Release(impl->allocator);
        memoryExtension->pVtbl->Release(memoryExtension);
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    memoryExtension->pVtbl->Release(memoryExtension);
    interfaceBus->pVtbl->Release(interfaceBus);

    impl->pVtbl = &EcoLab65Lab65Vtbl;
    *out = (void*)impl;
    return 0;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    EcoLab2Impl* impl;
    IEcoInterfaceBus1* interfaceBus;
    IEcoInterfaceBus1MemExt* memoryExtension;
    int16_t status;

    if (outer || !system || !iid || !out) {
        *out = NULL;
        return -1;
    }

    impl = (EcoLab2Impl*)malloc(sizeof(EcoLab2Impl));
    if (!impl) {
        *out = NULL;
        return -1;
    }

    impl->pVtbl = &EcoLab2Vtbl;
    impl->refCount = 1;
    impl->system = (IEcoSystem1*)system;
    impl->allocator = NULL;
    impl->lab65 = NULL;
    impl->calculatorY = NULL;

    status = system->pVtbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&interfaceBus);
    if (status != 0) {
        free(impl);
        *out = NULL;
        return -1;
    }

    status = interfaceBus->pVtbl->QueryInterface(interfaceBus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&memoryExtension);
    if (status != 0) {
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    status = memoryExtension->pVtbl->get_Manager(memoryExtension, (const UGUID**)&IID_IEcoMemoryAllocator1);
    if (status != 0) {
        memoryExtension->pVtbl->Release(memoryExtension);
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &IID_IEcoMemoryAllocator1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&impl->allocator);
    if (status != 0) {
        memoryExtension->pVtbl->Release(memoryExtension);
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &IID_IdEcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&impl->calculatorY);
    if (status != 0) {
        impl->allocator->pVtbl->Release(impl->allocator);
        memoryExtension->pVtbl->Release(memoryExtension);
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    status = createEcoLab65(system, NULL, &IID_IEcoLab65, (void**)&impl->lab65);
    if (status != 0) {
        impl->calculatorY->pVtbl->Release(impl->calculatorY);
        impl->allocator->pVtbl->Release(impl->allocator);
        memoryExtension->pVtbl->Release(memoryExtension);
        interfaceBus->pVtbl->Release(interfaceBus);
        free(impl);
        *out = NULL;
        return -1;
    }

    memoryExtension->pVtbl->Release(memoryExtension);
    interfaceBus->pVtbl->Release(interfaceBus);

    impl->pVtbl = &EcoLab2Lab2Vtbl;
    *out = (void*)impl;
    return 0;
}
