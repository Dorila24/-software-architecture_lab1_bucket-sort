#include "ReuseContract.h"

typedef struct Lab65State {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
    uint32_t local_refs;
    IEcoUnknown* controller;
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
} Lab65State;

/* Private unknown methods */
static int16_t ECOCALLMETHOD Lab65Private_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    int16_t result = ERR_ECO_NOINTERFACE;
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->private_unknown;
        pCMe->local_refs++;
        result = ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &pCMe->public_lab65;
        pCMe->public_lab65.pVTbl->AddRef(&pCMe->public_lab65);
        result = ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->public_x;
        pCMe->public_x.pVTbl->AddRef(&pCMe->public_x);
        result = ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->public_y;
        pCMe->public_y.pVTbl->AddRef(&pCMe->public_y);
        result = ERR_ECO_SUCCESS;
    }
    return result;
}

static uint32_t ECOCALLMETHOD Lab65Private_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (me == 0) return 0;
    pCMe->local_refs++;
    return pCMe->local_refs;
}

static uint32_t ECOCALLMETHOD Lab65Private_Release(IEcoUnknownPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (me == 0) return 0;
    pCMe->local_refs--;
    if (pCMe->local_refs == 0) {
        IEcoMemoryAllocator1* saved_allocator = pCMe->owned_allocator;
        if (pCMe->inner_x != 0) {
            pCMe->inner_x->pVTbl->Release(pCMe->inner_x);
        }
        if (pCMe->inner_y != 0) {
            pCMe->inner_y->pVTbl->Release(pCMe->inner_y);
        }
        if (pCMe->owned_system != 0) {
            pCMe->owned_system->pVTbl->Release(pCMe->owned_system);
        }
        pCMe->owned_allocator->pVTbl->Free(pCMe->owned_allocator, pCMe);
        saved_allocator->pVTbl->Release(saved_allocator);
        return 0;
    }
    return pCMe->local_refs;
}

/* Public Lab65 methods */
static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
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

/* Public X methods */
static int16_t ECOCALLMETHOD Lab65X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65X_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
}

static int32_t ECOCALLMETHOD Lab65X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (me == 0) return 0;
    return pCMe->inner_x->pVTbl->Addition(pCMe->inner_x, a, b);
}

static int16_t ECOCALLMETHOD Lab65X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (me == 0) return 0;
    return pCMe->inner_x->pVTbl->Subtraction(pCMe->inner_x, a, b);
}

/* Public Y methods */
static int16_t ECOCALLMETHOD Lab65Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65Y_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
}

static int32_t ECOCALLMETHOD Lab65Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (me == 0) return 0;
    return pCMe->inner_y->pVTbl->Multiplication(pCMe->inner_y, a, b);
}

static int16_t ECOCALLMETHOD Lab65Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (me == 0) return 0;
    return pCMe->inner_y->pVTbl->Division(pCMe->inner_y, a, b);
}

/* Static tables */
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

static IEcoCalculatorXVTbl Lab65XVtbl = {
    Lab65X_QueryInterface,
    Lab65X_AddRef,
    Lab65X_Release,
    Lab65X_Addition,
    Lab65X_Subtraction
};

static IEcoCalculatorYVTbl Lab65YVtbl = {
    Lab65Y_QueryInterface,
    Lab65Y_AddRef,
    Lab65Y_Release,
    Lab65Y_Multiplication,
    Lab65Y_Division
};

static IEcoUnknownVTbl Lab65PrivateVtbl = {
    Lab65Private_QueryInterface,
    Lab65Private_AddRef,
    Lab65Private_Release
};

/* Safe temporary error stubs */
int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;
    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    IEcoSystem1* owned_system = 0;
    IEcoInterfaceBus1* owned_bus = 0;
    IEcoInterfaceBus1MemExt* owned_extension = 0;
    IEcoMemoryAllocator1* owned_allocator = 0;
    Lab65State* state = 0;
    UGUID memory_manager_cid = CID_EcoMemoryManager1;
    const UGUID* temp_manager = 0;
    int16_t status = ERR_ECO_SUCCESS;

    /* Query for system and bus */
    if (status == ERR_ECO_SUCCESS) {
        status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
        if (status == ERR_ECO_SUCCESS) {
            status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&owned_bus);
        }
    }

    /* Query for optional memory extension */
    if (status == ERR_ECO_SUCCESS) {
        status = owned_bus->pVTbl->QueryInterface(owned_bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&owned_extension);
        if (status == ERR_ECO_SUCCESS) {
            temp_manager = owned_extension->pVTbl->get_Manager(owned_extension);
            if (temp_manager != 0) {
                memory_manager_cid = *temp_manager;
            }
            owned_extension->pVTbl->Release(owned_extension);
            owned_extension = 0;
        } else {
            status = ERR_ECO_SUCCESS; /* Optional extension not found, use default */
        }
    }

    /* Query for memory allocator */
    if (status == ERR_ECO_SUCCESS) {
        status = owned_bus->pVTbl->QueryComponent(owned_bus, &memory_manager_cid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    }

    /* Allocate state */
    if (status == ERR_ECO_SUCCESS) {
        state = (Lab65State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
        if (state == 0) {
            status = ERR_ECO_OUTOFMEMORY;
        } else {
            state->public_lab65.pVTbl = &Lab65Vtbl;
            state->private_unknown.pVTbl = &Lab65PrivateVtbl;
            state->public_x.pVTbl = &Lab65XVtbl;
            state->public_y.pVTbl = &Lab65YVtbl;
            state->local_refs = 1;
            state->controller = (outer != 0) ? outer : &state->private_unknown;
            state->owned_system = owned_system;
            state->owned_allocator = owned_allocator;
            state->inner_x = 0;
            state->inner_y = 0;

            owned_system = 0;
            owned_allocator = 0;
        }
    }

    /* Query for dependencies */
    if (status == ERR_ECO_SUCCESS) {
        status = owned_bus->pVTbl->QueryComponent(owned_bus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
        if (status == ERR_ECO_SUCCESS && state->inner_y != 0) {
            status = owned_bus->pVTbl->QueryComponent(owned_bus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
            if (status != ERR_ECO_SUCCESS || state->inner_x == 0) {
                if (state->inner_y != 0) {
                    state->inner_y->pVTbl->Release(state->inner_y);
                }
            }
        } else {
            if (state->inner_y != 0) {
                state->inner_y->pVTbl->Release(state->inner_y);
            }
        }
    }

    /* Finalize creation */
    if (status == ERR_ECO_SUCCESS) {
        if (outer != 0) {
            *out = &state->private_unknown;
        } else {
            status = Lab65Private_QueryInterface(&state->private_unknown, iid, out);
            if (status == ERR_ECO_SUCCESS) {
                Lab65Private_Release(&state->private_unknown);
            }
        }
    }

    /* Cleanup on failure */
    if (status != ERR_ECO_SUCCESS && state != 0) {
        Lab65Private_Release(&state->private_unknown);
    }

    if (owned_system != 0) {
        owned_system->pVTbl->Release(owned_system);
    }

    if (owned_allocator != 0) {
        owned_allocator->pVTbl->Release(owned_allocator);
    }

    if (owned_bus != 0) {
        owned_bus->pVTbl->Release(owned_bus);
    }

    return status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;
    if (outer != 0) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    if (me == 0) return 0;
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return pCMe->owned_allocator;
}
