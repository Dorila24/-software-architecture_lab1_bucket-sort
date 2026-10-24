#include "ReuseContract.h"
#include "ArpoMethods.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoLab65.h"
#include "IEcoLab2.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoMemoryAllocator1.h"

typedef struct {
    IEcoUnknownVTbl* pVtbl;
    uint32_t refCount;
    IEcoMemoryAllocator1* allocator;
    IEcoCalculatorX* calculatorX;
    IEcoCalculatorY* calculatorY;
} EcoLab65Impl;

typedef struct {
    IEcoUnknownVTbl* pVtbl;
    uint32_t refCount;
    IEcoLab65Impl* lab65;
} EcoLab2Impl;

int16_t ECOCALLMETHOD EcoLab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = me;
        ((IEcoUnknown*)me)->pVTbl->AddRef(me);
        return 0;
    }
    *ppv = NULL;
    return -1;
}

uint32_t ECOCALLMETHOD EcoLab65_AddRef(IEcoLab65Ptr_t me) {
    return ++((EcoLab65Impl*)me)->refCount;
}

uint32_t ECOCALLMETHOD EcoLab65_Release(IEcoLab65Ptr_t me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    if (--impl->refCount == 0) {
        if (impl->calculatorX) {
            impl->calculatorX->pVTbl->Release(impl->calculatorX);
        }
        if (impl->calculatorY) {
            impl->calculatorY->pVTbl->Release(impl->calculatorY);
        }
        impl->allocator->pVTbl->Free(impl->allocator, impl);
        return 0;
    }
    return impl->refCount;
}

int16_t ECOCALLMETHOD EcoLab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int(me, data, count, buckets);
}

int16_t ECOCALLMETHOD EcoLab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long(me, data, count, buckets);
}

int16_t ECOCALLMETHOD EcoLab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float(me, data, count, buckets);
}

int16_t ECOCALLMETHOD EcoLab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double(me, data, count, buckets);
}

int16_t ECOCALLMETHOD EcoLab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double(me, data, count, buckets);
}

int16_t ECOCALLMETHOD EcoLab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = me;
        ((IEcoUnknown*)me)->pVTbl->AddRef(me);
        return 0;
    }
    *ppv = NULL;
    return -1;
}

uint32_t ECOCALLMETHOD EcoLab2_AddRef(IEcoLab2Ptr_t me) {
    return ++((EcoLab2Impl*)me)->refCount;
}

uint32_t ECOCALLMETHOD EcoLab2_Release(IEcoLab2Ptr_t me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    if (--impl->refCount == 0) {
        if (impl->lab65) {
            impl->lab65->pVTbl->Release(impl->lab65);
        }
        impl->allocator->pVTbl->Free(impl->allocator, impl);
        return 0;
    }
    return impl->refCount;
}

uint32_t ECOCALLMETHOD EcoLab2_GetVersion(IEcoLab2Ptr_t me) {
    return 2;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !iid) {
        return -1;
    }
    *out = NULL;
    if (outer != NULL || !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return -1;
    }

    EcoLab65Impl* impl = (EcoLab65Impl*)lab_allocator(NULL)->pVTbl->Alloc(lab_allocator(NULL), sizeof(EcoLab65Impl));
    if (!impl) {
        return -2;
    }

    impl->pVtbl = (IEcoUnknownVTbl*)lab_allocator(NULL)->pVTbl->Alloc(lab_allocator(NULL), sizeof(IEcoUnknownVTbl));
    if (!impl->pVtbl) {
        lab_allocator(NULL)->pVTbl->Free(lab_allocator(NULL), impl);
        return -2;
    }

    impl->pVtbl->QueryInterface = (int16_t(ECOCALLMETHOD*)(IEcoUnknownPtr_t, const UGUID*, voidptr_t*))EcoLab65_QueryInterface;
    impl->pVtbl->AddRef = (uint32_t(ECOCALLMETHOD*)(IEcoUnknownPtr_t))EcoLab65_AddRef;
    impl->pVtbl->Release = (uint32_t(ECOCALLMETHOD*)(IEcoUnknownPtr_t))EcoLab65_Release;
    impl->refCount = 1;

    impl->allocator = (IEcoMemoryAllocator1*)system;
    impl->allocator->pVTbl->AddRef(impl->allocator);

    int16_t status = ((IEcoInterfaceBus1*)system)->pVTbl->QueryComponent(system, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&impl->calculatorX);
    if (status != 0) {
        goto cleanup;
    }

    status = ((IEcoInterfaceBus1*)system)->pVTbl->QueryComponent(system, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&impl->calculatorY);
    if (status != 0) {
        goto cleanup;
    }

    *out = impl;
    return 0;

cleanup:
    if (impl->calculatorX) {
        impl->calculatorX->pVTbl->Release(impl->calculatorX);
    }
    if (impl->allocator) {
        impl->allocator->pVTbl->Release(impl->allocator);
    }
    lab_allocator(NULL)->pVTbl->Free(lab_allocator(NULL), impl->pVtbl);
    lab_allocator(NULL)->pVTbl->Free(lab_allocator(NULL), impl);
    return status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !iid) {
        return -1;
    }
    *out = NULL;
    if (outer != NULL || (!IsEqualUGUID(iid, &IID_IEcoLab2) && !IsEqualUGUID(iid, &IID_IEcoLab65) && !IsEqualUGUID(iid, &IID_IEcoCalculatorX) && !IsEqualUGUID(iid, &IID_IEcoCalculatorY) && !IsEqualUGUID(iid, &IID_IEcoUnknown))) {
        return -1;
    }

    EcoLab2Impl* impl = (EcoLab2Impl*)lab_allocator(NULL)->pVTbl->Alloc(lab_allocator(NULL), sizeof(EcoLab2Impl));
    if (!impl) {
        return -2;
    }

    impl->pVtbl = (IEcoUnknownVTbl*)lab_allocator(NULL)->pVTbl->Alloc(lab_allocator(NULL), sizeof(IEcoUnknownVTbl));
    if (!impl->pVtbl) {
        lab_allocator(NULL)->pVTbl->Free(lab_allocator(NULL), impl);
        return -2;
    }

    impl->pVtbl->QueryInterface = (int16_t(ECOCALLMETHOD*)(IEcoUnknownPtr_t, const UGUID*, voidptr_t*))EcoLab2_QueryInterface;
    impl->pVtbl->AddRef = (uint32_t(ECOCALLMETHOD*)(IEcoUnknownPtr_t))EcoLab2_AddRef;
    impl->pVtbl->Release = (uint32_t(ECOCALLMETHOD*)(IEcoUnknownPtr_t))EcoLab2_Release;
    impl->refCount = 1;

    impl->allocator = (IEcoMemoryAllocator1*)system;
    impl->allocator->pVTbl->AddRef(impl->allocator);

    int16_t status = ((IEcoInterfaceBus1*)system)->pVTbl->QueryComponent(system, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&impl->lab65);
    if (status != 0) {
        goto cleanup;
    }

    *out = impl;
    return 0;

cleanup:
    if (impl->allocator) {
        impl->allocator->pVTbl->Release(impl->allocator);
    }
    lab_allocator(NULL)->pVTbl->Free(lab_allocator(NULL), impl->pVtbl);
    lab_allocator(NULL)->pVTbl->Free(lab_allocator(NULL), impl);
    return status;
}
