#include "ReuseContract.h"
#include "IdEcoLab65.h"

typedef struct {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
    uint32_t local_refs;
    IEcoUnknown* controller;  /* borrowed, do not AddRef or Release */
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
} Lab65State;

int16_t ECOCALLMETHOD public_Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv != NULL) *ppv = NULL;  
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

uint32_t ECOCALLMETHOD public_Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->AddRef(state->controller);
}

uint32_t ECOCALLMETHOD public_Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->Release(state->controller);
}

int16_t ECOCALLMETHOD public_X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv != NULL) *ppv = NULL;  
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

uint32_t ECOCALLMETHOD public_X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->controller->pVTbl->AddRef(state->controller);
}

uint32_t ECOCALLMETHOD public_X_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->controller->pVTbl->Release(state->controller);
}

int16_t ECOCALLMETHOD public_Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv != NULL) *ppv = NULL;  
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

uint32_t ECOCALLMETHOD public_Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->controller->pVTbl->AddRef(state->controller);
}

uint32_t ECOCALLMETHOD public_Y_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->controller->pVTbl->Release(state->controller);
}

int16_t ECOCALLMETHOD private_Unknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->private_unknown;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &state->public_lab65;
        state->public_lab65.pVTbl->AddRef(&state->public_lab65);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->public_x.pVTbl->AddRef(&state->public_x);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->public_y.pVTbl->AddRef(&state->public_y);
        return ERR_ECO_SUCCESS;
    }
    return ERR_ECO_NOINTERFACE;
}

uint32_t ECOCALLMETHOD private_Unknown_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    return ++state->local_refs;
}

/* private ownership saves allocator before freeing state */
uint32_t ECOCALLMETHOD private_Unknown_Release(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (--state->local_refs == 0) {
        IEcoMemoryAllocator1* saved_allocator = state->owned_allocator;  
        if (state->inner_x != NULL) {
            state->inner_x->pVTbl->Release(state->inner_x);
        }
        if (state->inner_y != NULL) {
            state->inner_y->pVTbl->Release(state->inner_y);
        }
        if (state->owned_system != NULL) {
            state->owned_system->pVTbl->Release(state->owned_system);
        }
        saved_allocator->pVTbl->Free(saved_allocator, state);
        saved_allocator->pVTbl->Release(saved_allocator);
        return 0;
    }
    return state->local_refs;
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65State* state;
    if (me == NULL) return NULL;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->owned_allocator;
}

int32_t ECOCALLMETHOD public_X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
}

int16_t ECOCALLMETHOD public_X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
}

int32_t ECOCALLMETHOD public_Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
}

int16_t ECOCALLMETHOD public_Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->inner_y->pVTbl->Division(state->inner_y, a, b);
}

int16_t ECOCALLMETHOD public_Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int(me, data, count, buckets);
}

int16_t ECOCALLMETHOD public_Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long(me, data, count, buckets);
}

int16_t ECOCALLMETHOD public_Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float(me, data, count, buckets);
}

int16_t ECOCALLMETHOD public_Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double(me, data, count, buckets);
}

int16_t ECOCALLMETHOD public_Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double(me, data, count, buckets);
}

static IEcoLab65VTbl public_Lab65_vtbl = {
    public_Lab65_QueryInterface,
    public_Lab65_AddRef,
    public_Lab65_Release,
    public_Lab65_SortInt,
    public_Lab65_SortLong,
    public_Lab65_SortFloat,
    public_Lab65_SortDouble,
    public_Lab65_SortLongDouble
};

static IEcoCalculatorXVTbl public_X_vtbl = {
    public_X_QueryInterface,
    public_X_AddRef,
    public_X_Release,
    public_X_Addition,
    public_X_Subtraction
};

static IEcoCalculatorYVTbl public_Y_vtbl = {
    public_Y_QueryInterface,
    public_Y_AddRef,
    public_Y_Release,
    public_Y_Multiplication,
    public_Y_Division
};

static IEcoUnknownVTbl private_Unknown_vtbl = {
    private_Unknown_QueryInterface,
    private_Unknown_AddRef,
    private_Unknown_Release
};

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoInterfaceBus1MemExt* extension = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab65State* state = NULL;
    UGUID memory_manager_cid = CID_EcoMemoryManager1;
    const UGUID* manager_cid = &memory_manager_cid;
    int16_t status = ERR_ECO_SUCCESS;

    if (out == NULL) return ERR_ECO_POINTER;
    *out = NULL;
    if (system == NULL || iid == NULL) return ERR_ECO_POINTER;

    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&extension);
    if (status == ERR_ECO_SUCCESS) {
        manager_cid = extension->pVTbl->get_Manager(extension);
        if (manager_cid != NULL) {
            memory_manager_cid = *manager_cid;
        }
        extension->pVTbl->Release(extension);
    }

    status = bus->pVTbl->QueryComponent(bus, &memory_manager_cid, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    state = (Lab65State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
    if (state == NULL) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab65.pVTbl = &public_Lab65_vtbl;
    state->private_unknown.pVTbl = &private_Unknown_vtbl;
    state->public_x.pVTbl = &public_X_vtbl;
    state->public_y.pVTbl = &public_Y_vtbl;
    state->local_refs = 1;
    state->controller = (outer != NULL) ? outer : (IEcoUnknown*)&state->private_unknown;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->inner_x = NULL;
    state->inner_y = NULL;

    owned_system = NULL;
    owned_allocator = NULL;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != ERR_ECO_SUCCESS || state->inner_y == NULL) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != ERR_ECO_SUCCESS || state->inner_x == NULL) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    if (outer != NULL) {
        *out = &state->private_unknown;
        bus->pVTbl->Release(bus);
        return ERR_ECO_SUCCESS;
    }

    status = private_Unknown_QueryInterface((IEcoUnknown*)&state->private_unknown, iid, out);
    state->controller->pVTbl->Release(state->controller);
    bus->pVTbl->Release(bus);
    return status;

cleanup:
    if (state != NULL) {
        private_Unknown_Release((IEcoUnknown*)&state->private_unknown);
    }
    if (owned_system != NULL) {
        owned_system->pVTbl->Release(owned_system);
    }
    if (owned_allocator != NULL) {
        owned_allocator->pVTbl->Release(owned_allocator);
    }
    if (bus != NULL) {
        bus->pVTbl->Release(bus);
    }
    return status;
}

typedef struct {
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

int16_t ECOCALLMETHOD public_Lab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->public_lab2.pVTbl->pVtbl->QueryInterface((IEcoUnknown*)&state->public_lab2, riid, ppv);
        return *ppv != NULL ? ERR_ECO_SUCCESS : ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_NOINTERFACE;
}

uint32_t ECOCALLMETHOD public_Lab2_AddRef(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
    return ++state->local_refs;
}

uint32_t ECOCALLMETHOD public_Lab2_Release(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
    if (--state->local_refs == 0) {
        IEcoMemoryAllocator1* saved_allocator = state->owned_allocator;
        if (state->owned_inner_x != NULL) {
            state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
        }
        if (state->owned_inner_y != NULL) {
            state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
        }
        if (state->owned_inner_unknown != NULL) {
            state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
        }
        if (state->owned_system != NULL) {
            state->owned_system->pVTbl->Release(state->owned_system);
        }
        saved_allocator->pVTbl->Free(saved_allocator, state);
        saved_allocator->pVTbl->Release(saved_allocator);
        return 0;
    }
    return state->local_refs;
}

uint32_t ECOCALLMETHOD public_Lab2_GetVersion(IEcoLab2Ptr_t me) {
    if (me == NULL) return 0;
    return 2;
}

int16_t ECOCALLMETHOD public_X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->public_x.pVTbl->pVtbl->QueryInterface((IEcoUnknown*)&state->public_x, riid, ppv);
        return *ppv != NULL ? ERR_ECO_SUCCESS : ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_NOINTERFACE;
}

uint32_t ECOCALLMETHOD public_X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    return ++state->local_refs;
}

uint32_t ECOCALLMETHOD public_X_Release(IEcoCalculatorXPtr_t me) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    if (--state->local_refs == 0) {
        IEcoMemoryAllocator1* saved_allocator = state->owned_allocator;
        if (state->owned_inner_x != NULL) {
            state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
        }
        if (state->owned_inner_y != NULL) {
            state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
        }
        if (state->owned_inner_unknown != NULL) {
            state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
        }
        if (state->owned_system != NULL) {
            state->owned_system->pVTbl->Release(state->owned_system);
        }
        saved_allocator->pVTbl->Free(saved_allocator, state);
        saved_allocator->pVTbl->Release(saved_allocator);
        return 0;
    }
    return state->local_refs;
}

int32_t ECOCALLMETHOD public_X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    return state->owned_inner_x->pVTbl->Addition(state->owned_inner_x, a, b);
}

int16_t ECOCALLMETHOD public_X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    return state->owned_inner_x->pVTbl->Subtraction(state->owned_inner_x, a, b);
}

int16_t ECOCALLMETHOD public_Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->public_y.pVTbl->pVtbl->QueryInterface((IEcoUnknown*)&state->public_y, riid, ppv);
        return *ppv != NULL ? ERR_ECO_SUCCESS : ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_NOINTERFACE;
}

uint32_t ECOCALLMETHOD public_Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    return ++state->local_refs;
}

uint32_t ECOCALLMETHOD public_Y_Release(IEcoCalculatorYPtr_t me) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    if (--state->local_refs == 0) {
        IEcoMemoryAllocator1* saved_allocator = state->owned_allocator;
        if (state->owned_inner_x != NULL) {
            state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
        }
        if (state->owned_inner_y != NULL) {
            state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
        }
        if (state->owned_inner_unknown != NULL) {
            state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
        }
        if (state->owned_system != NULL) {
            state->owned_system->pVTbl->Release(state->owned_system);
        }
        saved_allocator->pVTbl->Free(saved_allocator, state);
        saved_allocator->pVTbl->Release(saved_allocator);
        return 0;
    }
    return state->local_refs;
}

int32_t ECOCALLMETHOD public_Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    return state->owned_inner_y->pVTbl->Multiplication(state->owned_inner_y, a, b);
}

int16_t ECOCALLMETHOD public_Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    return state->owned_inner_y->pVTbl->Division(state->owned_inner_y, a, b);
}

static IEcoLab2VTbl public_Lab2_vtbl = {
    public_Lab2_QueryInterface,
    public_Lab2_AddRef,
    public_Lab2_Release,
    public_Lab2_GetVersion
};

static IEcoCalculatorXVTbl public_X_vtbl = {
    public_X_QueryInterface,
    public_X_AddRef,
    public_X_Release,
    public_X_Addition,
    public_X_Subtraction
};

static IEcoCalculatorYVTbl public_Y_vtbl = {
    public_Y_QueryInterface,
    public_Y_AddRef,
    public_Y_Release,
    public_Y_Multiplication,
    public_Y_Division
};

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoInterfaceBus1MemExt* extension = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab2State* state = NULL;
    UGUID memory_manager_cid = CID_EcoMemoryManager1;
    const UGUID* manager_cid = &memory_manager_cid;
    int16_t status = ERR_ECO_SUCCESS;

    if (out == NULL) return ERR_ECO_POINTER;
    *out = NULL;
    if (system == NULL || iid == NULL) return ERR_ECO_POINTER;

    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&extension);
    if (status == ERR_ECO_SUCCESS) {
        manager_cid = extension->pVTbl->get_Manager(extension);
        if (manager_cid != NULL) {
            memory_manager_cid = *manager_cid;
        }
        extension->pVTbl->Release(extension);
    }

    status = bus->pVTbl->QueryComponent(bus, &memory_manager_cid, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    state = (Lab2State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab2State));
    if (state == NULL) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab2.pVTbl = &public_Lab2_vtbl;
    state->public_x.pVTbl = &public_X_vtbl;
    state->public_y.pVTbl = &public_Y_vtbl;
    state->local_refs = 1;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->owned_inner_x = NULL;
    state->owned_inner_y = NULL;
    state->owned_inner_unknown = NULL;

    owned_system = NULL;
    owned_allocator = NULL;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->owned_inner_y);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_y == NULL) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = state->owned_inner_y->pVTbl->QueryInterface(state->owned_inner_y, &IID_IEcoCalculatorX, (voidptr_t*)&state->owned_inner_x);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_x == NULL) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoLab65, (IEcoUnknown*)&state->public_lab2, &IID_IEcoUnknown, (voidptr_t*)&state->owned_inner_unknown);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_unknown == NULL) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    if (outer != NULL) {
        *out = (IEcoUnknown*)&state->public_lab2;
        bus->pVTbl->Release(bus);
        return ERR_ECO_SUCCESS;
    }

    status = public_Lab2_QueryInterface((IEcoLab2Ptr_t)&state->public_lab2, iid, out);
    state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
    bus->pVTbl->Release(bus);
    return status;

cleanup:
    if (state != NULL) {
        public_Lab2_Release((IEcoLab2Ptr_t)&state->public_lab2);
    }
    if (owned_system != NULL) {
        owned_system->pVTbl->Release(owned_system);
    }
    if (owned_allocator != NULL) {
        owned_allocator->pVTbl->Release(owned_allocator);
    }
    if (bus != NULL) {
        bus->pVTbl->Release(bus);
    }
    return status;
}
