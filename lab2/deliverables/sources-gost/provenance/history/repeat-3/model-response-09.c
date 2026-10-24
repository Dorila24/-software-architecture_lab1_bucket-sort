#include "ReuseContract.h"
#include "IdEcoLab65.h"

typedef struct Lab65State {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
    uint32_t local_refs;
    IEcoUnknown* controller; /* This is a borrowed reference and never AddRef'd or Released by Lab65. */
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

static int16_t ECOCALLMETHOD lab65_private_query_interface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD lab65_private_add_ref(IEcoUnknownPtr_t me);
static uint32_t ECOCALLMETHOD lab65_private_release(IEcoUnknownPtr_t me);

static int16_t ECOCALLMETHOD lab_x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD lab_x_add_ref(IEcoCalculatorXPtr_t me);
static uint32_t ECOCALLMETHOD lab_x_release(IEcoCalculatorXPtr_t me);
static int32_t ECOCALLMETHOD lab_x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD lab_x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);

static int16_t ECOCALLMETHOD lab_y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD lab_y_add_ref(IEcoCalculatorYPtr_t me);
static uint32_t ECOCALLMETHOD lab_y_release(IEcoCalculatorYPtr_t me);
static int32_t ECOCALLMETHOD lab_y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD lab_y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);

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

static IEcoUnknownVTbl lab65_private_vtbl = {
    lab65_private_query_interface,
    lab65_private_add_ref,
    lab65_private_release
};

static IEcoCalculatorXVTbl lab_x_vtbl = {
    lab_x_query_interface,
    lab_x_add_ref,
    lab_x_release,
    lab_x_addition,
    lab_x_subtraction
};

static IEcoCalculatorYVTbl lab_y_vtbl = {
    lab_y_query_interface,
    lab_y_add_ref,
    lab_y_release,
    lab_y_multiplication,
    lab_y_division
};

static int16_t ECOCALLMETHOD lab65_query_interface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == 0) return ERR_ECO_NOINTERFACE;

    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD lab65_add_ref(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == 0) return 0;

    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD lab65_release(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == 0) return 0;

    return state->controller->pVTbl->Release(state->controller);
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

static int16_t ECOCALLMETHOD lab65_private_query_interface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (state == 0) return ERR_ECO_POINTER;

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

static uint32_t ECOCALLMETHOD lab65_private_add_ref(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (state == 0) return 0;

    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD lab65_private_release(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (state == 0) return 0;

    state->local_refs--;
    if (state->local_refs == 0) {
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
        state->owned_allocator->pVTbl->Free(state->owned_allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }

    return state->local_refs;
}

static int16_t ECOCALLMETHOD lab_x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->controller == 0) return ERR_ECO_NOINTERFACE;

    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD lab_x_add_ref(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->controller == 0) return 0;

    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD lab_x_release(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->controller == 0) return 0;

    return state->controller->pVTbl->Release(state->controller);
}

static int32_t ECOCALLMETHOD lab_x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->inner_x == 0) return 0;

    return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
}

static int16_t ECOCALLMETHOD lab_x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->inner_x == 0) return 0;

    return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
}

static int16_t ECOCALLMETHOD lab_y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->controller == 0) return ERR_ECO_NOINTERFACE;

    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD lab_y_add_ref(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->controller == 0) return 0;

    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD lab_y_release(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->controller == 0) return 0;

    return state->controller->pVTbl->Release(state->controller);
}

static int32_t ECOCALLMETHOD lab_y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->inner_y == 0) return 0;

    return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
}

static int16_t ECOCALLMETHOD lab_y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->inner_y == 0) return 0;

    return state->inner_y->pVTbl->Division(state->inner_y, a, b);
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65State* state;
    if (me == 0) return 0;

    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->owned_allocator;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* owned_system = 0;
    IEcoInterfaceBus1* owned_bus = 0;
    IEcoInterfaceBus1MemExt* owned_extension = 0;
    IEcoMemoryAllocator1* owned_allocator = 0;
    Lab65State* state = 0;
    UGUID memory_manager_cid = CID_EcoMemoryManager1;
    const UGUID* manager_cid = &memory_manager_cid;
    int16_t status = ERR_ECO_SUCCESS;

    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;

    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;

    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOAGGREGATION;
    }

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&owned_bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = owned_bus->pVTbl->QueryInterface(owned_bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&owned_extension);
    if (status == ERR_ECO_SUCCESS) {
        manager_cid = owned_extension->pVTbl->get_Manager(owned_extension);
        if (manager_cid != 0) {
            memory_manager_cid = *manager_cid;
        }
        owned_extension->pVTbl->Release(owned_extension);
    }

    status = owned_bus->pVTbl->QueryComponent(owned_bus, &memory_manager_cid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    state = (Lab65State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
    if (state == 0) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab65.pVTbl = &lab65_vtbl;
    state->private_unknown.pVTbl = &lab65_private_vtbl;
    state->public_x.pVTbl = &lab_x_vtbl;
    state->public_y.pVTbl = &lab_y_vtbl;
    state->local_refs = 1;
    state->controller = (outer != 0) ? outer : &state->private_unknown;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->inner_x = 0;
    state->inner_y = 0;

    owned_system = 0;
    owned_allocator = 0;

    status = owned_bus->pVTbl->QueryComponent(owned_bus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != ERR_ECO_SUCCESS || state->inner_y == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = owned_bus->pVTbl->QueryComponent(owned_bus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != ERR_ECO_SUCCESS || state->inner_x == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    if (outer != 0) {
        *out = &state->private_unknown;
        owned_bus->pVTbl->Release(owned_bus);
        return ERR_ECO_SUCCESS;
    }

    status = lab65_private_query_interface(&state->private_unknown, iid, out);
    if (status == ERR_ECO_SUCCESS) {
        lab65_private_release(&state->private_unknown);
    } else {
        lab65_private_release(&state->private_unknown);
    }

    state = 0;

cleanup:
    if (state != 0) {
        lab65_private_release(&state->private_unknown);
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
    lab2_add_ref,
    lab2_release,
    lab2_get_version
};

static IEcoCalculatorXVTbl lab2_x_vtbl = {
    (int16_t (ECOCALLMETHOD *)(IEcoCalculatorXPtr_t, const UGUID*, voidptr_t*))lab2_x_query_interface,
    lab2_x_add_ref,
    lab2_x_release,
    lab2_x_addition,
    lab2_x_subtraction
};

static IEcoCalculatorYVTbl lab2_y_vtbl = {
    (int16_t (ECOCALLMETHOD *)(IEcoCalculatorYPtr_t, const UGUID*, voidptr_t*))lab2_y_query_interface,
    lab2_y_add_ref,
    lab2_y_release,
    lab2_y_multiplication,
    lab2_y_division
};

static int16_t ECOCALLMETHOD lab2_query_interface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
    if (state == 0) return ERR_ECO_POINTER;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }

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

    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD lab2_add_ref(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
    if (state == 0) return 0;

    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD lab2_release(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
    if (state == 0) return 0;

    state->local_refs--;
    if (state->local_refs == 0) {
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
        state->owned_allocator->pVTbl->Free(state->owned_allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }

    return state->local_refs;
}

static uint32_t ECOCALLMETHOD lab2_get_version(IEcoLab2Ptr_t me) {
    if (me == 0) return 0;
    return 2;
}

static int16_t ECOCALLMETHOD lab2_x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    if (state == 0) return ERR_ECO_POINTER;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->public_x;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }

    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }

    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD lab2_x_add_ref(IEcoCalculatorXPtr_t me) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    if (state == 0) return 0;

    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD lab2_x_release(IEcoCalculatorXPtr_t me) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    if (state == 0) return 0;

    state->local_refs--;
    if (state->local_refs == 0) {
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
        state->owned_allocator->pVTbl->Free(state->owned_allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }

    return state->local_refs;
}

static int32_t ECOCALLMETHOD lab2_x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    if (state->owned_inner_x == 0) return 0;

    return state->owned_inner_x->pVTbl->Addition(state->owned_inner_x, a, b);
}

static int16_t ECOCALLMETHOD lab2_x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    if (state->owned_inner_x == 0) return 0;

    return state->owned_inner_x->pVTbl->Subtraction(state->owned_inner_x, a, b);
}

static int16_t ECOCALLMETHOD lab2_y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    if (state == 0) return ERR_ECO_POINTER;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->public_y;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }

    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    }

    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD lab2_y_add_ref(IEcoCalculatorYPtr_t me) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    if (state == 0) return 0;

    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD lab2_y_release(IEcoCalculatorYPtr_t me) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    if (state == 0) return 0;

    state->local_refs--;
    if (state->local_refs == 0) {
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
        state->owned_allocator->pVTbl->Free(state->owned_allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }

    return state->local_refs;
}

static int32_t ECOCALLMETHOD lab2_y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    if (state->owned_inner_y == 0) return 0;

    return state->owned_inner_y->pVTbl->Multiplication(state->owned_inner_y, a, b);
}

static int16_t ECOCALLMETHOD lab2_y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab2State* state;
    if (me == 0) return 0;

    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    if (state->owned_inner_y == 0) return 0;

    return state->owned_inner_y->pVTbl->Division(state->owned_inner_y, a, b);
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* owned_system = 0;
    IEcoInterfaceBus1* owned_bus = 0;
    IEcoInterfaceBus1MemExt* owned_extension = 0;
    IEcoMemoryAllocator1* owned_allocator = 0;
    Lab2State* state = 0;
    UGUID memory_manager_cid = CID_EcoMemoryManager1;
    const UGUID* manager_cid = &memory_manager_cid;
    int16_t status = ERR_ECO_SUCCESS;

    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;

    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;

    if (outer != 0) return ERR_ECO_NOAGGREGATION;

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&owned_bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = owned_bus->pVTbl->QueryInterface(owned_bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&owned_extension);
    if (status == ERR_ECO_SUCCESS) {
        manager_cid = owned_extension->pVTbl->get_Manager(owned_extension);
        if (manager_cid != 0) {
            memory_manager_cid = *manager_cid;
        }
        owned_extension->pVTbl->Release(owned_extension);
    }

    status = owned_bus->pVTbl->QueryComponent(owned_bus, &memory_manager_cid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    state = (Lab2State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab2State));
    if (state == 0) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

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

    status = owned_bus->pVTbl->QueryComponent(owned_bus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->owned_inner_y);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_y == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = state->owned_inner_y->pVTbl->QueryInterface(state->owned_inner_y, &IID_IEcoCalculatorX, (voidptr_t*)&state->owned_inner_x);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_x == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = owned_bus->pVTbl->QueryComponent(owned_bus, &CID_EcoLab65, &state->public_lab2, &IID_IEcoUnknown, (voidptr_t*)&state->owned_inner_unknown);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_unknown == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = lab2_query_interface(&state->public_lab2, iid, out);
    if (status == ERR_ECO_SUCCESS) {
        lab2_release(&state->public_lab2);
    } else {
        lab2_release(&state->public_lab2);
    }

    state = 0;

cleanup:
    if (state != 0) {
        lab2_release(&state->public_lab2);
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
