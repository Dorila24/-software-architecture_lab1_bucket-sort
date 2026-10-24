#include "ReuseContract.h"
#include <string.h>

// Helper functions
int16_t is_valid_iid(const UGUID* riid) {
    return riid != NULL;
}

int16_t is_valid_cid(const UGUID* rcid) {
    return rcid != NULL;
}

// IEcoUnknown implementation
typedef struct IEcoUnknownImpl {
    uint32_t refCount;
    struct IEcoUnknownVTbl* pVtbl;
} IEcoUnknownImpl;

int16_t ECOCALLMETHOD QueryInterfaceImpl(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    IEcoUnknownImpl* self = (IEcoUnknownImpl*)me;
    if (!self || !riid || !ppv) return -1;

    *ppv = NULL;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = self;
        AddRefImpl(me);
        return 0;
    }
    return -1;
}

uint32_t ECOCALLMETHOD AddRefImpl(IEcoUnknownPtr_t me) {
    IEcoUnknownImpl* self = (IEcoUnknownImpl*)me;
    if (!self) return 0;
    return ++self->refCount;
}

uint32_t ECOCALLMETHOD ReleaseImpl(IEcoUnknownPtr_t me) {
    IEcoUnknownImpl* self = (IEcoUnknownImpl*)me;
    if (!self) return 0;
    if (--self->refCount == 0) {
        free(self);
        return 0;
    }
    return self->refCount;
}

IEcoUnknownVTbl unknownVtbl = {
    QueryInterfaceImpl,
    AddRefImpl,
    ReleaseImpl
};

IEcoUnknown* createUnknown(uint32_t refCount) {
    IEcoUnknownImpl* unknown = (IEcoUnknownImpl*)malloc(sizeof(IEcoUnknownImpl));
    if (!unknown) return NULL;
    unknown->refCount = refCount;
    unknown->pVtbl = &unknownVtbl;
    return (IEcoUnknown*)unknown;
}

// IEcoLab65 implementation
typedef struct IEcoLab65Impl {
    IEcoUnknownImpl base;
    IEcoCalculatorX* calcX;
    IEcoCalculatorY* calcY;
} IEcoLab65Impl;

int16_t ECOCALLMETHOD QueryInterfaceLab65(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    IEcoLab65Impl* self = (IEcoLab65Impl*)me;
    if (!self || !riid || !ppv) return -1;

    *ppv = NULL;
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = self;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &self->base;
    } else {
        return -1;
    }
    AddRefImpl(me);
    return 0;
}

int16_t ECOCALLMETHOD SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int(me, data, count, buckets);
}

int16_t ECOCALLMETHOD SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long(me, data, count, buckets);
}

int16_t ECOCALLMETHOD SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float(me, data, count, buckets);
}

int16_t ECOCALLMETHOD SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double(me, data, count, buckets);
}

int16_t ECOCALLMETHOD SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double(me, data, count, buckets);
}

IEcoLab65VTbl lab65Vtbl = {
    QueryInterfaceLab65,
    AddRefImpl,
    ReleaseImpl,
    SortInt,
    SortLong,
    SortFloat,
    SortDouble,
    SortLongDouble
};

IEcoLab65* createLab65(IEcoSystem1* system, IEcoUnknown* outer) {
    IEcoLab65Impl* lab65 = (IEcoLab65Impl*)malloc(sizeof(IEcoLab65Impl));
    if (!lab65) return NULL;

    lab65->base.pVtbl = &unknownVtbl;
    lab65->base.refCount = 1;

    lab65->calcX = NULL;
    lab65->calcY = NULL;

    IEcoInterfaceBus1* interfaceBus = NULL;
    IEcoMemoryAllocator1* allocator = NULL;

    int16_t status = system->pVtbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&interfaceBus);
    if (status != 0) goto cleanup;

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&lab65->calcX);
    if (status != 0) goto cleanup;

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&lab65->calcY);
    if (status != 0) goto cleanup;

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != 0) goto cleanup;

cleanup:
    if (status != 0) {
        if (lab65->calcX) lab65->calcX->pVtbl->Release(lab65->calcX);
        if (lab65->calcY) lab65->calcY->pVtbl->Release(lab65->calcY);
        if (interfaceBus) interfaceBus->pVtbl->Release(interfaceBus);
        free(lab65);
    }
    return (IEcoLab65*)lab65;
}

// IEcoLab2 implementation
typedef struct IEcoLab2Impl {
    IEcoUnknownImpl base;
    IEcoCalculatorY* calcY;
    IEcoLab65* lab65;
} IEcoLab2Impl;

int16_t ECOCALLMETHOD QueryInterfaceLab2(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    IEcoLab2Impl* self = (IEcoLab2Impl*)me;
    if (!self || !riid || !ppv) return -1;

    *ppv = NULL;
    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = self;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = self->lab65;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &self->base;
    } else {
        return -1;
    }
    AddRefImpl(me);
    return 0;
}

uint32_t ECOCALLMETHOD GetVersion(IEcoLab2Ptr_t me) {
    return 2;
}

IEcoLab2VTbl lab2Vtbl = {
    QueryInterfaceLab2,
    AddRefImpl,
    ReleaseImpl,
    GetVersion
};

IEcoLab2* createLab2(IEcoSystem1* system, IEcoUnknown* outer) {
    if (outer != NULL) return NULL;

    IEcoLab2Impl* lab2 = (IEcoLab2Impl*)malloc(sizeof(IEcoLab2Impl));
    if (!lab2) return NULL;

    lab2->base.pVtbl = &unknownVtbl;
    lab2->base.refCount = 1;

    lab2->calcY = NULL;
    lab2->lab65 = NULL;

    IEcoInterfaceBus1* interfaceBus = NULL;
    IEcoMemoryAllocator1* allocator = NULL;

    int16_t status = system->pVtbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&interfaceBus);
    if (status != 0) goto cleanup;

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&lab2->calcY);
    if (status != 0) goto cleanup;

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &CID_EcoLab65, NULL, &IID_IEcoUnknown, (voidptr_t*)&lab2->lab65);
    if (status != 0) goto cleanup;

    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != 0) goto cleanup;

cleanup:
    if (status != 0) {
        if (lab2->calcY) lab2->calcY->pVtbl->Release(lab2->calcY);
        if (lab2->lab65) lab2->lab65->pVtbl->Release(lab2->lab65);
        if (interfaceBus) interfaceBus->pVtbl->Release(interfaceBus);
        free(lab2);
    }
    return (IEcoLab2*)lab2;
}

// Implementations of createEcoLab65 and createEcoLab2
int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!system || !iid || !out) return -1;
    *out = NULL;

    IEcoSystem1* ecoSystem = NULL;
    int16_t status = system->pVtbl->QueryInterface(system, &IID_IEcoSystem1, (voidptr_t*)&ecoSystem);
    if (status != 0) return -1;

    IEcoLab65* lab65 = createLab65(ecoSystem, outer);
    if (!lab65) {
        ecoSystem->pVtbl->Release(ecoSystem);
        return -1;
    }

    status = lab65->pVtbl->QueryInterface(lab65, iid, out);
    if (status != 0) {
        lab65->pVtbl->Release(lab65);
        ecoSystem->pVtbl->Release(ecoSystem);
        return -1;
    }

    ecoSystem->pVtbl->Release(ecoSystem);
    return 0;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!system || !iid || !out) return -1;
    *out = NULL;

    IEcoSystem1* ecoSystem = NULL;
    int16_t status = system->pVtbl->QueryInterface(system, &IID_IEcoSystem1, (voidptr_t*)&ecoSystem);
    if (status != 0) return -1;

    IEcoLab2* lab2 = createLab2(ecoSystem, outer);
    if (!lab2) {
        ecoSystem->pVtbl->Release(ecoSystem);
        return -1;
    }

    status = lab2->pVtbl->QueryInterface(lab2, iid, out);
    if (status != 0) {
        lab2->pVtbl->Release(lab2);
        ecoSystem->pVtbl->Release(ecoSystem);
        return -1;
    }

    ecoSystem->pVtbl->Release(ecoSystem);
    return 0;
}

// Allocator implementation
IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    IEcoLab65Impl* self = (IEcoLab65Impl*)me;
    if (!self) return NULL;

    IEcoSystem1* system = NULL;
    int16_t status = self->base.pVtbl->QueryInterface((IEcoUnknown*)self, &IID_IEcoSystem1, (voidptr_t*)&system);
    if (status != 0) return NULL;

    IEcoInterfaceBus1* interfaceBus = NULL;
    status = system->pVtbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (voidptr_t*)&interfaceBus);
    if (status != 0) {
        system->pVtbl->Release(system);
        return NULL;
    }

    IEcoMemoryAllocator1* allocator = NULL;
    status = interfaceBus->pVtbl->QueryComponent(interfaceBus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != 0) {
        interfaceBus->pVtbl->Release(interfaceBus);
        system->pVtbl->Release(system);
        return NULL;
    }

    interfaceBus->pVtbl->Release(interfaceBus);
    system->pVtbl->Release(system);
    return allocator;
}
