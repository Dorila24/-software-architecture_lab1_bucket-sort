#include "ReuseContract.h"
#include "IdEcoLab65.h"

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
    Lab65State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->private_unknown;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &state->public_lab65;
        state->public_lab65.pVTbl->AddRef(&state->public_lab65);
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->public_x.pVTbl->AddRef(&state->public_x);
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->public_y.pVTbl->AddRef(&state->public_y);
        return ERR_ECO_SUCCESS;
    }

    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD Lab65Private_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    return ++state->local_refs;
}

static uint32_t ECOCALLMETHOD Lab65Private_Release(IEcoUnknownPtr_t me) {
    Lab65State* state;
    IEcoMemoryAllocator1* saved_allocator;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (--state->local_refs == 0) {
        saved_allocator = state->owned_allocator;
        if (state->inner_x != 0) state->inner_x->pVTbl->Release(state->inner_x);
        if (state->inner_y != 0) state->inner_y->pVTbl->Release(state->inner_y);
        if (state->owned_system != 0) state->owned_system->pVTbl->Release(state->owned_system);
        saved_allocator->pVTbl->Free(saved_allocator, state);
        saved_allocator->pVTbl->Release(saved_allocator);
        return 0;
    }
    return state->local_refs;
}

/* Public Lab65 methods */
static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));

    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->Release(state->controller);
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
    Lab65State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));

    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD Lab65X_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->controller->pVTbl->Release(state->controller);
}

static int32_t ECOCALLMETHOD Lab65X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
}

static int16_t ECOCALLMETHOD Lab65X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
}

/* Public Y methods */
static int16_t ECOCALLMETHOD Lab65Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));

    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD Lab65Y_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->controller->pVTbl->Release(state->controller);
}

static int32_t ECOCALLMETHOD Lab65Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
}

static int16_t ECOCALLMETHOD Lab65Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->inner_y->pVTbl->Division(state->inner_y, a, b);
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

/* Ownership comment: Lab65's controller is borrowed and public controls delegate to it. */
int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* owned_system = 0;
    IEcoInterfaceBus1* owned_bus = 0;
    IEcoInterfaceBus1MemExt* owned_extension = 0;
    IEcoMemoryAllocator1* owned_allocator = 0;
    Lab65State* state = 0;
    UGUID memory_manager_cid = CID_EcoMemoryManager1;
    const UGUID* temp_manager = 0;
    int16_t status = ERR_ECO_SUCCESS;

    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;
    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

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
                status = ERR_ECO_NOINTERFACE;
            }
        } else {
            status = ERR_ECO_NOINTERFACE;
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

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65State* state;
    if (me == 0) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->owned_allocator;
}

typedef struct Lab2State {
    IEcoLab2 public_lab2;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
    uint32_t local_refs;
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* owned_inner_x;
    IEcoCalculatorY* owned_inner_y;
    IEcoUnknown* owned_inner_unknown;
} Lab2State;

/* Public Lab2 methods */
static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        return state->owned_inner_unknown->pVTbl->QueryInterface(state->owned_inner_unknown, riid, ppv);
    }

    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
    return ++state->local_refs;
}

static uint32_t ECOCALLMETHOD Lab2_Release(IEcoLab2Ptr_t me) {
    Lab2State* state;
    IEcoMemoryAllocator1* saved_allocator;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
    if (--state->local_refs == 0) {
        saved_allocator = state->owned_allocator;
        if (state->owned_inner_x != 0) state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
        if (state->owned_inner_y != 0) state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
        if (state->owned_inner_unknown != 0) state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
        if (state->owned_system != 0) state->owned_system->pVTbl->Release(state->owned_system);
        saved_allocator->pVTbl->Free(saved_allocator, state);
        saved_allocator->pVTbl->Release(saved_allocator);
        return 0;
    }
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD Lab2_GetVersion(IEcoLab2Ptr_t me) {
    if (me == 0) return 0;
    return 2;
}

/* Public X methods */
static int16_t ECOCALLMETHOD Lab2X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));

    return Lab2_QueryInterface((IEcoLab2Ptr_t)&state->public_lab2, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab2X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    return Lab2_AddRef((IEcoLab2Ptr_t)&state->public_lab2);
}

static uint32_t ECOCALLMETHOD Lab2X_Release(IEcoCalculatorXPtr_t me) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    return Lab2_Release((IEcoLab2Ptr_t)&state->public_lab2);
}

static int32_t ECOCALLMETHOD Lab2X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    return state->owned_inner_x->pVTbl->Addition(state->owned_inner_x, a, b);
}

static int16_t ECOCALLMETHOD Lab2X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    return state->owned_inner_x->pVTbl->Subtraction(state->owned_inner_x, a, b);
}

/* Public Y methods */
static int16_t ECOCALLMETHOD Lab2Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));

    return Lab2_QueryInterface((IEcoLab2Ptr_t)&state->public_lab2, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab2Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    return Lab2_AddRef((IEcoLab2Ptr_t)&state->public_lab2);
}

static uint32_t ECOCALLMETHOD Lab2Y_Release(IEcoCalculatorYPtr_t me) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    return Lab2_Release((IEcoLab2Ptr_t)&state->public_lab2);
}

static int32_t ECOCALLMETHOD Lab2Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    return state->owned_inner_y->pVTbl->Multiplication(state->owned_inner_y, a, b);
}

static int16_t ECOCALLMETHOD Lab2Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == 0) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    return state->owned_inner_y->pVTbl->Division(state->owned_inner_y, a, b);
}

/* Static tables */
static IEcoLab2VTbl lab2_primary_vtbl = {
    Lab2_QueryInterface,
    Lab2_AddRef,
    Lab2_Release,
    Lab2_GetVersion
};

static IEcoCalculatorXVTbl lab2_x_vtbl = {
    Lab2X_QueryInterface,
    Lab2X_AddRef,
    Lab2X_Release,
    Lab2X_Addition,
    Lab2X_Subtraction
};

static IEcoCalculatorYVTbl lab2_y_vtbl = {
    Lab2Y_QueryInterface,
    Lab2Y_AddRef,
    Lab2Y_Release,
    Lab2Y_Multiplication,
    Lab2Y_Division
};

/* Ownership comment: Lab2 owns the E interfaces and only the private inner Unknown, avoiding a controller reference cycle. */
int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* owned_system = 0;
    IEcoInterfaceBus1* owned_bus = 0;
    IEcoInterfaceBus1MemExt* owned_extension = 0;
    IEcoMemoryAllocator1* owned_allocator = 0;
    Lab2State* state = 0;
    UGUID memory_manager_cid = CID_EcoMemoryManager1;
    const UGUID* temp_manager = 0;
    int16_t status = ERR_ECO_SUCCESS;

    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;
    if (outer != 0) return ERR_ECO_NOAGGREGATION;

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
        state = (Lab2State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab2State));
        if (state == 0) {
            status = ERR_ECO_OUTOFMEMORY;
        } else {
            state->public_lab2.pVTbl = &lab2_primary_vtbl;
            state->public_x.pVTbl = &lab2_x_vtbl;
            state->public_y.pVTbl = &lab2_y_vtbl;
            state->local_refs = 1;
            state->owned_system = owned_system;
            state->owned_allocator = owned_allocator;
            state->owned_inner_x = 0;
            state->owned_inner_y = 0;
            state->owned_inner_unknown = 0;

            owned_system = 0;
            owned_allocator = 0;
        }
    }

    /* Query for dependencies */
    if (status == ERR_ECO_SUCCESS) {
        status = owned_bus->pVTbl->QueryComponent(owned_bus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->owned_inner_y);
        if (status == ERR_ECO_SUCCESS && state->owned_inner_y != 0) {
            status = state->owned_inner_y->pVTbl->QueryInterface(state->owned_inner_y, &IID_IEcoCalculatorX, (voidptr_t*)&state->owned_inner_x);
            if (status != ERR_ECO_SUCCESS || state->owned_inner_x == 0) {
                status = ERR_ECO_NOINTERFACE;
            }
        } else {
            status = ERR_ECO_NOINTERFACE;
        }
    }

    if (status == ERR_ECO_SUCCESS) {
        status = owned_bus->pVTbl->QueryComponent(owned_bus, &CID_EcoLab65, (IEcoUnknown*)&state->public_lab2, &IID_IEcoUnknown, (voidptr_t*)&state->owned_inner_unknown);
        if (status != ERR_ECO_SUCCESS || state->owned_inner_unknown == 0) {
            status = ERR_ECO_NOINTERFACE;
        }
    }

    /* Finalize creation */
    if (status == ERR_ECO_SUCCESS) {
        status = Lab2_QueryInterface((IEcoLab2Ptr_t)&state->public_lab2, iid, out);
        if (status == ERR_ECO_SUCCESS) {
            Lab2_Release((IEcoLab2Ptr_t)&state->public_lab2);
        }
    }

    /* Cleanup on failure */
    if (status != ERR_ECO_SUCCESS && state != 0) {
        Lab2_Release((IEcoLab2Ptr_t)&state->public_lab2);
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
