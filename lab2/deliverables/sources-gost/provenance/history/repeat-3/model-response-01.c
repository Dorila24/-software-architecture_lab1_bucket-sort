#include "ReuseContract.h"
#include "SdkAbiReference.md"
#include "SharedFiles/IEcoLab65.h"
#include "SharedFiles/IdEcoLab65.h"
#include "SharedFiles/IEcoLab2.h"
#include "SharedFiles/IdEcoLab2.h"
#include "Vendor/Eco.CalculatorA/HeaderFiles/CEcoCalculatorA.h"
#include "Vendor/Eco.CalculatorB/HeaderFiles/CEcoCalculatorB.h"
#include "Vendor/Eco.CalculatorC/HeaderFiles/CEcoCalculatorC.h"
#include "Vendor/Eco.CalculatorD/HeaderFiles/CEcoCalculatorD.h"
#include "Vendor/Eco.CalculatorE/HeaderFiles/CEcoCalculatorE.h"

typedef struct {
    IEcoUnknownVTbl vtbl;
    uint32_t ref_count;
    IEcoSystem1* system;
    IEcoInterfaceBus1* bus;
    IEcoMemoryAllocator1* allocator;
    IEcoCalculatorX* calcX;
    IEcoCalculatorY* calcY;
} EcoLab65Impl;

typedef struct {
    IEcoUnknownVTbl vtbl;
    uint32_t ref_count;
    IEcoSystem1* system;
    IEcoInterfaceBus1* bus;
    IEcoMemoryAllocator1* allocator;
    IEcoLab65* lab65;
    IEcoCalculatorY* calcY;
} EcoLab2Impl;

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    if (!ppv) return -1;

    *ppv = NULL;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)me;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (voidptr_t)me;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = (voidptr_t)impl->calcX;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (voidptr_t)impl->calcY;
    } else {
        return -1;
    }

    ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoUnknownPtr_t me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    return ++impl->ref_count;
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoUnknownPtr_t me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    if (--impl->ref_count == 0) {
        if (impl->calcX) impl->calcX->pVTbl->Release(impl->calcX);
        if (impl->calcY) impl->calcY->pVTbl->Release(impl->calcY);
        if (impl->allocator) impl->allocator->pVTbl->Free(impl->allocator, impl);
        return 0;
    }
    return impl->ref_count;
}

static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    if (!ppv) return -1;

    *ppv = NULL;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)me;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = (voidptr_t)me;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (voidptr_t)impl->lab65;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (voidptr_t)impl->calcY;
    } else {
        return -1;
    }

    ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoUnknownPtr_t me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    return ++impl->ref_count;
}

static uint32_t ECOCALLMETHOD Lab2_Release(IEcoUnknownPtr_t me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    if (--impl->ref_count == 0) {
        if (impl->lab65) impl->lab65->pVTbl->Release(impl->lab65);
        if (impl->calcY) impl->calcY->pVTbl->Release(impl->calcY);
        if (impl->allocator) impl->allocator->pVTbl->Free(impl->allocator, impl);
        return 0;
    }
    return impl->ref_count;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !system || !iid) return -1;

    *out = NULL;

    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return -1;

    IEcoInterfaceBus1* bus = NULL;
    IEcoMemoryAllocator1* allocator = NULL;
    IEcoCalculatorX* calcX = NULL;
    IEcoCalculatorY* calcY = NULL;

    int16_t status = system->pVTbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != 0) return -1;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != 0) {
        bus->pVTbl->Release(bus);
        return -1;
    }

    void* temp = allocator->pVTbl->Alloc(allocator, sizeof(EcoLab65Impl));
    if (!temp) {
        bus->pVTbl->Release(bus);
        allocator->pVTbl->Release(allocator);
        return -2;
    }

    EcoLab65Impl* impl = (EcoLab65Impl*)temp;
    impl->vtbl.QueryInterface = Lab65_QueryInterface;
    impl->vtbl.AddRef = Lab65_AddRef;
    impl->vtbl.Release = Lab65_Release;
    impl->ref_count = 1;
    impl->system = system;
    impl->bus = bus;
    impl->allocator = allocator;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&calcX);
    if (status != 0) {
        allocator->pVTbl->Free(allocator, impl);
        bus->pVTbl->Release(bus);
        allocator->pVTbl->Release(allocator);
        return -1;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&calcY);
    if (status != 0) {
        calcX->pVTbl->Release(calcX);
        allocator->pVTbl->Free(allocator, impl);
        bus->pVTbl->Release(bus);
        allocator->pVTbl->Release(allocator);
        return -1;
    }

    impl->calcX = calcX;
    impl->calcY = calcY;

    if (outer != NULL) {
        *out = (voidptr_t)impl;
    } else {
        *out = (voidptr_t)impl;
    }

    return 0;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !system || !iid) return -1;

    *out = NULL;

    if (outer != NULL) return -1;

    IEcoInterfaceBus1* bus = NULL;
    IEcoMemoryAllocator1* allocator = NULL;
    IEcoLab65* lab65 = NULL;
    IEcoCalculatorY* calcY = NULL;

    int16_t status = system->pVTbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != 0) return -1;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != 0) {
        bus->pVTbl->Release(bus);
        return -1;
    }

    void* temp = allocator->pVTbl->Alloc(allocator, sizeof(EcoLab2Impl));
    if (!temp) {
        bus->pVTbl->Release(bus);
        allocator->pVTbl->Release(allocator);
        return -2;
    }

    EcoLab2Impl* impl = (EcoLab2Impl*)temp;
    impl->vtbl.QueryInterface = Lab2_QueryInterface;
    impl->vtbl.AddRef = Lab2_AddRef;
    impl->vtbl.Release = Lab2_Release;
    impl->ref_count = 1;
    impl->system = system;
    impl->bus = bus;
    impl->allocator = allocator;

    status = createEcoLab65(system, NULL, &IID_IEcoLab65, (void**)&lab65);
    if (status != 0) {
        allocator->pVTbl->Free(allocator, impl);
        bus->pVTbl->Release(bus);
        allocator->pVTbl->Release(allocator);
        return -1;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&calcY);
    if (status != 0) {
        lab65->pVTbl->Release(lab65);
        allocator->pVTbl->Free(allocator, impl);
        bus->pVTbl->Release(bus);
        allocator->pVTbl->Release(allocator);
        return -1;
    }

    impl->lab65 = lab65;
    impl->calcY = calcY;

    *out = (voidptr_t)impl;

    return 0;
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    return impl->allocator;
}
