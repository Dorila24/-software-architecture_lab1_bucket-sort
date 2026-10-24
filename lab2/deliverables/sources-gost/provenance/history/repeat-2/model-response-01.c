#include "ReuseContract.h"
#include <string.h>

typedef struct {
    IEcoUnknownVtbl* pVTbl;
    uint32_t refCount;
    IEcoMemoryAllocator1* allocator;
    IEcoInterfaceBus1* bus;
    IEcoCalculatorX* calculatorX;
    IEcoCalculatorY* calculatorY;
} EcoLab65Impl;

typedef struct {
    IEcoUnknownVtbl* pVTbl;
    uint32_t refCount;
    IEcoLab65Impl* lab65; // aggregated Lab65
    IEcoCalculatorY* calculatorY;
} EcoLab2Impl;

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoUnknown* me, const UGUID* riid, void** ppv) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = impl;
        AddRef(me);
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = impl->calculatorX;
        AddRef(impl->calculatorX);
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = impl->calculatorY;
        AddRef(impl->calculatorY);
        return 0;
    }
    *ppv = NULL;
    return -1;
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoUnknown* me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    return ++impl->refCount;
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoUnknown* me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    if (--impl->refCount == 0) {
        if (impl->calculatorX) Release(impl->calculatorX);
        if (impl->calculatorY) Release(impl->calculatorY);
        if (impl->allocator) Release(impl->allocator);
        if (impl->bus) Release(impl->bus);
        impl->allocator->pVTbl->Free(impl->allocator, impl);
        return 0;
    }
    return impl->refCount;
}

static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoUnknown* me, const UGUID* riid, void** ppv) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = impl;
        AddRef(me);
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = impl->calculatorY;
        AddRef(impl->calculatorY);
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = impl->lab65;
        AddRef(impl->lab65);
        return 0;
    }
    *ppv = NULL;
    return -1;
}

static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoUnknown* me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    return ++impl->refCount;
}

static uint32_t ECOCALLMETHOD Lab2_Release(IEcoUnknown* me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    if (--impl->refCount == 0) {
        if (impl->calculatorY) Release(impl->calculatorY);
        if (impl->lab65) Release(impl->lab65);
        impl->allocator->pVTbl->Free(impl->allocator, impl);
        return 0;
    }
    return impl->refCount;
}

static uint32_t ECOCALLMETHOD Lab2_GetVersion(IEcoLab2* me) {
    return 2;
}

static int16_t createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!system || !iid || !out) {
        *out = NULL;
        return -1;
    }
    IEcoSystem1* ecoSystem;
    IEcoInterfaceBus1* bus;
    int16_t status = system->pVTbl->QueryInterface(system, &IID_IEcoSystem1, (void**)&ecoSystem);
    if (status != 0) {
        return status;
    }
    status = system->pVTbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (void**)&bus);
    if (status != 0) {
        Release(ecoSystem);
        return status;
    }
    IEcoMemoryAllocator1* allocator = NULL;
    status = bus->pVTbl->QueryComponent(bus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (void**)&allocator);
    if (status != 0) {
        Release(bus);
        Release(ecoSystem);
        return status;
    }
    EcoLab65Impl* impl = (EcoLab65Impl*)allocator->pVTbl->Alloc(allocator, sizeof(EcoLab65Impl));
    if (!impl) {
        Release(allocator);
        Release(bus);
        Release(ecoSystem);
        return -2;
    }
    memset(impl, 0, sizeof(EcoLab65Impl));
    impl->pVTbl = (IEcoUnknownVtbl*)allocator->pVTbl->Alloc(allocator, sizeof(IEcoUnknownVtbl));
    impl->pVTbl->QueryInterface = Lab65_QueryInterface;
    impl->pVTbl->AddRef = Lab65_AddRef;
    impl->pVTbl->Release = Lab65_Release;
    impl->allocator = allocator;
    impl->bus = bus;
    impl->refCount = 1;

    // Query components
    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (void**)&impl->calculatorX);
    if (status != 0) {
        goto cleanup;
    }
    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (void**)&impl->calculatorY);
    if (status != 0) {
        goto cleanup;
    }

    // Check outer
    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        goto cleanup;
    }

    *out = impl;
    return 0;

cleanup:
    Release(impl->calculatorX);
    Release(impl->calculatorY);
    Release(impl->allocator);
    Release(impl->bus);
    allocator->pVTbl->Free(allocator, impl);
    return -1;
}

static int16_t createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!system || !iid || !out) {
        *out = NULL;
        return -1;
    }
    if (outer != NULL) {
        *out = NULL;
        return -1;
    }
    IEcoSystem1* ecoSystem;
    IEcoInterfaceBus1* bus;
    int16_t status = system->pVTbl->QueryInterface(system, &IID_IEcoSystem1, (void**)&ecoSystem);
    if (status != 0) {
        return status;
    }
    status = system->pVTbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (void**)&bus);
    if (status != 0) {
        Release(ecoSystem);
        return status;
    }
    IEcoMemoryAllocator1* allocator = NULL;
    status = bus->pVTbl->QueryComponent(bus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (void**)&allocator);
    if (status != 0) {
        Release(bus);
        Release(ecoSystem);
        return status;
    }
    EcoLab2Impl* impl = (EcoLab2Impl*)allocator->pVTbl->Alloc(allocator, sizeof(EcoLab2Impl));
    if (!impl) {
        Release(allocator);
        Release(bus);
        Release(ecoSystem);
        return -2;
    }
    memset(impl, 0, sizeof(EcoLab2Impl));
    impl->pVTbl = (IEcoUnknownVtbl*)allocator->pVTbl->Alloc(allocator, sizeof(IEcoUnknownVtbl));
    impl->pVTbl->QueryInterface = Lab2_QueryInterface;
    impl->pVTbl->AddRef = Lab2_AddRef;
    impl->pVTbl->Release = Lab2_Release;
    impl->allocator = allocator;
    impl->refCount = 1;

    // Query components
    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (void**)&impl->calculatorY);
    if (status != 0) {
        goto cleanup;
    }
    status = createEcoLab65(system, NULL, &IID_IEcoUnknown, (void**)&impl->lab65);
    if (status != 0) {
        goto cleanup;
    }

    *out = impl;
    return 0;

cleanup:
    Release(impl->calculatorY);
    Release(impl->lab65);
    Release(impl->allocator);
    Release(impl->bus);
    allocator->pVTbl->Free(allocator, impl);
    return -1;
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    return impl->allocator;
}
