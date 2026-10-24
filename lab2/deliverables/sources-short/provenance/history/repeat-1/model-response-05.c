#include "ReuseContract.h"
#include "SdkAbiReference.md"
#include "IEcoLab65.h"
#include "IdEcoLab65.h"
#include "IEcoCalculatorX.h"
#include "IdEcoCalculatorA.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorD.h"

typedef struct {
    IEcoUnknown unknown;
    uint32_t refCount;
    IEcoLab65 lab65;
    IEcoCalculatorX calculatorX;
    IEcoCalculatorY calculatorY;
    IEcoUnknown* controllingUnknown;
    IEcoSystem1* system;
    IEcoMemoryAllocator1* allocator;
    IEcoCalculatorX* ownedCalculatorX;
    IEcoCalculatorY* ownedCalculatorY;
} Lab65State;

static const UGUID CID_EcoMemoryManager1 = {0x01, 0x10, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0x01, 0x02}};

static Lab65State* getLab65StateFromUnknown(IEcoUnknown* unknown) {
    return (Lab65State*)((char*)unknown - offsetof(Lab65State, unknown));
}

static Lab65State* getLab65StateFromCalculatorX(IEcoCalculatorX* calculatorX) {
    return (Lab65State*)((char*)calculatorX - offsetof(Lab65State, calculatorX));
}

static Lab65State* getLab65StateFromCalculatorY(IEcoCalculatorY* calculatorY) {
    return (Lab65State*)((char*)calculatorY - offsetof(Lab65State, calculatorY));
}

static int16_t ECOCALLMETHOD lab65_QueryInterface(IEcoLab65* me, const UGUID* riid, void** ppv) {
    Lab65State* state = getLab65StateFromUnknown((IEcoUnknown*)me);
    if (!ppv) return ERR_ECO_NOINTERFACE;
    *ppv = NULL;
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &state->lab65;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->calculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->calculatorY;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->unknown;
    } else {
        return state->controllingUnknown->pVTbl->QueryInterface(state->controllingUnknown, riid, ppv);
    }
    ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD lab65_AddRef(IEcoLab65* me) {
    Lab65State* state = getLab65StateFromUnknown((IEcoUnknown*)me);
    return ++state->refCount;
}

static uint32_t ECOCALLMETHOD lab65_Release(IEcoLab65* me) {
    Lab65State* state = getLab65StateFromUnknown((IEcoUnknown*)me);
    if (--state->refCount == 0) {
        IEcoMemoryAllocator1* allocator = state->allocator;
        if (state->ownedCalculatorX) state->ownedCalculatorX->pVTbl->Release(state->ownedCalculatorX);
        if (state->ownedCalculatorY) state->ownedCalculatorY->pVTbl->Release(state->ownedCalculatorY);
        if (state->system) state->system->pVTbl->Release(state->system);
        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return state->refCount;
}

static int16_t ECOCALLMETHOD lab65_SortInt(IEcoLab65* me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_SortLong(IEcoLab65* me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_SortFloat(IEcoLab65* me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_SortDouble(IEcoLab65* me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_SortLongDouble(IEcoLab65* me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_unknown_QueryInterface(IEcoUnknown* me, const UGUID* riid, void** ppv) {
    Lab65State* state = getLab65StateFromUnknown(me);
    if (!ppv) return ERR_ECO_NOINTERFACE;
    *ppv = NULL;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->unknown;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &state->lab65;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->calculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->calculatorY;
    } else {
        return ERR_ECO_NOINTERFACE;
    }
    ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD lab65_unknown_AddRef(IEcoUnknown* me) {
    Lab65State* state = getLab65StateFromUnknown(me);
    return ++state->refCount;
}

static uint32_t ECOCALLMETHOD lab65_unknown_Release(IEcoUnknown* me) {
    Lab65State* state = getLab65StateFromUnknown(me);
    if (--state->refCount == 0) {
        IEcoMemoryAllocator1* allocator = state->allocator;
        if (state->ownedCalculatorX) state->ownedCalculatorX->pVTbl->Release(state->ownedCalculatorX);
        if (state->ownedCalculatorY) state->ownedCalculatorY->pVTbl->Release(state->ownedCalculatorY);
        if (state->system) state->system->pVTbl->Release(state->system);
        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return state->refCount;
}

static int16_t ECOCALLMETHOD lab65_calculatorX_QueryInterface(IEcoCalculatorX* me, const UGUID* riid, void** ppv) {
    Lab65State* state = getLab65StateFromCalculatorX(me);
    if (!ppv) return ERR_ECO_NOINTERFACE;
    *ppv = NULL;
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->calculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->unknown;
    } else {
        return state->controllingUnknown->pVTbl->QueryInterface(state->controllingUnknown, riid, ppv);
    }
    ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD lab65_calculatorX_AddRef(IEcoCalculatorX* me) {
    Lab65State* state = getLab65StateFromCalculatorX(me);
    return ++state->refCount;
}

static uint32_t ECOCALLMETHOD lab65_calculatorX_Release(IEcoCalculatorX* me) {
    Lab65State* state = getLab65StateFromCalculatorX(me);
    if (--state->refCount == 0) {
        IEcoMemoryAllocator1* allocator = state->allocator;
        if (state->ownedCalculatorX) state->ownedCalculatorX->pVTbl->Release(state->ownedCalculatorX);
        if (state->ownedCalculatorY) state->ownedCalculatorY->pVTbl->Release(state->ownedCalculatorY);
        if (state->system) state->system->pVTbl->Release(state->system);
        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return state->refCount;
}

static int32_t ECOCALLMETHOD lab65_calculatorX_Addition(IEcoCalculatorX* me, int16_t a, int16_t b) {
    Lab65State* state = getLab65StateFromCalculatorX(me);
    return state->ownedCalculatorX->pVTbl->Addition(state->ownedCalculatorX, a, b);
}

static int16_t ECOCALLMETHOD lab65_calculatorX_Subtraction(IEcoCalculatorX* me, int16_t a, int16_t b) {
    Lab65State* state = getLab65StateFromCalculatorX(me);
    return state->ownedCalculatorX->pVTbl->Subtraction(state->ownedCalculatorX, a, b);
}

static int16_t ECOCALLMETHOD lab65_calculatorY_QueryInterface(IEcoCalculatorY* me, const UGUID* riid, void** ppv) {
    Lab65State* state = getLab65StateFromCalculatorY(me);
    if (!ppv) return ERR_ECO_NOINTERFACE;
    *ppv = NULL;
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->calculatorY;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->unknown;
    } else {
        return state->controllingUnknown->pVTbl->QueryInterface(state->controllingUnknown, riid, ppv);
    }
    ((IEcoUnknown*)*ppv)->pVTbl->AddRef((IEcoUnknown*)*ppv);
    return 0;
}

static uint32_t ECOCALLMETHOD lab65_calculatorY_AddRef(IEcoCalculatorY* me) {
    Lab65State* state = getLab65StateFromCalculatorY(me);
    return ++state->refCount;
}

static uint32_t ECOCALLMETHOD lab65_calculatorY_Release(IEcoCalculatorY* me) {
    Lab65State* state = getLab65StateFromCalculatorY(me);
    if (--state->refCount == 0) {
        IEcoMemoryAllocator1* allocator = state->allocator;
        if (state->ownedCalculatorX) state->ownedCalculatorX->pVTbl->Release(state->ownedCalculatorX);
        if (state->ownedCalculatorY) state->ownedCalculatorY->pVTbl->Release(state->ownedCalculatorY);
        if (state->system) state->system->pVTbl->Release(state->system);
        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return state->refCount;
}

static int32_t ECOCALLMETHOD lab65_calculatorY_Multiplication(IEcoCalculatorY* me, int16_t a, int16_t b) {
    Lab65State* state = getLab65StateFromCalculatorY(me);
    return state->ownedCalculatorY->pVTbl->Multiplication(state->ownedCalculatorY, a, b);
}

static int16_t ECOCALLMETHOD lab65_calculatorY_Division(IEcoCalculatorY* me, int16_t a, int16_t b) {
    Lab65State* state = getLab65StateFromCalculatorY(me);
    return state->ownedCalculatorY->pVTbl->Division(state->ownedCalculatorY, a, b);
}

IEcoLab65VTbl lab65Vtbl = {
    lab65_QueryInterface,
    lab65_AddRef,
    lab65_Release,
    lab65_SortInt,
    lab65_SortLong,
    lab65_SortFloat,
    lab65_SortDouble,
    lab65_SortLongDouble
};

IEcoUnknownVTbl lab65UnknownVtbl = {
    lab65_unknown_QueryInterface,
    lab65_unknown_AddRef,
    lab65_unknown_Release
};

IEcoCalculatorXVtbl lab65CalculatorXVtbl = {
    lab65_calculatorX_QueryInterface,
    lab65_calculatorX_AddRef,
    lab65_calculatorX_Release,
    lab65_calculatorX_Addition,
    lab65_calculatorX_Subtraction
};

IEcoCalculatorYVtbl lab65CalculatorYVtbl = {
    lab65_calculatorY_QueryInterface,
    lab65_calculatorY_AddRef,
    lab65_calculatorY_Release,
    lab65_calculatorY_Multiplication,
    lab65_calculatorY_Division
};

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65State* state = getLab65StateFromUnknown((IEcoUnknown*)me);
    return state->allocator;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out) return ERR_ECO_NOINTERFACE;
    *out = NULL;
    if (!system || !iid) return ERR_ECO_NOINTERFACE;

    IEcoSystem1* ecoSystem = NULL;
    int16_t status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (void**)&ecoSystem);
    if (status != 0) return status;

    IEcoInterfaceBus1* bus = NULL;
    status = ecoSystem->pVTbl->QueryInterface(ecoSystem, &IID_IEcoInterfaceBus1, (void**)&bus);
    if (status != 0) {
        ecoSystem->pVTbl->Release(ecoSystem);
        return status;
    }

    IEcoInterfaceBus1MemExt* busExt = NULL;
    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (void**)&busExt);
    UGUID managerCID = CID_EcoMemoryManager1;
    if (status == 0) {
        UGUID* extManagerCID = busExt->pVTbl->get_Manager(busExt);
        if (extManagerCID) managerCID = *extManagerCID;
        busExt->pVTbl->Release(busExt);
    }

    IEcoMemoryAllocator1* allocator = NULL;
    status = bus->pVTbl->QueryComponent(bus, &managerCID, NULL, &IID_IEcoMemoryAllocator1, (void**)&allocator);
    if (status != 0) {
        bus->pVTbl->Release(bus);
        ecoSystem->pVTbl->Release(ecoSystem);
        return status;
    }

    Lab65State* state = (Lab65State*)allocator->pVTbl->Alloc(allocator, sizeof(Lab65State));
    if (!state) {
        allocator->pVTbl->Release(allocator);
        bus->pVTbl->Release(bus);
        ecoSystem->pVTbl->Release(ecoSystem);
        return ERR_ECO_OUTOFMEMORY;
    }

    state->unknown.pVTbl = &lab65UnknownVtbl;
    state->lab65.pVTbl = &lab65Vtbl;
    state->calculatorX.pVTbl = &lab65CalculatorXVtbl;
    state->calculatorY.pVTbl = &lab65CalculatorYVtbl;
    state->refCount = 1;
    state->controllingUnknown = outer ? outer : &state->unknown;
    state->system = ecoSystem;
    state->allocator = allocator;
    state->ownedCalculatorX = NULL;
    state->ownedCalculatorY = NULL;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (void**)&state->ownedCalculatorX);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (void**)&state->ownedCalculatorY);
    if (status != 0) {
        goto cleanup;
    }

    status = state->controllingUnknown->pVTbl->QueryInterface(state->controllingUnknown, iid, out);
    if (status != 0) {
        goto cleanup;
    }

    bus->pVTbl->Release(bus);
    return 0;

cleanup:
    if (state->ownedCalculatorX) state->ownedCalculatorX->pVTbl->Release(state->ownedCalculatorX);
    if (state->ownedCalculatorY) state->ownedCalculatorY->pVTbl->Release(state->ownedCalculatorY);
    if (state->system) state->system->pVTbl->Release(state->system);
    allocator->pVTbl->Free(allocator, state);
    allocator->pVTbl->Release(allocator);
    bus->pVTbl->Release(bus);
    ecoSystem->pVTbl->Release(ecoSystem);
    return status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out) return ERR_ECO_NOINTERFACE;
    *out = NULL;
    if (!system || !iid) return ERR_ECO_NOINTERFACE;
    if (outer) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}
