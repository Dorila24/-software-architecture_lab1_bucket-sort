#include "ReuseContract.h"
#include "SdkAbiReference.md"

typedef struct {
    IEcoLab65VTbl* pVTbl;
    IEcoCalculatorXVTbl* pVTblX;
    IEcoCalculatorYVTbl* pVTblY;
    IEcoUnknownVTbl* pVTblND;

    uint32_t refCount;
    IEcoMemoryAllocator1* allocator;
    IEcoSystem1* system;
    IEcoUnknown* m_pIUnkOuter;

    IEcoCalculatorX* pICalculatorX;
    IEcoCalculatorY* pICalculatorY;
} CEcoLab65State;

static int16_t ECOCALLMETHOD CEcoLab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoLab65State* pCMe = (CEcoLab65State*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    *ppv = 0;

    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (voidptr_t)pCMe;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = (voidptr_t)pCMe->pICalculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (voidptr_t)pCMe->pICalculatorY;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)&pCMe->pVTblND;
    } else {
        return ERR_ECO_NOINTERFACE;
    }

    if (*ppv) {
        ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    }

    return 0;
}

static uint32_t ECOCALLMETHOD CEcoLab65_AddRef(IEcoLab65Ptr_t me) {
    CEcoLab65State* pCMe = (CEcoLab65State*)me;

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIUnkOuter ? pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter) : 0;
}

static uint32_t ECOCALLMETHOD CEcoLab65_Release(IEcoLab65Ptr_t me) {
    CEcoLab65State* pCMe = (CEcoLab65State*)me;

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIUnkOuter ? pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter) : 0;
}

static int16_t ECOCALLMETHOD CEcoLab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    CEcoLab65State* pCMe = (CEcoLab65State*)me;
    return lab_sort_int(pCMe, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    CEcoLab65State* pCMe = (CEcoLab65State*)me;
    return lab_sort_long(pCMe, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    CEcoLab65State* pCMe = (CEcoLab65State*)me;
    return lab_sort_float(pCMe, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    CEcoLab65State* pCMe = (CEcoLab65State*)me;
    return lab_sort_double(pCMe, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    CEcoLab65State* pCMe = (CEcoLab65State*)me;
    return lab_sort_long_double(pCMe, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoCalculatorX_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorX));

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    *ppv = 0;

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = (voidptr_t)pCMe->pICalculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)&pCMe->pVTblND;
    } else {
        return ERR_ECO_NOINTERFACE;
    }

    if (*ppv) {
        ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    }

    return 0;
}

static uint32_t ECOCALLMETHOD CEcoCalculatorX_AddRef(IEcoCalculatorXPtr_t me) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorX));

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIUnkOuter ? pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter) : 0;
}

static uint32_t ECOCALLMETHOD CEcoCalculatorX_Release(IEcoCalculatorXPtr_t me) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorX));

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIUnkOuter ? pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter) : 0;
}

static int32_t ECOCALLMETHOD CEcoCalculatorX_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorX));
    return pCMe->pICalculatorX->pVTbl->Addition(pCMe->pICalculatorX, a, b);
}

static int16_t ECOCALLMETHOD CEcoCalculatorX_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorX));
    return pCMe->pICalculatorX->pVTbl->Subtraction(pCMe->pICalculatorX, a, b);
}

static int16_t ECOCALLMETHOD CEcoCalculatorY_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorY));

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    *ppv = 0;

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (voidptr_t)pCMe->pICalculatorY;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)&pCMe->pVTblND;
    } else {
        return ERR_ECO_NOINTERFACE;
    }

    if (*ppv) {
        ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    }

    return 0;
}

static uint32_t ECOCALLMETHOD CEcoCalculatorY_AddRef(IEcoCalculatorYPtr_t me) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorY));

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIUnkOuter ? pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter) : 0;
}

static uint32_t ECOCALLMETHOD CEcoCalculatorY_Release(IEcoCalculatorYPtr_t me) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorY));

    if (me == 0) {
        return -1;
    }

    return pCMe->m_pIUnkOuter ? pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter) : 0;
}

static int32_t ECOCALLMETHOD CEcoCalculatorY_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorY));
    return pCMe->pICalculatorY->pVTbl->Multiplication(pCMe->pICalculatorY, a, b);
}

static int16_t ECOCALLMETHOD CEcoCalculatorY_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pICalculatorY));
    return pCMe->pICalculatorY->pVTbl->Division(pCMe->pICalculatorY, a, b);
}

static int16_t ECOCALLMETHOD CEcoLab65ND_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pVTblND));

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    *ppv = 0;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)&pCMe->pVTblND;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (voidptr_t)pCMe;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = (voidptr_t)pCMe->pICalculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (voidptr_t)pCMe->pICalculatorY;
    } else {
        return ERR_ECO_NOINTERFACE;
    }

    if (*ppv) {
        ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    }

    return 0;
}

static uint32_t ECOCALLMETHOD CEcoLab65ND_AddRef(IEcoUnknownPtr_t me) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pVTblND));

    if (me == 0) {
        return -1;
    }

    return ++pCMe->refCount;
}

static uint32_t ECOCALLMETHOD CEcoLab65ND_Release(IEcoUnknownPtr_t me) {
    CEcoLab65State* pCMe = (CEcoLab65State*)((char*)me - offsetof(CEcoLab65State, pVTblND));

    if (me == 0) {
        return -1;
    }

    if (--pCMe->refCount == 0) {
        IEcoMemoryAllocator1* allocator = pCMe->allocator;

        if (pCMe->pICalculatorX) {
            pCMe->pICalculatorX->pVTbl->Release(pCMe->pICalculatorX);
        }

        if (pCMe->pICalculatorY) {
            pCMe->pICalculatorY->pVTbl->Release(pCMe->pICalculatorY);
        }

        if (pCMe->system) {
            pCMe->system->pVTbl->Release(pCMe->system);
        }

        allocator->pVTbl->Free(allocator, pCMe);

        allocator->pVTbl->Release(allocator);

        return 0;
    }

    return pCMe->refCount;
}

static int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) {
        return ERR_ECO_POINTER;
    }

    *out = 0;

    if (system == 0 || iid == 0) {
        return ERR_ECO_POINTER;
    }

    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOAGGREGATION;
    }

    IEcoInterfaceBus1* bus = 0;
    IEcoMemoryAllocator1* allocator = 0;
    IEcoLab65State* state = 0;
    int16_t status = 0;

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&bus);
    if (status != 0) {
        return status;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != 0) {
        bus->pVTbl->Release(bus);
        return status;
    }

    state = (CEcoLab65State*)allocator->pVTbl->Alloc(allocator, sizeof(CEcoLab65State));
    if (state == 0) {
        allocator->pVTbl->Release(allocator);
        bus->pVTbl->Release(bus);
        return ERR_ECO_OUTOFMEMORY;
    }

    state->pVTbl = (IEcoLab65VTbl*)allocator->pVTbl->Alloc(allocator, sizeof(IEcoLab65VTbl));
    state->pVTblX = (IEcoCalculatorXVTbl*)allocator->pVTbl->Alloc(allocator, sizeof(IEcoCalculatorXVTbl));
    state->pVTblY = (IEcoCalculatorYVTbl*)allocator->pVTbl->Alloc(allocator, sizeof(IEcoCalculatorYVTbl));
    state->pVTblND = (IEcoUnknownVTbl*)allocator->pVTbl->Alloc(allocator, sizeof(IEcoUnknownVTbl));

    if (state->pVTbl == 0 || state->pVTblX == 0 || state->pVTblY == 0 || state->pVTblND == 0) {
        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        bus->pVTbl->Release(bus);
        return ERR_ECO_OUTOFMEMORY;
    }

    state->pVTbl->QueryInterface = CEcoLab65_QueryInterface;
    state->pVTbl->AddRef = CEcoLab65_AddRef;
    state->pVTbl->Release = CEcoLab65_Release;
    state->pVTbl->SortInt = CEcoLab65_SortInt;
    state->pVTbl->SortLong = CEcoLab65_SortLong;
    state->pVTbl->SortFloat = CEcoLab65_SortFloat;
    state->pVTbl->SortDouble = CEcoLab65_SortDouble;
    state->pVTbl->SortLongDouble = CEcoLab65_SortLongDouble;

    state->pVTblX->QueryInterface = CEcoCalculatorX_QueryInterface;
    state->pVTblX->AddRef = CEcoCalculatorX_AddRef;
    state->pVTblX->Release = CEcoCalculatorX_Release;
    state->pVTblX->Addition = CEcoCalculatorX_Addition;
    state->pVTblX->Subtraction = CEcoCalculatorX_Subtraction;

    state->pVTblY->QueryInterface = CEcoCalculatorY_QueryInterface;
    state->pVTblY->AddRef = CEcoCalculatorY_AddRef;
    state->pVTblY->Release = CEcoCalculatorY_Release;
    state->pVTblY->Multiplication = CEcoCalculatorY_Multiplication;
    state->pVTblY->Division = CEcoCalculatorY_Division;

    state->pVTblND->QueryInterface = CEcoLab65ND_QueryInterface;
    state->pVTblND->AddRef = CEcoLab65ND_AddRef;
    state->pVTblND->Release = CEcoLab65ND_Release;

    state->refCount = 1;
    state->allocator = allocator;
    state->system = system;
    state->m_pIUnkOuter = outer;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&state->pICalculatorX);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->pICalculatorY);
    if (status != 0) {
        goto cleanup;
    }

    if (outer == 0) {
        state->m_pIUnkOuter = (IEcoUnknown*)&state->pVTblND;
    }

    status = state->pVTblND->QueryInterface((IEcoUnknown*)state, iid, out);
    if (status != 0) {
        goto cleanup;
    }

    state->pVTblND->Release((IEcoUnknown*)state);

    bus->pVTbl->Release(bus);
    return 0;

cleanup:
    if (state->pICalculatorX) {
        state->pICalculatorX->pVTbl->Release(state->pICalculatorX);
    }

    if (state->pICalculatorY) {
        state->pICalculatorY->pVTbl->Release(state->pICalculatorY);
    }

    if (state->system) {
        state->system->pVTbl->Release(state->system);
    }

    allocator->pVTbl->Free(allocator, state);

    allocator->pVTbl->Release(allocator);
    bus->pVTbl->Release(bus);
    return status;
}

static int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) {
        return ERR_ECO_POINTER;
    }

    *out = 0;

    if (system == 0 || iid == 0) {
        return ERR_ECO_POINTER;
    }

    if (outer != 0) {
        return ERR_ECO_NOAGGREGATION;
    }

    // Temporary error stub for Lab2
    return ERR_ECO_NOINTERFACE;
}
