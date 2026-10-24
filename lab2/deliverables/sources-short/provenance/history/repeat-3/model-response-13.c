#include "ReuseContract.h"

typedef struct Lab65State {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
    uint32_t local_refs;
    IEcoUnknown* controller; /* The controller is borrowed and must not be AddRefed/Released by the inner object. */
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
} Lab65State;

static int16_t ECOCALLMETHOD lab65_query_interface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD lab65_add_ref(IEcoLab65Ptr_t me);
static uint32_t ECOCALLMETHOD lab65_release(IEcoLab65Ptr_t me);
static int16_t ECOCALLMETHOD lab65_sort_int(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD lab65_sort_long(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD lab65_sort_float(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD lab65_sort_double(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD lab65_sort_long_double(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets);

static int16_t ECOCALLMETHOD unknown_query_interface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD unknown_add_ref(IEcoUnknownPtr_t me);
static uint32_t ECOCALLMETHOD unknown_release(IEcoUnknownPtr_t me);

static int16_t ECOCALLMETHOD x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD x_add_ref(IEcoCalculatorXPtr_t me);
static uint32_t ECOCALLMETHOD x_release(IEcoCalculatorXPtr_t me);
static int32_t ECOCALLMETHOD x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);

static int16_t ECOCALLMETHOD y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD y_add_ref(IEcoCalculatorYPtr_t me);
static uint32_t ECOCALLMETHOD y_release(IEcoCalculatorYPtr_t me);
static int32_t ECOCALLMETHOD y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);

static IEcoLab65VTbl lab65_vtbl = {
    lab65_query_interface,
    lab65_add_ref,
    lab65_release,
    lab65_sort_int,
    lab65_sort_long,
    lab65_sort_float,
    lab65_sort_double,
    lab65_sort_long_double
};

static IEcoUnknownVTbl unknown_vtbl = {
    unknown_query_interface,
    unknown_add_ref,
    unknown_release
};

static IEcoCalculatorXVTbl x_vtbl = {
    x_query_interface,
    x_add_ref,
    x_release,
    x_addition,
    x_subtraction
};

static IEcoCalculatorYVTbl y_vtbl = {
    y_query_interface,
    y_add_ref,
    y_release,
    y_multiplication,
    y_division
};

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return state->owned_allocator;
    }
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t status = ERR_ECO_SUCCESS;
    IEcoSystem1* local_system = 0;
    IEcoInterfaceBus1* bus = 0;
    IEcoInterfaceBus1MemExt* ext = 0;
    const UGUID* manager = &CID_EcoMemoryManager1;
    IEcoMemoryAllocator1* allocator = 0;
    Lab65State* state = 0;
    UGUID owned_manager;

    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOSYSTEM;
    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&local_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = local_system->pVTbl->QueryInterface(local_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&ext);
    if (status == ERR_ECO_SUCCESS) {
        const UGUID* ext_manager = ext->pVTbl->get_Manager(ext);
        if (ext_manager != 0) {
            owned_manager = *ext_manager;
        } else {
            status = ERR_ECO_NOINTERFACE;
        }
        ext->pVTbl->Release(ext);
        if (status != ERR_ECO_SUCCESS) goto cleanup;
    } else {
        owned_manager = *manager;
    }

    status = bus->pVTbl->QueryComponent(bus, &owned_manager, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != ERR_ECO_SUCCESS || allocator == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    state = (Lab65State*)allocator->pVTbl->Alloc(allocator, sizeof(Lab65State));
    if (state == 0) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab65.pVTbl = &lab65_vtbl;
    state->private_unknown.pVTbl = &unknown_vtbl;
    state->public_x.pVTbl = &x_vtbl;
    state->public_y.pVTbl = &y_vtbl;
    state->local_refs = 1;
    state->inner_x = 0;
    state->inner_y = 0;
    state->owned_system = local_system;
    state->owned_allocator = allocator;
    state->controller = (outer != 0) ? outer : &state->private_unknown;

    local_system = 0;
    allocator = 0;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != ERR_ECO_SUCCESS || state->inner_y == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != ERR_ECO_SUCCESS || state->inner_x == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    if (outer != 0) {
        *out = &state->private_unknown;
        state = 0; /* Transfer ownership to the caller */
        goto cleanup;
    }

    status = unknown_query_interface(&state->private_unknown, iid, out);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

cleanup:
    if (state != 0) {
        unknown_release(&state->private_unknown);
    }

    if (local_system != 0) {
        local_system->pVTbl->Release(local_system);
    }

    if (allocator != 0) {
        allocator->pVTbl->Release(allocator);
    }

    if (bus != 0) {
        bus->pVTbl->Release(bus);
    }

    return status;
}

static int16_t ECOCALLMETHOD lab65_query_interface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));

        if (state->controller != 0) {
            return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD lab65_add_ref(IEcoLab65Ptr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        if (state->controller != 0) {
            return state->controller->pVTbl->AddRef(state->controller);
        }
        return 0;
    }
}

static uint32_t ECOCALLMETHOD lab65_release(IEcoLab65Ptr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        if (state->controller != 0) {
            return state->controller->pVTbl->Release(state->controller);
        }
        return 0;
    }
}

static int16_t ECOCALLMETHOD lab65_sort_int(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_sort_long(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_sort_float(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_sort_double(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD lab65_sort_long_double(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD unknown_query_interface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));

        if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
            *ppv = &state->public_lab65;
            state->public_lab65.pVTbl->AddRef(&state->public_lab65);
            return ERR_ECO_SUCCESS;
        }

        if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
            *ppv = &state->private_unknown;
            state->local_refs++;
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
}

static uint32_t ECOCALLMETHOD unknown_add_ref(IEcoUnknownPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
        return ++state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD unknown_release(IEcoUnknownPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
        if (--state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->inner_x != 0) {
                state->inner_x->pVTbl->Release(state->inner_x);
            }
            if (state->inner_y != 0) {
                state->inner_y->pVTbl->Release(state->inner_y);
            }
            if (state->owned_system != 0) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
            return 0;
        }
        return state->local_refs;
    }
}

static int16_t ECOCALLMETHOD x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));

        if (state->controller != 0) {
            return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD x_add_ref(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state->controller != 0) {
            return state->controller->pVTbl->AddRef(state->controller);
        }
        return 0;
    }
}

static uint32_t ECOCALLMETHOD x_release(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state->controller != 0) {
            return state->controller->pVTbl->Release(state->controller);
        }
        return 0;
    }
}

static int32_t ECOCALLMETHOD x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state->inner_x != 0) {
            return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
        }
        return 0;
    }
}

static int16_t ECOCALLMETHOD x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state->inner_x != 0) {
            return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
        }
        return 0;
    }
}

static int16_t ECOCALLMETHOD y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));

        if (state->controller != 0) {
            return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD y_add_ref(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state->controller != 0) {
            return state->controller->pVTbl->AddRef(state->controller);
        }
        return 0;
    }
}

static uint32_t ECOCALLMETHOD y_release(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state->controller != 0) {
            return state->controller->pVTbl->Release(state->controller);
        }
        return 0;
    }
}

static int32_t ECOCALLMETHOD y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state->inner_y != 0) {
            return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
        }
        return 0;
    }
}

static int16_t ECOCALLMETHOD y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state->inner_y != 0) {
            return state->inner_y->pVTbl->Division(state->inner_y, a, b);
        }
        return 0;
    }
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

static int16_t ECOCALLMETHOD lab2_query_interface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD lab2_add_ref(IEcoLab2Ptr_t me);
static uint32_t ECOCALLMETHOD lab2_release(IEcoLab2Ptr_t me);
static uint32_t ECOCALLMETHOD lab2_get_version(IEcoLab2Ptr_t me);

static int16_t ECOCALLMETHOD lab2_x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD lab2_x_add_ref(IEcoCalculatorXPtr_t me);
static uint32_t ECOCALLMETHOD lab2_x_release(IEcoCalculatorXPtr_t me);
static int32_t ECOCALLMETHOD lab2_x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD lab2_x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);

static int16_t ECOCALLMETHOD lab2_y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD lab2_y_add_ref(IEcoCalculatorYPtr_t me);
static uint32_t ECOCALLMETHOD lab2_y_release(IEcoCalculatorYPtr_t me);
static int32_t ECOCALLMETHOD lab2_y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD lab2_y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);

static IEcoLab2VTbl lab2_primary_vtbl = {
    (int16_t (ECOCALLMETHOD *)(IEcoLab2Ptr_t, const UGUID*, voidptr_t*))lab2_query_interface,
    (uint32_t (ECOCALLMETHOD *)(IEcoLab2Ptr_t))lab2_add_ref,
    (uint32_t (ECOCALLMETHOD *)(IEcoLab2Ptr_t))lab2_release,
    lab2_get_version
};

static IEcoCalculatorXVTbl lab2_x_vtbl = {
    (int16_t (ECOCALLMETHOD *)(IEcoCalculatorXPtr_t, const UGUID*, voidptr_t*))lab2_x_query_interface,
    (uint32_t (ECOCALLMETHOD *)(IEcoCalculatorXPtr_t))lab2_x_add_ref,
    (uint32_t (ECOCALLMETHOD *)(IEcoCalculatorXPtr_t))lab2_x_release,
    lab2_x_addition,
    lab2_x_subtraction
};

static IEcoCalculatorYVTbl lab2_y_vtbl = {
    (int16_t (ECOCALLMETHOD *)(IEcoCalculatorYPtr_t, const UGUID*, voidptr_t*))lab2_y_query_interface,
    (uint32_t (ECOCALLMETHOD *)(IEcoCalculatorYPtr_t))lab2_y_add_ref,
    (uint32_t (ECOCALLMETHOD *)(IEcoCalculatorYPtr_t))lab2_y_release,
    lab2_y_multiplication,
    lab2_y_division
};

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t status = ERR_ECO_SUCCESS;
    IEcoSystem1* local_system = 0;
    IEcoInterfaceBus1* bus = 0;
    IEcoInterfaceBus1MemExt* ext = 0;
    const UGUID* manager = &CID_EcoMemoryManager1;
    IEcoMemoryAllocator1* allocator = 0;
    Lab2State* state = 0;
    UGUID owned_manager;

    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOSYSTEM;
    if (outer != 0) return ERR_ECO_NOAGGREGATION;

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&local_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = local_system->pVTbl->QueryInterface(local_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&ext);
    if (status == ERR_ECO_SUCCESS) {
        const UGUID* ext_manager = ext->pVTbl->get_Manager(ext);
        if (ext_manager != 0) {
            owned_manager = *ext_manager;
        } else {
            status = ERR_ECO_NOINTERFACE;
        }
        ext->pVTbl->Release(ext);
        if (status != ERR_ECO_SUCCESS) goto cleanup;
    } else {
        owned_manager = *manager;
    }

    status = bus->pVTbl->QueryComponent(bus, &owned_manager, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != ERR_ECO_SUCCESS || allocator == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    state = (Lab2State*)allocator->pVTbl->Alloc(allocator, sizeof(Lab2State));
    if (state == 0) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab2.pVTbl = &lab2_primary_vtbl;
    state->public_x.pVTbl = &lab2_x_vtbl;
    state->public_y.pVTbl = &lab2_y_vtbl;
    state->local_refs = 1;
    state->owned_inner_x = 0;
    state->owned_inner_y = 0;
    state->owned_inner_unknown = 0;
    state->owned_system = local_system;
    state->owned_allocator = allocator;

    local_system = 0;
    allocator = 0;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->owned_inner_y);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_y == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = state->owned_inner_y->pVTbl->QueryInterface(state->owned_inner_y, &IID_IEcoCalculatorX, (voidptr_t*)&state->owned_inner_x);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_x == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoLab65, (IEcoUnknown*)&state->public_lab2, &IID_IEcoUnknown, (voidptr_t*)&state->owned_inner_unknown);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_unknown == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = lab2_query_interface(&state->public_lab2, iid, out);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

cleanup:
    if (status != ERR_ECO_SUCCESS && state != 0) {
        lab2_release(&state->public_lab2);
    }

    if (bus != 0) {
        bus->pVTbl->Release(bus);
    }

    return status;
}

static int16_t ECOCALLMETHOD lab2_query_interface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));

        if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
            *ppv = &state->public_lab2;
            state->local_refs++;
            return ERR_ECO_SUCCESS;
        }

        if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
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

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD lab2_add_ref(IEcoLab2Ptr_t me) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return ++state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD lab2_release(IEcoLab2Ptr_t me) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        if (--state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->owned_inner_x != 0) {
                state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
            }
            if (state->owned_inner_y != 0) {
                state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
            }
            if (state->owned_inner_unknown != 0) {
                state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
            }
            if (state->owned_system != 0) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
            return 0;
        }
        return state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD lab2_get_version(IEcoLab2Ptr_t me) {
    if (me == 0) return 0;
    return 2;
}

static int16_t ECOCALLMETHOD lab2_x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));

        if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
            *ppv = &state->public_x;
            state->local_refs++;
            return ERR_ECO_SUCCESS;
        }

        if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
            *ppv = &state->public_x;
            state->local_refs++;
            return ERR_ECO_SUCCESS;
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD lab2_x_add_ref(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        return ++state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD lab2_x_release(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        if (--state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->owned_inner_x != 0) {
                state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
            }
            if (state->owned_inner_y != 0) {
                state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
            }
            if (state->owned_inner_unknown != 0) {
                state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
            }
            if (state->owned_system != 0) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
            return 0;
        }
        return state->local_refs;
    }
}

static int32_t ECOCALLMETHOD lab2_x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        if (state->owned_inner_x != 0) {
            return state->owned_inner_x->pVTbl->Addition(state->owned_inner_x, a, b);
        }
        return 0;
    }
}

static int16_t ECOCALLMETHOD lab2_x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        if (state->owned_inner_x != 0) {
            return state->owned_inner_x->pVTbl->Subtraction(state->owned_inner_x, a, b);
        }
        return 0;
    }
}

static int16_t ECOCALLMETHOD lab2_y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));

        if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
            *ppv = &state->public_y;
            state->local_refs++;
            return ERR_ECO_SUCCESS;
        }

        if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
            *ppv = &state->public_y;
            state->local_refs++;
            return ERR_ECO_SUCCESS;
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD lab2_y_add_ref(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
        return ++state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD lab2_y_release(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab65State, public_y));
        if (--state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->owned_inner_x != 0) {
                state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
            }
            if (state->owned_inner_y != 0) {
                state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
            }
            if (state->owned_inner_unknown != 0) {
                state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
            }
            if (state->owned_system != 0) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
            return 0;
        }
        return state->local_refs;
    }
}

static int32_t ECOCALLMETHOD lab2_y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
        if (state->owned_inner_y != 0) {
            return state->owned_inner_y->pVTbl->Multiplication(state->owned_inner_y, a, b);
        }
        return 0;
    }
}

static int16_t ECOCALLMETHOD lab2_y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
        if (state->owned_inner_y != 0) {
            return state->owned_inner_y->pVTbl->Division(state->owned_inner_y, a, b);
        }
        return 0;
    }
}
