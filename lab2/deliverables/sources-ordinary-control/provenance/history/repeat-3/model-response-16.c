#include "ReuseContract.h"
#include "IdEcoLab65.h"

typedef struct Lab65State {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
    uint32_t local_refs;
    IEcoUnknown* controller; /* Borrowed ownership */
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
} Lab65State;

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me);
static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me);
static int16_t ECOCALLMETHOD Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets);

static int16_t ECOCALLMETHOD Lab65X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD Lab65X_AddRef(IEcoCalculatorXPtr_t me);
static uint32_t ECOCALLMETHOD Lab65X_Release(IEcoCalculatorXPtr_t me);
static int32_t ECOCALLMETHOD Lab65X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD Lab65X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);

static int16_t ECOCALLMETHOD Lab65Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD Lab65Y_AddRef(IEcoCalculatorYPtr_t me);
static uint32_t ECOCALLMETHOD Lab65Y_Release(IEcoCalculatorYPtr_t me);
static int32_t ECOCALLMETHOD Lab65Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD Lab65Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);

static int16_t ECOCALLMETHOD Lab65Private_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD Lab65Private_AddRef(IEcoUnknownPtr_t me);
static uint32_t ECOCALLMETHOD Lab65Private_Release(IEcoUnknownPtr_t me);

static IEcoLab65VTbl lab65_vtbl = {
    Lab65_QueryInterface,
    Lab65_AddRef,
    Lab65_Release,
    Lab65_SortInt,
    Lab65_SortLong,
    Lab65_SortFloat,
    Lab65_SortDouble,
    Lab65_SortLongDouble
};

static IEcoCalculatorXVTbl lab65x_vtbl = {
    Lab65X_QueryInterface,
    Lab65X_AddRef,
    Lab65X_Release,
    Lab65X_Addition,
    Lab65X_Subtraction
};

static IEcoCalculatorYVTbl lab65y_vtbl = {
    Lab65Y_QueryInterface,
    Lab65Y_AddRef,
    Lab65Y_Release,
    Lab65Y_Multiplication,
    Lab65Y_Division
};

static IEcoUnknownVTbl lab65private_vtbl = {
    Lab65Private_QueryInterface,
    Lab65Private_AddRef,
    Lab65Private_Release
};

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        if (state == 0) return ERR_ECO_POINTER;

        return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        if (state == 0) return 0;

        return state->controller->pVTbl->AddRef(state->controller);
    }
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        if (state == 0) return 0;

        return state->controller->pVTbl->Release(state->controller);
    }
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

static int16_t ECOCALLMETHOD Lab65X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state == 0) return ERR_ECO_POINTER;

        return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab65X_AddRef(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state == 0) return 0;

        return state->controller->pVTbl->AddRef(state->controller);
    }
}

static uint32_t ECOCALLMETHOD Lab65X_Release(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state == 0) return 0;

        return state->controller->pVTbl->Release(state->controller);
    }
}

static int32_t ECOCALLMETHOD Lab65X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state == 0 || state->inner_x == 0) return 0;

        return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab65X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state == 0 || state->inner_x == 0) return 0;

        return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab65Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state == 0) return ERR_ECO_POINTER;

        return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab65Y_AddRef(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state == 0) return 0;

        return state->controller->pVTbl->AddRef(state->controller);
    }
}

static uint32_t ECOCALLMETHOD Lab65Y_Release(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state == 0) return 0;

        return state->controller->pVTbl->Release(state->controller);
    }
}

static int32_t ECOCALLMETHOD Lab65Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state == 0 || state->inner_y == 0) return 0;

        return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab65Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state == 0 || state->inner_y == 0) return 0;

        return state->inner_y->pVTbl->Division(state->inner_y, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab65Private_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state;
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
}

static uint32_t ECOCALLMETHOD Lab65Private_AddRef(IEcoUnknownPtr_t me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
        if (state == 0) return 0;

        state->local_refs++;
        return state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD Lab65Private_Release(IEcoUnknownPtr_t me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
        if (state == 0) return 0;

        state->local_refs--;
        if (state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->owned_system != 0) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            if (state->inner_x != 0) {
                state->inner_x->pVTbl->Release(state->inner_x);
            }
            if (state->inner_y != 0) {
                state->inner_y->pVTbl->Release(state->inner_y);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
            return 0;
        }

        return state->local_refs;
    }
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    if (me == 0) return 0;

    {
        Lab65State* state;
        state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        if (state == 0) return 0;

        return state->owned_allocator;
    }
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* local_system = 0;
    IEcoInterfaceBus1* local_bus = 0;
    IEcoInterfaceBus1MemExt* local_extension = 0;
    IEcoMemoryAllocator1* local_allocator = 0;
    Lab65State* state = 0;
    UGUID manager_guid = CID_EcoMemoryManager1;
    int16_t status = 0;

    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;

    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;

    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&local_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = local_system->pVTbl->QueryInterface(local_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&local_bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = local_bus->pVTbl->QueryInterface(local_bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&local_extension);
    if (status == ERR_ECO_SUCCESS) {
        const UGUID* manager_guid_ptr = local_extension->pVTbl->get_Manager(local_extension);
        if (manager_guid_ptr != 0) {
            manager_guid = *manager_guid_ptr;
        }
        local_extension->pVTbl->Release(local_extension);
    }

    status = local_bus->pVTbl->QueryComponent(local_bus, &manager_guid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&local_allocator);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    state = (Lab65State*)local_allocator->pVTbl->Alloc(local_allocator, sizeof(Lab65State));
    if (state == 0) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab65.pVTbl = &lab65_vtbl;
    state->private_unknown.pVTbl = &lab65private_vtbl;
    state->public_x.pVTbl = &lab65x_vtbl;
    state->public_y.pVTbl = &lab65y_vtbl;
    state->local_refs = 1;
    state->controller = outer ? outer : (IEcoUnknown*)&state->private_unknown;
    state->owned_system = local_system;
    state->owned_allocator = local_allocator;
    state->inner_x = 0;
    state->inner_y = 0;

    local_system = 0;
    local_allocator = 0;

    status = local_bus->pVTbl->QueryComponent(local_bus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != ERR_ECO_SUCCESS || state->inner_y == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = local_bus->pVTbl->QueryComponent(local_bus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != ERR_ECO_SUCCESS || state->inner_x == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    if (outer != 0) {
        *out = &state->private_unknown;
        state = 0; /* Transfer ownership to caller */
        goto cleanup;
    }

    status = state->controller->pVTbl->QueryInterface(state->controller, iid, out);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

cleanup:
    if (state != 0) {
        Lab65Private_Release(&state->private_unknown);
    }

    if (local_system != 0) {
        local_system->pVTbl->Release(local_system);
    }

    if (local_bus != 0) {
        local_bus->pVTbl->Release(local_bus);
    }

    if (local_allocator != 0) {
        local_allocator->pVTbl->Release(local_allocator);
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
    IEcoUnknown* owned_inner_unknown; /* OWNS the private nondelegating Unknown of aggregate Lab65 */
} Lab2State;

static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoLab2Ptr_t me);
static uint32_t ECOCALLMETHOD Lab2_Release(IEcoLab2Ptr_t me);
static uint32_t ECOCALLMETHOD Lab2_GetVersion(IEcoLab2Ptr_t me);

static int16_t ECOCALLMETHOD Lab2X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD Lab2X_AddRef(IEcoCalculatorXPtr_t me);
static uint32_t ECOCALLMETHOD Lab2X_Release(IEcoCalculatorXPtr_t me);
static int32_t ECOCALLMETHOD Lab2X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD Lab2X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);

static int16_t ECOCALLMETHOD Lab2Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD Lab2Y_AddRef(IEcoCalculatorYPtr_t me);
static uint32_t ECOCALLMETHOD Lab2Y_Release(IEcoCalculatorYPtr_t me);
static int32_t ECOCALLMETHOD Lab2Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);
static int16_t ECOCALLMETHOD Lab2Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);

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

static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab2State* state;
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

        if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
            return state->owned_inner_unknown->pVTbl->QueryInterface(state->owned_inner_unknown, riid, ppv);
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoLab2Ptr_t me) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        if (state == 0) return 0;

        state->local_refs++;
        return state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD Lab2_Release(IEcoLab2Ptr_t me) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        if (state == 0) return 0;

        state->local_refs--;
        if (state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->owned_system != 0) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            if (state->owned_inner_x != 0) {
                state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
            }
            if (state->owned_inner_y != 0) {
                state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
            }
            if (state->owned_inner_unknown != 0) {
                state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
            return 0;
        }

        return state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD Lab2_GetVersion(IEcoLab2Ptr_t me) {
    if (me == 0) return 0;
    return 2;
}

static int16_t ECOCALLMETHOD Lab2X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        if (state == 0) return ERR_ECO_POINTER;

        return Lab2_QueryInterface(&state->public_lab2, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab2X_AddRef(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        if (state == 0) return 0;

        return Lab2_AddRef(&state->public_lab2);
    }
}

static uint32_t ECOCALLMETHOD Lab2X_Release(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        if (state == 0) return 0;

        return Lab2_Release(&state->public_lab2);
    }
}

static int32_t ECOCALLMETHOD Lab2X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        if (state == 0 || state->owned_inner_x == 0) return 0;

        return state->owned_inner_x->pVTbl->Addition(state->owned_inner_x, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab2X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
        if (state == 0 || state->owned_inner_x == 0) return 0;

        return state->owned_inner_x->pVTbl->Subtraction(state->owned_inner_x, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab2Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;

    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
        if (state == 0) return ERR_ECO_POINTER;

        return Lab2_QueryInterface(&state->public_lab2, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab2Y_AddRef(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
        if (state == 0) return 0;

        return Lab2_AddRef(&state->public_lab2);
    }
}

static uint32_t ECOCALLMETHOD Lab2Y_Release(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
        if (state == 0) return 0;

        return Lab2_Release(&state->public_lab2);
    }
}

static int32_t ECOCALLMETHOD Lab2Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
        if (state == 0 || state->owned_inner_y == 0) return 0;

        return state->owned_inner_y->pVTbl->Multiplication(state->owned_inner_y, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab2Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;

    {
        Lab2State* state;
        state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
        if (state == 0 || state->owned_inner_y == 0) return 0;

        return state->owned_inner_y->pVTbl->Division(state->owned_inner_y, a, b);
    }
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* local_system = 0;
    IEcoInterfaceBus1* local_bus = 0;
    IEcoInterfaceBus1MemExt* local_extension = 0;
    IEcoMemoryAllocator1* local_allocator = 0;
    Lab2State* state = 0;
    UGUID manager_guid = CID_EcoMemoryManager1;
    int16_t status = 0;

    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;

    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;

    if (outer != 0) return ERR_ECO_NOAGGREGATION;

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&local_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = local_system->pVTbl->QueryInterface(local_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&local_bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = local_bus->pVTbl->QueryInterface(local_bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&local_extension);
    if (status == ERR_ECO_SUCCESS) {
        const UGUID* manager_guid_ptr = local_extension->pVTbl->get_Manager(local_extension);
        if (manager_guid_ptr != 0) {
            manager_guid = *manager_guid_ptr;
        }
        local_extension->pVTbl->Release(local_extension);
    }

    status = local_bus->pVTbl->QueryComponent(local_bus, &manager_guid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&local_allocator);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    state = (Lab2State*)local_allocator->pVTbl->Alloc(local_allocator, sizeof(Lab2State));
    if (state == 0) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab2.pVTbl = &lab2_primary_vtbl;
    state->public_x.pVTbl = &lab2_x_vtbl;
    state->public_y.pVTbl = &lab2_y_vtbl;
    state->local_refs = 1;
    state->owned_system = local_system;
    state->owned_allocator = local_allocator;
    state->owned_inner_x = 0;
    state->owned_inner_y = 0;
    state->owned_inner_unknown = 0;

    local_system = 0;
    local_allocator = 0;

    status = local_bus->pVTbl->QueryComponent(local_bus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->owned_inner_y);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_y == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = state->owned_inner_y->pVTbl->QueryInterface(state->owned_inner_y, &IID_IEcoCalculatorX, (voidptr_t*)&state->owned_inner_x);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_x == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = local_bus->pVTbl->QueryComponent(local_bus, &CID_EcoLab65, (IEcoUnknown*)&state->public_lab2, &IID_IEcoUnknown, (voidptr_t*)&state->owned_inner_unknown);
    if (status != ERR_ECO_SUCCESS || state->owned_inner_unknown == 0) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = state->public_lab2.pVTbl->QueryInterface(&state->public_lab2, iid, out);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

cleanup:
    if (state != 0) {
        Lab2_Release(&state->public_lab2);
    }

    if (local_system != 0) {
        local_system->pVTbl->Release(local_system);
    }

    if (local_bus != 0) {
        local_bus->pVTbl->Release(local_bus);
    }

    if (local_allocator != 0) {
        local_allocator->pVTbl->Release(local_allocator);
    }

    return status;
}
