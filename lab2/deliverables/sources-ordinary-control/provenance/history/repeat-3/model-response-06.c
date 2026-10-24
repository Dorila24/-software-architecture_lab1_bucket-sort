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
    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;

    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;

    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    IEcoSystem1* owned_system = 0;
    IEcoInterfaceBus1* bus = 0;
    IEcoInterfaceBus1MemExt* extension = 0;
    IEcoMemoryAllocator1* owned_allocator = 0;
    Lab65State* state = 0;
    UGUID memory_manager_cid = CID_EcoMemoryManager1;
    const UGUID* temp_manager = 0;
    int16_t status = ERR_ECO_SUCCESS;

    // Phase A
    *out = 0;
    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    // Phase B
    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&extension);
    if (status == ERR_ECO_SUCCESS) {
        temp_manager = extension->pVTbl->get_Manager(extension);
        if (temp_manager != 0) memory_manager_cid = *temp_manager;
        extension->pVTbl->Release(extension);
    }

    status = bus->pVTbl->QueryComponent(bus, &memory_manager_cid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    // Phase C
    state = (Lab65State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
    if (state == 0) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab65.pVTbl = &lab65_vtbl;
    state->private_unknown.pVTbl = &lab65private_vtbl;
    state->public_x.pVTbl = &lab65x_vtbl;
    state->public_y.pVTbl = &lab65y_vtbl;
    state->local_refs = 1;
    state->controller = (outer != 0) ? outer : (IEcoUnknown*)&state->private_unknown;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->inner_x = 0;
    state->inner_y = 0;

    owned_system = 0;
    owned_allocator = 0;

    // Phase D
    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != ERR_ECO_SUCCESS || state->inner_y == 0) goto cleanup;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != ERR_ECO_SUCCESS || state->inner_x == 0) goto cleanup;

    // Phase E
    if (outer != 0) {
        *out = &state->private_unknown;
        state->local_refs++;
        bus->pVTbl->Release(bus);
        return ERR_ECO_SUCCESS;
    }

    status = Lab65Private_QueryInterface((IEcoUnknown*)&state->private_unknown, iid, out);
    if (status != ERR_ECO_SUCCESS) goto cleanup;

    Lab65Private_Release((IEcoUnknown*)&state->private_unknown);
    bus->pVTbl->Release(bus);
    return status;

cleanup:
    if (state != 0) Lab65Private_Release((IEcoUnknown*)&state->private_unknown);
    if (owned_allocator != 0) owned_allocator->pVTbl->Release(owned_allocator);
    if (bus != 0) bus->pVTbl->Release(bus);
    return status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;

    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;

    if (outer != 0) return ERR_ECO_NOAGGREGATION;

    return ERR_ECO_NOINTERFACE;
}
