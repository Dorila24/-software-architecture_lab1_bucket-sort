#include "ReuseContract.h"

/* Forward declarations */
static int16_t Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t Lab65_AddRef(IEcoLab65Ptr_t me);
static uint32_t Lab65_Release(IEcoLab65Ptr_t me);
static int16_t Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets);
static int16_t Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets);
static int16_t Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets);
static int16_t Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets);
static int16_t Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets);

static int16_t X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t X_AddRef(IEcoCalculatorXPtr_t me);
static uint32_t X_Release(IEcoCalculatorXPtr_t me);
static int32_t X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);
static int16_t X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);

static int16_t Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t Y_AddRef(IEcoCalculatorYPtr_t me);
static uint32_t Y_Release(IEcoCalculatorYPtr_t me);
static int32_t Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);
static int16_t Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);

static int16_t Unknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t Unknown_AddRef(IEcoUnknownPtr_t me);
static uint32_t Unknown_Release(IEcoUnknownPtr_t me);

/* Static vtables */
static IEcoLab65VTbl Lab65Vtbl = {
    Lab65_QueryInterface,
    Lab65_AddRef,
    Lab65_Release,
    Lab65_SortInt,
    Lab65_SortLong,
    Lab65_SortFloat,
    Lab65_SortDouble,
    Lab65_SortLongDouble
};

static IEcoCalculatorXVTbl XVtbl = {
    X_QueryInterface,
    X_AddRef,
    X_Release,
    X_Addition,
    X_Subtraction
};

static IEcoCalculatorYVTbl YVtbl = {
    Y_QueryInterface,
    Y_AddRef,
    Y_Release,
    Y_Multiplication,
    Y_Division
};

static IEcoUnknownVTbl UnknownVtbl = {
    Unknown_QueryInterface,
    Unknown_AddRef,
    Unknown_Release
};

/* Lab65 state */
typedef struct {
    IEcoLab65VTbl* pVTbl;
    IEcoCalculatorXVTbl* pXVtbl;
    IEcoCalculatorYVTbl* pYVtbl;
    IEcoUnknownVTbl* pUnknownVtbl;
    uint32_t refcount;
    IEcoUnknownPtr_t controller;
    IEcoMemoryAllocator1Ptr_t allocator;
    IEcoInterfaceBus1Ptr_t bus;
    IEcoCalculatorXPtr_t X;
    IEcoCalculatorYPtr_t Y;
} Lab65State;

/* Private functions */
static int16_t Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return Unknown_QueryInterface((IEcoUnknownPtr_t)state, riid, ppv);
}

static uint32_t Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return Unknown_AddRef((IEcoUnknownPtr_t)state);
}

static uint32_t Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return Unknown_Release((IEcoUnknownPtr_t)state);
}

static int16_t Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return lab_sort_int(state->controller, data, count, buckets);
}

static int16_t Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return lab_sort_long(state->controller, data, count, buckets);
}

static int16_t Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return lab_sort_float(state->controller, data, count, buckets);
}

static int16_t Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return lab_sort_double(state->controller, data, count, buckets);
}

static int16_t Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return lab_sort_long_double(state->controller, data, count, buckets);
}

static int16_t X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pXVtbl));
    return Unknown_QueryInterface((IEcoUnknownPtr_t)state, riid, ppv);
}

static uint32_t X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pXVtbl));
    return Unknown_AddRef((IEcoUnknownPtr_t)state);
}

static uint32_t X_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pXVtbl));
    return Unknown_Release((IEcoUnknownPtr_t)state);
}

static int32_t X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pXVtbl));
    return state->X->pVTbl->Addition(state->X, a, b);
}

static int16_t X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pXVtbl));
    return state->X->pVTbl->Subtraction(state->X, a, b);
}

static int16_t Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pYVtbl));
    return Unknown_QueryInterface((IEcoUnknownPtr_t)state, riid, ppv);
}

static uint32_t Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pYVtbl));
    return Unknown_AddRef((IEcoUnknownPtr_t)state);
}

static uint32_t Y_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pYVtbl));
    return Unknown_Release((IEcoUnknownPtr_t)state);
}

static int32_t Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pYVtbl));
    return state->Y->pVTbl->Multiplication(state->Y, a, b);
}

static int16_t Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pYVtbl));
    return state->Y->pVTbl->Division(state->Y, a, b);
}

static int16_t Unknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pUnknownVtbl));
    if (ppv == NULL) {
        return ERR_ECO_POINTER;
    }
    *ppv = NULL;

    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (voidptr_t)&state->pVTbl;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = (voidptr_t)&state->pXVtbl;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (voidptr_t)&state->pYVtbl;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)&state->pUnknownVtbl;
    } else {
        return ERR_ECO_NOINTERFACE;
    }

    Unknown_AddRef((IEcoUnknownPtr_t)state);
    return ERR_ECO_SUCCESS;
}

static uint32_t Unknown_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pUnknownVtbl));
    return ++state->refcount;
}

static uint32_t Unknown_Release(IEcoUnknownPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pUnknownVtbl));
    if (--state->refcount == 0) {
        state->allocator->pVTbl->Free(state->allocator, state);
        return 0;
    }
    return state->refcount;
}

/* Exported functions */
int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == NULL || system == NULL || iid == NULL) {
        return ERR_ECO_POINTER;
    }
    *out = NULL;

    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOAGGREGATION;
    }

    IEcoInterfaceBus1Ptr_t bus = NULL;
    IEcoMemoryAllocator1Ptr_t allocator = NULL;
    IEcoCalculatorXPtr_t X = NULL;
    IEcoCalculatorYPtr_t Y = NULL;
    int16_t status = ERR_ECO_SUCCESS;

    do {
        IEcoSystem1Ptr_t ecoSystem = NULL;
        status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&ecoSystem);
        if (status != ERR_ECO_SUCCESS) {
            break;
        }

        status = ecoSystem->pVTbl->QueryInterface(ecoSystem, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
        if (status != ERR_ECO_SUCCESS) {
            break;
        }

        UGUID managerCID = CID_EcoMemoryManager1;
        IEcoInterfaceBus1MemExtPtr_t memExt = NULL;
        status = bus->pVTbl->QueryComponent(bus, &CID_EcoInterfaceBus1MemExt, NULL, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&memExt);
        if (status == ERR_ECO_SUCCESS) {
            UGUID tempManagerCID;
            status = memExt->pVTbl->get_Manager(memExt, &tempManagerCID);
            if (status == ERR_ECO_SUCCESS) {
                managerCID = tempManagerCID;
            }
            memExt->pVTbl->Release(memExt);
        }

        status = bus->pVTbl->QueryComponent(bus, &managerCID, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
        if (status != ERR_ECO_SUCCESS) {
            break;
        }

        Lab65State* state = (Lab65State*)allocator->pVTbl->Alloc(allocator, sizeof(Lab65State));
        if (state == NULL) {
            status = ERR_ECO_OUTOFMEMORY;
            break;
        }

        state->pVTbl = &Lab65Vtbl;
        state->pXVtbl = &XVtbl;
        state->pYVtbl = &YVtbl;
        state->pUnknownVtbl = &UnknownVtbl;
        state->refcount = 1;
        state->controller = (outer != NULL) ? outer : (IEcoUnknownPtr_t)state;
        state->allocator = allocator;
        state->bus = bus;

        status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&X);
        if (status != ERR_ECO_SUCCESS) {
            break;
        }

        status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&Y);
        if (status != ERR_ECO_SUCCESS) {
            break;
        }

        state->X = X;
        state->Y = Y;

        bus->pVTbl->Release(bus);
        bus = NULL;
        allocator->pVTbl->Release(allocator);
        allocator = NULL;

        if (outer == NULL) {
            status = Unknown_QueryInterface((IEcoUnknownPtr_t)state, iid, out);
            if (status != ERR_ECO_SUCCESS) {
                break;
            }
        } else {
            *out = (voidptr_t)state;
        }
    } while (0);

    if (status != ERR_ECO_SUCCESS) {
        if (X != NULL) {
            X->pVTbl->Release(X);
        }
        if (Y != NULL) {
            Y->pVTbl->Release(Y);
        }
        if (allocator != NULL) {
            allocator->pVTbl->Free(allocator, state);
        }
        if (bus != NULL) {
            bus->pVTbl->Release(bus);
        }
    }

    return status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == NULL || system == NULL || iid == NULL) {
        return ERR_ECO_POINTER;
    }
    *out = NULL;

    if (outer != NULL) {
        return ERR_ECO_NOAGGREGATION;
    }

    // Implement Lab2 creation here
    return ERR_ECO_SUCCESS;
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    if (me == NULL) {
        return NULL;
    }
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, pVTbl));
    return state->allocator;
}
