#include "ReuseContract.h"

typedef struct {
    IEcoLab65 IEcoLab65;
    IEcoCalculatorX IEcoCalculatorX;
    IEcoCalculatorY IEcoCalculatorY;
    IEcoUnknown privateNDUnknown;
    uint32_t refCount;
    IEcoMemoryAllocator1* allocator;
    IEcoSystem1* system;
    IEcoInterfaceBus1* bus;
    IEcoCalculatorX* ownedAX;
    IEcoCalculatorY* ownedDY;
} Lab65State;

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoLab65));
    if (ppv == 0) {
        return ERR_ECO_POINTER;
    }
    *ppv = 0;
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &pState->IEcoLab65;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pState->IEcoCalculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pState->IEcoCalculatorY;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pState->privateNDUnknown;
    } else {
        return ERR_ECO_NOINTERFACE;
    }
    ((IEcoUnknownPtr_t)*ppv)->pVTbl->AddRef((IEcoUnknownPtr_t)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoLab65));
    if (me == 0) {
        return 0;
    }
    return pState->privateNDUnknown.pVTbl->AddRef(&pState->privateNDUnknown);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoLab65));
    if (me == 0) {
        return 0;
    }
    return pState->privateNDUnknown.pVTbl->Release(&pState->privateNDUnknown);
}

static int16_t ECOCALLMETHOD Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD CalculatorX_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorX));
    if (ppv == 0) {
        return ERR_ECO_POINTER;
    }
    *ppv = 0;
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pState->IEcoCalculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pState->privateNDUnknown;
    } else {
        return ERR_ECO_NOINTERFACE;
    }
    ((IEcoUnknownPtr_t)*ppv)->pVTbl->AddRef((IEcoUnknownPtr_t)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD CalculatorX_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorX));
    if (me == 0) {
        return 0;
    }
    return pState->privateNDUnknown.pVTbl->AddRef(&pState->privateNDUnknown);
}

static uint32_t ECOCALLMETHOD CalculatorX_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorX));
    if (me == 0) {
        return 0;
    }
    return pState->privateNDUnknown.pVTbl->Release(&pState->privateNDUnknown);
}

static int32_t ECOCALLMETHOD CalculatorX_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorX));
    return pState->ownedAX->pVTbl->Addition(pState->ownedAX, a, b);
}

static int16_t ECOCALLMETHOD CalculatorX_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorX));
    return pState->ownedAX->pVTbl->Subtraction(pState->ownedAX, a, b);
}

static int16_t ECOCALLMETHOD CalculatorY_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorY));
    if (ppv == 0) {
        return ERR_ECO_POINTER;
    }
    *ppv = 0;
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pState->IEcoCalculatorY;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pState->privateNDUnknown;
    } else {
        return ERR_ECO_NOINTERFACE;
    }
    ((IEcoUnknownPtr_t)*ppv)->pVTbl->AddRef((IEcoUnknownPtr_t)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD CalculatorY_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorY));
    if (me == 0) {
        return 0;
    }
    return pState->privateNDUnknown.pVTbl->AddRef(&pState->privateNDUnknown);
}

static uint32_t ECOCALLMETHOD CalculatorY_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorY));
    if (me == 0) {
        return 0;
    }
    return pState->privateNDUnknown.pVTbl->Release(&pState->privateNDUnknown);
}

static int32_t ECOCALLMETHOD CalculatorY_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorY));
    return pState->ownedDY->pVTbl->Multiplication(pState->ownedDY, a, b);
}

static int16_t ECOCALLMETHOD CalculatorY_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoCalculatorY));
    return pState->ownedDY->pVTbl->Division(pState->ownedDY, a, b);
}

static int16_t ECOCALLMETHOD NDUnknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, privateNDUnknown));
    if (ppv == 0) {
        return ERR_ECO_POINTER;
    }
    *ppv = 0;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pState->privateNDUnknown;
        pState->refCount++;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &pState->IEcoLab65;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pState->IEcoCalculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pState->IEcoCalculatorY;
    } else {
        return ERR_ECO_NOINTERFACE;
    }
    ((IEcoUnknownPtr_t)*ppv)->pVTbl->AddRef((IEcoUnknownPtr_t)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD NDUnknown_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, privateNDUnknown));
    if (me == 0) {
        return 0;
    }
    pState->refCount++;
    return pState->refCount;
}

static uint32_t ECOCALLMETHOD NDUnknown_Release(IEcoUnknownPtr_t me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, privateNDUnknown));
    if (me == 0) {
        return 0;
    }
    pState->refCount--;
    if (pState->refCount == 0) {
        if (pState->ownedAX) {
            pState->ownedAX->pVTbl->Release(pState->ownedAX);
        }
        if (pState->ownedDY) {
            pState->ownedDY->pVTbl->Release(pState->ownedDY);
        }
        if (pState->allocator) {
            pState->allocator->pVTbl->Free(pState->allocator, pState);
        }
        return 0;
    }
    return pState->refCount;
}

static IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65State* pState = (Lab65State*)((char*)me - offsetof(Lab65State, IEcoLab65));
    if (me == 0) {
        return 0;
    }
    return pState->allocator;
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
        return ERR_ECO_NOINTERFACE;
    }

    IEcoSystem1* ecoSystem = 0;
    int16_t result = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&ecoSystem);
    if (result != 0) {
        return result;
    }

    IEcoInterfaceBus1* ecoBus = 0;
    result = ecoSystem->pVTbl->QueryInterface(ecoSystem, &IID_IEcoInterfaceBus1, (voidptr_t*)&ecoBus);
    if (result != 0) {
        ecoSystem->pVTbl->Release(ecoSystem);
        return result;
    }

    IEcoMemoryAllocator1* allocator = 0;
    result = ecoBus->pVTbl->QueryComponent(ecoBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (result != 0) {
        ecoBus->pVTbl->Release(ecoBus);
        ecoSystem->pVTbl->Release(ecoSystem);
        return result;
    }

    Lab65State* state = (Lab65State*)allocator->pVTbl->Alloc(allocator, sizeof(Lab65State));
    if (state == 0) {
        allocator->pVTbl->Release(allocator);
        ecoBus->pVTbl->Release(ecoBus);
        ecoSystem->pVTbl->Release(ecoSystem);
        return ERR_ECO_ALLOCATOR_FAILURE;
    }

    state->refCount = 1;
    state->allocator = allocator;
    state->system = ecoSystem;
    state->bus = ecoBus;
    state->ownedAX = 0;
    state->ownedDY = 0;

    IEcoInterfaceBus1MemExt* busExt = 0;
    result = ecoBus->pVTbl->QueryInterface(ecoBus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&busExt);
    if (result == 0) {
        const UGUID* managerCID = busExt->pVTbl->get_Manager(busExt);
        if (managerCID != 0) {
            state->allocator = 0;
            result = ecoBus->pVTbl->QueryComponent(ecoBus, managerCID, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&state->allocator);
            if (result != 0) {
                busExt->pVTbl->Release(busExt);
                allocator->pVTbl->Free(allocator, state);
                ecoBus->pVTbl->Release(ecoBus);
                ecoSystem->pVTbl->Release(ecoSystem);
                return result;
            }
        }
        busExt->pVTbl->Release(busExt);
    }

    result = ecoBus->pVTbl->QueryComponent(ecoBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&state->ownedAX);
    if (result != 0) {
        allocator->pVTbl->Free(allocator, state);
        ecoBus->pVTbl->Release(ecoBus);
        ecoSystem->pVTbl->Release(ecoSystem);
        return result;
    }

    result = ecoBus->pVTbl->QueryComponent(ecoBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->ownedDY);
    if (result != 0) {
        state->ownedAX->pVTbl->Release(state->ownedAX);
        allocator->pVTbl->Free(allocator, state);
        ecoBus->pVTbl->Release(ecoBus);
        ecoSystem->pVTbl->Release(ecoSystem);
        return result;
    }

    state->IEcoLab65.pVTbl = (IEcoLab65VTbl*)malloc(sizeof(IEcoLab65VTbl));
    state->IEcoCalculatorX.pVTbl = (IEcoCalculatorXVTbl*)malloc(sizeof(IEcoCalculatorXVTbl));
    state->IEcoCalculatorY.pVTbl = (IEcoCalculatorYVTbl*)malloc(sizeof(IEcoCalculatorYVTbl));
    state->privateNDUnknown.pVTbl = (IEcoUnknownVTbl*)malloc(sizeof(IEcoUnknownVTbl));

    state->IEcoLab65.pVTbl->QueryInterface = Lab65_QueryInterface;
    state->IEcoLab65.pVTbl->AddRef = Lab65_AddRef;
    state->IEcoLab65.pVTbl->Release = Lab65_Release;
    state->IEcoLab65.pVTbl->SortInt = Lab65_SortInt;
    state->IEcoLab65.pVTbl->SortLong = Lab65_SortLong;
    state->IEcoLab65.pVTbl->SortFloat = Lab65_SortFloat;
    state->IEcoLab65.pVTbl->SortDouble = Lab65_SortDouble;
    state->IEcoLab65.pVTbl->SortLongDouble = Lab65_SortLongDouble;

    state->IEcoCalculatorX.pVTbl->QueryInterface = CalculatorX_QueryInterface;
    state->IEcoCalculatorX.pVTbl->AddRef = CalculatorX_AddRef;
    state->IEcoCalculatorX.pVTbl->Release = CalculatorX_Release;
    state->IEcoCalculatorX.pVTbl->Addition = CalculatorX_Addition;
    state->IEcoCalculatorX.pVTbl->Subtraction = CalculatorX_Subtraction;

    state->IEcoCalculatorY.pVTbl->QueryInterface = CalculatorY_QueryInterface;
    state->IEcoCalculatorY.pVTbl->AddRef = CalculatorY_AddRef;
    state->IEcoCalculatorY.pVTbl->Release = CalculatorY_Release;
    state->IEcoCalculatorY.pVTbl->Multiplication = CalculatorY_Multiplication;
    state->IEcoCalculatorY.pVTbl->Division = CalculatorY_Division;

    state->privateNDUnknown.pVTbl->QueryInterface = NDUnknown_QueryInterface;
    state->privateNDUnknown.pVTbl->AddRef = NDUnknown_AddRef;
    state->privateNDUnknown.pVTbl->Release = NDUnknown_Release;

    if (outer == 0) {
        *out = &state->privateNDUnknown;
        return 0;
    }

    IEcoUnknown* controllingUnknown = outer;
    result = controllingUnknown->pVTbl->QueryInterface(controllingUnknown, &IID_IEcoUnknown, (voidptr_t*)out);
    if (result != 0) {
        state->privateNDUnknown.pVTbl->Release(&state->privateNDUnknown);
        return result;
    }

    return 0;
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
    return ERR_ECO_NOINTERFACE;
}
